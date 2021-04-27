#ifndef _CLIENT_PROTOCOL_
#define _CLIENT_PROTOCOL_

#include "socket.h"
#include <stdio.h>

typedef struct client_protocol {
	socket_t socket;
} client_protocol_t;

int client_protocol_init(client_protocol_t *self, 
						 const char *host, 
						 const char *service);
void client_protocol_run(client_protocol_t *self, FILE *file);
void client_protocol_uninit(client_protocol_t *self);

	/*char *buff = NULL;
	size_t buff_size = 0;
	ssize_t bytes_read = getline(&buff, &buff_size, file);
	while(bytes_read != EOF) {

		//fprintf(stdout, "%02x\n", buff[bytes_read-1]);
		//fprintf(stdout, "%li\n", bytes_read);
		bytes_read = getline(&buff, &buff_size, file);
	}	
	free(buff);*/

#endif