#include "server_protocol.h"
#include "common_communication_protocol.h"
#include "server_hill_cipher.h"
#include <stdlib.h>

#define HOST "localhost"
#define PEER_CLOSED 0
#define SUCCESS 0
#define ERROR -1

/**
 * @brief Espera por una conexion entrante del cliente.
 * @param service: Puerto de la máquina del servidor.
 * @param peer: Socket de comunicacion entre cliente y servidor.
 */
static int server_protocol_wait_for_connection(server_protocol_t *self, 
											   const char *service, 
											   socket_t *peer);

void server_protocol_init(server_protocol_t *self) {
	socket_init(&(self->_socket));
}

void server_protocol_uninit(server_protocol_t *self) {
	socket_uninit(&(self->_socket));
}

void server_protocol_run(server_protocol_t *self, 
						const char *service, 
						const unsigned char *key, 
						size_t key_length) {
	char *client_request = NULL; 
	socket_t peer;
	comm_protocol_t comm_protocol;
	hill_cipher_t hill_cipher;

	socket_init(&peer);
	comm_protocol_init(&comm_protocol, &peer);
	hill_cipher_init(&hill_cipher, key, key_length);
	if (server_protocol_wait_for_connection(self, service, &peer) != ERROR) {
		ssize_t request_size = ERROR;
		while (request_size != PEER_CLOSED) {
			request_size = comm_protocol_receive(&comm_protocol, &client_request);
			if (request_size != PEER_CLOSED) {
				hill_cipher_encode(&hill_cipher, 
								   (const unsigned char*)client_request, 
								   request_size);
				hill_cipher_output_result(&hill_cipher, comm_protocol_send, &comm_protocol);
			}
		}
	}
	hill_cipher_uninit(&hill_cipher);
	comm_protocol_uninit(&comm_protocol);
	socket_uninit(&peer);
}

static int server_protocol_wait_for_connection(server_protocol_t *self,  
											   const char *service, 
											   socket_t *peer) {
	int status = ERROR;
	if (socket_bind_and_listen(&(self->_socket), HOST, service) != ERROR) {
		status = socket_accept(&(self->_socket), peer);
	}
	return status;
}
