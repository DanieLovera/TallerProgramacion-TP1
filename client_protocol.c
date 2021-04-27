#include "client_protocol.h"
#include "common_communication_protocol.h"
#include "maplib.h"
#include <stdlib.h>
#include <arpa/inet.h>

#define SUCCESS 0
#define ERROR -1
#define OUTPUT stdout

void client_protocol_read_and_process(client_protocol_t *self, 
									  comm_protocol_t *comm_protocol, 
									  FILE *file);
void client_protocol_output_result(client_protocol_t *self, 
								   char *buffer, 
								   size_t size);

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
	client_protocol_read_and_process(self, &comm_protocol, file);
	comm_protocol_uninit(&comm_protocol);
}

void client_protocol_uninit(client_protocol_t *self) {
	socket_uninit(&(self->socket));
}

void client_protocol_output_result(client_protocol_t *self, 
								   char *buffer, 
								   size_t size) {
	hill_cipher_unmap((unsigned char*)buffer, size);
	fwrite(buffer, 1, size, OUTPUT);
	printf("\n");
}

void client_protocol_read_and_process(client_protocol_t *self, 
									  comm_protocol_t *comm_protocol, 
									  FILE *file) {
	char *server_response = NULL; 
	char *buffer = NULL;
	ssize_t response_size = 0;
	size_t buffer_size = 0;

	ssize_t read_bytes = getline(&buffer, &buffer_size, file);
	while(read_bytes != EOF) {
		comm_protocol_client_send(comm_protocol, buffer, read_bytes);
		comm_protocol_client_receive(comm_protocol, 
									 &server_response, 
									 &response_size);
		client_protocol_output_result(self, server_response, (size_t)response_size);
		read_bytes = getline(&buffer, &buffer_size, file);
	}
	free(buffer);
}
