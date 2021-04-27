#ifndef _COMMON_COMMUNICATION_PROTOCOL_
#define _COMMON_COMMUNICATION_PROTOCOL_

#include "common_socket.h"	
#include "client_protocol.h"

typedef struct communication_protocol {
	char *buffer;
	socket_t *socket;
} comm_protocol_t;

/**
 * @brief Inicializa el protocolo de comunicacion entre cliente y servidor.
 * @param socket: Guardara el puntero al socket que se utiliza 
 * en la comunicacion. Debe estar correctamente inicializado fuera del protocolo.
 */
void comm_protocol_init(comm_protocol_t *self, socket_t *socket);

/**
 * @brief Libera los recursos del protocolo.
 */
void comm_protocol_uninit(comm_protocol_t *self);

/**
 * @brief Protocolo para enviar los datos del cliente al servidor.
 * @param buffer: Guarda el contenido del buffer que se va a enviar al servidor.
 * @param size: Guarda el tamanio del buffer a enviar. 
 */
void comm_protocol_client_send(comm_protocol_t *self, 
							   char *buffer, 
							   ssize_t size);

/**
 * @brief Protocolo para recibir los datos del servidor al cliente.
 * @param buffer: Modifica y guarda la direccion de memoria del 
 * buffer que almacenara los datos que envia el servidor.
 * @param size: Modifica y guarda el tamanio del buffer a recibir. 
 */
void comm_protocol_client_receive(comm_protocol_t *self, 
								  char **buffer, 
								  ssize_t *size);

#endif
