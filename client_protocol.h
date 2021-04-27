#ifndef _CLIENT_PROTOCOL_
#define _CLIENT_PROTOCOL_

#include "common_socket.h"
#include <stdio.h>

typedef struct client_protocol {
	socket_t socket;
} client_protocol_t;

/**
 * @brief Inicializa el protocolo del cliente.
 * @param host: Dominio de la maquina en donde correra el cliente.
 * @param service: Puerto de la maquina del cliente.
 */
int client_protocol_init(client_protocol_t *self, 
						 const char *host, 
						 const char *service);

/**
 * @brief Ejecuta el protocolo cliente.
 * @param file: archivo desde el cual el cliente lee la informacion.
 * @param service: Puerto de la maquina en el que se ubicara un socket.
 */
void client_protocol_run(client_protocol_t *self, FILE *file);

/**
 * @brief Libera los recursos del protocolo cliente.
 */
void client_protocol_uninit(client_protocol_t *self);

#endif
