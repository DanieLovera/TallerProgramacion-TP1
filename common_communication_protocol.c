#include "common_communication_protocol.h"
#include <stdlib.h>
#include <arpa/inet.h>

// PROTOCOLOS DE COMUNICACION DEL CLIENTE.
/**
 * @brief Envia el tamanio del envio que quiere realizar el cliente y
 * seran 2 bytes en big endian.
 * @param size: Guarda el valor a enviar.
 */
static void _comm_protocol_client_send_size(comm_protocol_t *self, 
											ssize_t size);

/**
 * @brief Envia el stream al servidor.
 * @param buffer: Puntero al contenido que se quiere enviar.
 * @param size: Tamanio del contenido a enviar.
 */
static void _comm_protocol_client_send_chunk(comm_protocol_t *self, 
											 char *buffer, 
											 ssize_t size);

/**
 * Recibe el tamanio del envio que quiere realizar el servidor y
 * seran 2 bytes en big endian.
 * @param size: Guarda el valor del tamanio de datos que se van a recibir.
 */
static void _comm_protocol_client_receive_size(comm_protocol_t *self, 
											   ssize_t *size);

/**
 * @brief Recibe el stream del servidor.
 * @param buffer: Modifica y guarda la direccion de memoria del 
 * buffer que almacenara los datos que envia el servidor.
 * @param size: Modifica y guarda el tamanio del buffer a recibir. 
 */
static void _comm_protocol_client_receive_chunk(comm_protocol_t *self, 
												char **buffer, 
												ssize_t size);

// PROTOCOLOS DE COMUNICACION DEL SERVIDOR.

void comm_protocol_init(comm_protocol_t *self, socket_t *socket) {
	self->buffer = NULL;
	self->socket = socket;
}

void comm_protocol_uninit(comm_protocol_t *self) {
	if (self->buffer != NULL) {
		free(self->buffer);
		self->buffer = NULL;
	}
	self->socket = NULL;
}

void comm_protocol_client_send(comm_protocol_t *self, 
							   char *buffer, 
							   ssize_t size) {
	_comm_protocol_client_send_size(self, size);
	_comm_protocol_client_send_chunk(self, buffer, size);
}

void comm_protocol_client_receive(comm_protocol_t *self, 
								  char **buffer, 
								  ssize_t *size) {
	//ssize_t size = 0;
	*size = 0;
	_comm_protocol_client_receive_size(self, size);
	_comm_protocol_client_receive_chunk(self, buffer, *size);
}

static void _comm_protocol_client_send_size(comm_protocol_t *self, 
											ssize_t size) {
	uint16_t bytes_to_send = (uint16_t)size;
	bytes_to_send = htons(bytes_to_send);
	//printf("%04x\n", bytes_to_send);
	socket_send(self->socket, (const void*)(&bytes_to_send), sizeof(uint16_t));
}

static void _comm_protocol_client_send_chunk(comm_protocol_t *self, 
											 char *buffer, 
											 ssize_t size) {
	socket_send(self->socket, (const void*)buffer, size);
}

static void _comm_protocol_client_receive_size(comm_protocol_t *self, 
											   ssize_t *size) {
	uint16_t response_size = 0;
	socket_receive(self->socket, (void*)(&response_size), sizeof(uint16_t));
	response_size = ntohs(response_size);
	*size = response_size;
}

static void _comm_protocol_client_receive_chunk(comm_protocol_t *self, 
												char **buffer, 
												ssize_t size) {
	self->buffer = calloc(size, sizeof(char));
	socket_receive(self->socket, (void*)(self->buffer), size);
	*(buffer) = self->buffer;
}
