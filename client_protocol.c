#include "client_protocol.h"
#include "common_communication_protocol.h"
#include <stdlib.h>
#include <arpa/inet.h>

#define SUCCESS 0
#define ERROR -1

int client_protocol_init(client_protocol_t *self, 
						 const char *host, 
						 const char *service) {
	int status = SUCCESS;
	socket_init(&(self->socket));
	status = socket_connect(&(self->socket), host, service);
	return status;
}

void client_protocol_run(client_protocol_t *self, FILE *file) {
	comm_protocol_t comm_protocol;
	comm_protocol_init(&comm_protocol, &(self->socket));
	char *server_response = NULL; 
	ssize_t response_size = 0;
	
	char *buffer = NULL;
	size_t buffer_size = 0;
	ssize_t read_bytes = getline(&buffer, &buffer_size, file);
	while(read_bytes != EOF) {
		comm_protocol_client_send(&comm_protocol, buffer, read_bytes);
		printf("%s", buffer);
		comm_protocol_client_receive(&comm_protocol, 
									 &server_response, 
									 &response_size);

		read_bytes = getline(&buffer, &buffer_size, file);
	}
	comm_protocol_uninit(&comm_protocol);
	free(buffer);
}

void client_protocol_uninit(client_protocol_t *self) {
	socket_uninit(&(self->socket));
}
