#ifndef _COMMON_COMMUNICATION_PROTOCOL_
#define _COMMON_COMMUNICATION_PROTOCOL_

#include "common_socket.h"	
#include "client_protocol.h"

typedef struct communication_protocol {
	char *_buffer;
	socket_t *_socket;
} comm_protocol_t;

/**
 * @brief Inicializa el protocolo de comunicacion entre cliente y servidor.
 * @param socket: Guardara el puntero al socket que se utiliza involucrado en 
 * en la comunicacion y debe estar preparado preparado para
 * recibir/enviar informacion.
 */
void comm_protocol_init(comm_protocol_t *self, socket_t *socket);

/**
 * @brief Libera los recursos usados por el protocolo.
 */
void comm_protocol_uninit(comm_protocol_t *self);

/**
 * @brief Protocolo para enviar datos (actua como callback), 
 * debe recibir como parametro self un comm_protocol_t, si no se 
 * cumple resultara en error.
 * @param buffer: Guarda el contenido del buffer que se va a enviar.
 * @param size: Guarda el tamanio del buffer a enviar. 
 * @return Devuelve la cantidad de bytes enviados, en caso de error
 * devuelve -1.
 */
ssize_t comm_protocol_send(void *self, 
						   const unsigned char *buffer, 
						   ssize_t size);

/**
 * @brief Protocolo para recibir datos.
 * @param buffer: Modifica y guarda la direccion de memoria del 
 * buffer que almacenara los datos que se reciben
 * @return devuelve la cantidad de bytes recibidos o -1 en caso de error,
 * los datos recibidos se envian a traves de buffer
 */
ssize_t comm_protocol_receive(comm_protocol_t *self, char **buffer);

#endif
