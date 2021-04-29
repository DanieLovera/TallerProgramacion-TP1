#ifndef _SERVER_PROTOCOL_
#define _SERVER_PROTOCOL_

#include "common_socket.h"
#include <stdio.h>

typedef struct server_protocol {
	socket_t _socket;
} server_protocol_t;

void server_protocol_init(server_protocol_t *self);
void server_protocol_run(server_protocol_t *self, 
						 const char *service, 
						 const unsigned char *key, 
						 size_t key_length);
void server_protocol_uninit(server_protocol_t *self);

#endif