#include "client_protocol.h"
#include <stdlib.h>
#include <arpa/inet.h>

#define SUCCESS 0
#define ERROR -1

void client_protocol_send(client_protocol_t *self, char *buffer, ssize_t size);
void client_protocol_send_size(client_protocol_t *self, ssize_t size);
void client_protocol_send_chunk(client_protocol_t *self, char *buffer, ssize_t size);

int client_protocol_init(client_protocol_t *self, 
						 const char *host, 
						 const char *service) {
	int status = SUCCESS;
	socket_init(&(self->socket));
	status = socket_connect(&(self->socket), host, service);
	return status;
}

void client_protocol_run(client_protocol_t *self, FILE *file) {
	
	//ssize_t socket_send(socket_t *self, const void *buffer, size_t length)
	//ssize_t socket_receive(socket_t *self, void *buffer, size_t length);

	char *buffer = NULL;
	size_t buffer_size = 0;
	ssize_t read_bytes = getline(&buffer, &buffer_size, file);
	while(read_bytes != EOF) {
		client_protocol_send(self, buffer, read_bytes);
		//client_protocol_send_size(self, read_bytes);
		//client_protocol_send_chunk(self, buffer, read_bytes);
		//fprintf(stdout, "%02x\n", buff[read_bytes-1]);
		//fprintf(stdout, "%li\n", read_bytes);
		read_bytes = getline(&buffer, &buffer_size, file);
	}
	free(buffer);

}

void client_protocol_uninit(client_protocol_t *self) {
	socket_uninit(&(self->socket));
}

void client_protocol_send(client_protocol_t *self, char *buffer, ssize_t size) {
	client_protocol_send_size(self, size);
	client_protocol_send_chunk(self, buffer, size);
}

void client_protocol_send_size(client_protocol_t *self, ssize_t size) {
	uint16_t bytes_to_send = (uint16_t)size;
	bytes_to_send = htons(bytes_to_send);
	printf("%04x\n", bytes_to_send);
	socket_send(&(self->socket), (const void*)(&bytes_to_send), sizeof(uint16_t));
}

void client_protocol_send_chunk(client_protocol_t *self, char *buffer, ssize_t size) {
	socket_send(&(self->socket), (const void*)buffer, size);
}