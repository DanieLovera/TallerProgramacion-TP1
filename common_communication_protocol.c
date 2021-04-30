#include "common_communication_protocol.h"
#include <stdlib.h>
#include <arpa/inet.h>

#define PEER_CLOSED 0
#define ERROR -1

/**
 * @brief Envia el tamanio del envio que se quiere realizar
 * el cual seran 2 bytes en big endian.
 * @param size: Guarda el el tamanio del envio.
 * @return Devuelve la cantidad de datos enviados o -1 en caso
 * de error.
 */
static ssize_t _comm_protocol_send_size(comm_protocol_t *self, 
									 	ssize_t size);

/**
 * @brief Envia el stream de datos.
 * @param buffer: Puntero al contenido que se quiere enviar.
 * @param size: Tamanio del contenido a enviar.
 * @return Devuelve la cantidad de datos enviados o -1 en caso
 * de error.
 */
static ssize_t _comm_protocol_send_chunk(comm_protocol_t *self, 
									  char *buffer, 
									  ssize_t size);

/**
 * Recibe el tamanio del envio que se quiere recibir, los cuales 
 * deben ser 2 bytes en big endian.
 * @param size: Guarda el valor del tamanio de datos que se van a recibir.
 * @return Devuelve en size la cantidad de datos que se van a recibir y
 * por retorno la cantidad de datos que recibidos, en caso de error
 * sera -1.
 */
static ssize_t _comm_protocol_receive_size(comm_protocol_t *self, 
										   ssize_t *size);

/**
 * @brief Recibe el stream de de datos.
 * @param buffer: Modifica y guarda la direccion de memoria del 
 * buffer que almacenara los datos que se reciben.
 * @param size: Guarda el tamanio del buffer a recibir.
 * @return Devuelve en size la cantidad de datos que se van a recibir y
 * por retorno la cantidad de datos que recibidos, en caso de error
 * sera -1.
 */
static ssize_t _comm_protocol_receive_chunk(comm_protocol_t *self, 
										    char **buffer, 
										 	ssize_t size);

static void comm_protocol_free(char* buffer);

void comm_protocol_init(comm_protocol_t *self, socket_t *socket) {
	self->_buffer = NULL;
	self->_socket = socket;
}

void comm_protocol_uninit(comm_protocol_t *self) {
	comm_protocol_free(self->_buffer);
	self->_socket = NULL;
}

ssize_t comm_protocol_send(void *self, 
						   unsigned char *buffer, 
						   ssize_t size) {
	comm_protocol_t *comm_protocol = (comm_protocol_t*)self;
	ssize_t sent_bytes = _comm_protocol_send_size(comm_protocol, size);
	if (sent_bytes != ERROR) {
		sent_bytes = _comm_protocol_send_chunk(comm_protocol, (char*)buffer, size);
	}
	return sent_bytes;
}

ssize_t comm_protocol_receive(comm_protocol_t *self, char **buffer) {
	ssize_t size = 0; 
	ssize_t received_bytes = 0;

	received_bytes = _comm_protocol_receive_size(self, &size);
	if (received_bytes != PEER_CLOSED && received_bytes != ERROR) {
		received_bytes = _comm_protocol_receive_chunk(self, buffer, size);
	}
	return received_bytes;
}

static ssize_t _comm_protocol_send_size(comm_protocol_t *self, 
									 	ssize_t size) {
	ssize_t sent_bytes = 0;
	uint16_t bytes_to_send = (uint16_t)size;
	bytes_to_send = htons(bytes_to_send);
	sent_bytes = socket_send(self->_socket, 
							 (const void*)(&bytes_to_send), 
							 sizeof(uint16_t));
	return sent_bytes != sizeof(uint16_t) ? ERROR : sent_bytes;
}

static ssize_t _comm_protocol_send_chunk(comm_protocol_t *self, 
									  	 char *buffer, 
									  	 ssize_t size) {
	ssize_t sent_bytes = socket_send(self->_socket, (const void*)buffer, size);
	return sent_bytes != size ? ERROR : sent_bytes;
}

static ssize_t _comm_protocol_receive_size(comm_protocol_t *self, 
										   ssize_t *size) {
	uint16_t response_size = 0;
	ssize_t received_bytes = 0;

	received_bytes = socket_receive(self->_socket, 
									(void*)(&response_size), 
									sizeof(uint16_t));
	if (received_bytes != PEER_CLOSED) {
		if (received_bytes != sizeof(uint16_t)) {
			return ERROR;
		}
		response_size = ntohs(response_size);
		*size = response_size;
	}
	return received_bytes;
}

static ssize_t _comm_protocol_receive_chunk(comm_protocol_t *self, 
										    char **buffer, 
										 	ssize_t size) {
	ssize_t received_bytes = 0;
	comm_protocol_free(self->_buffer);

	self->_buffer = calloc(size, sizeof(char));
	received_bytes = socket_receive(self->_socket, (void*)(self->_buffer), size);
	if (received_bytes != PEER_CLOSED) {
		if (received_bytes != size) {
			return ERROR;
		}
		*(buffer) = self->_buffer;
	}
	return received_bytes;
}

static void comm_protocol_free(char *buffer) {
	if (buffer != NULL) {
		free(buffer);
	}
}
