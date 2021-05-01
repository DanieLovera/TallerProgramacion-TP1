#ifndef _SERVER_PROTOCOL_
#define _SERVER_PROTOCOL_

#include "common_socket.h"
#include <stdio.h>

typedef struct server_protocol {
	socket_t _socket;
} server_protocol_t;

/**
 * @brief Inicializa el protocolo del servidor
 */
void server_protocol_init(server_protocol_t *self);

/**
 * @brief Ejecuta el protocolo servidor.
 * @param service: Puerto de la maquina local en donde correra
 * el servivio.
 * @param key: Clave de cifrado que usa el servidor.
 * @param key_lenght: Longitud de la clave.
 */
void server_protocol_run(server_protocol_t *self, 
						 const char *service, 
						 const unsigned char *key, 
						 size_t key_length);

/**
 * @brief Libera los recursos del protocolo servidor.
 */
void server_protocol_uninit(server_protocol_t *self);

#endif
