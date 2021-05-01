#include "client_protocol.h"
#include "common_communication_protocol.h"
#include "common_maplib.h"
#include <stdlib.h>
#include <arpa/inet.h>

#define SUCCESS 0
#define ERROR -1
#define OUTPUT stdout

/**
 * @brief Lee un archivo de texto y lo procesa segun el protocolo.
 * @param comm_protocol: protocolo de comunicacion con el servidor.
 * @param file: archivo a leer.
 */
static void client_protocol_read_and_process(client_protocol_t *self, 
									  		 comm_protocol_t *comm_protocol, 
									  		 FILE *file);

/**
 * @brief Muestra el resultado esperado.
 * @param buffer: contenido del resultado.
 * @param size: tamanio del resultado.
 */
static void client_protocol_print_result(client_protocol_t *self, 
	   								   	  char *buffer, 
									   	  size_t size);

void client_protocol_init(client_protocol_t *self) {
	socket_init(&(self->_socket));
}

void client_protocol_uninit(client_protocol_t *self) {
	socket_uninit(&(self->_socket));
}

void client_protocol_run(client_protocol_t *self, 
						 const char *host, 
						 const char *service, 
						 FILE *file) {
	int status = ERROR;
	comm_protocol_t comm_protocol;
	status = socket_connect(&(self->_socket), host, service);
	if (status == SUCCESS) {
		comm_protocol_init(&comm_protocol, &(self->_socket));
		client_protocol_read_and_process(self, &comm_protocol, file);
		comm_protocol_uninit(&comm_protocol);
	}
}

static void client_protocol_read_and_process(client_protocol_t *self, 
									  		 comm_protocol_t *comm_protocol, 
									  		 FILE *file) {
	char *server_response = NULL; 
	char *buffer = NULL;
	size_t buffer_size = 0;

	ssize_t read_bytes = getline(&buffer, &buffer_size, file);
	while(read_bytes != EOF) {
		ssize_t response_size;
		comm_protocol_send(comm_protocol, (unsigned char*)buffer, read_bytes);
		response_size = comm_protocol_receive(comm_protocol, 
								&server_response);
		client_protocol_print_result(self, server_response, (size_t)response_size);
		read_bytes = getline(&buffer, &buffer_size, file);
	}
	free(buffer);
}

static void client_protocol_print_result(client_protocol_t *self, 
								   		  char *buffer, 
								   		  size_t size) {
	hill_cipher_unmap((unsigned char*)buffer, size);
	fwrite(buffer, 1, size, OUTPUT);
	printf("\n");
}
