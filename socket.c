#include "socket.h"
#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>

#define SUCCESS 0
#define ERROR -1
#define SERVER_SIDE_FLAGS AI_PASSIVE
#define CLIENT_SIDE_FLAGS 0

int _socket_is_in_valid_state(socket_t *self);
int _socket_addrinfo_iterate(socket_t *self, struct addrinfo *result);
int _socket_create_and_connect(socket_t *self, struct addrinfo **result);
int _socket_get_addrinfo(socket_t *self, 
						  const char *host, 
						  const char *service,
						  const int FLAGS, 
						  struct addrinfo **result);


void socket_init(socket_t *self) {
	self->file_descriptor = ERROR;
}

void socket_uninit(socket_t *self) {
	if (_socket_is_in_valid_state(self)) {
		shutdown(self->file_descriptor, SHUT_RDWR);
		close(self->file_descriptor);
	}
}

void socket_bind_and_listen(socket_t *self, 
							const char *host,
							const char *service) {
}

void socket_accept(socket_t *listener, socket_t *peer) {
}


int socket_connect(socket_t *self, const char *host, const char *service) {
	struct addrinfo *result = NULL;
	int rt_code = 0;
	rt_code = _socket_get_addrinfo(self, host, service, CLIENT_SIDE_FLAGS, &result);
	
	if (rt_code == SUCCESS) {
		rt_code = _socket_addrinfo_iterate(self, result);
		freeaddrinfo(result);
	}
	return rt_code;
}

ssize_t socket_send(socket_t *self, const char *buffer, size_t length) {
	return -1;
}

ssize_t socket_receive(socket_t *self, char *buffer, size_t length) {
	return -1;
}

int _socket_is_in_valid_state(socket_t *self) {
	return (self != NULL && self->file_descriptor != ERROR);
}

int _socket_get_addrinfo(socket_t *self, 
								 const char *host, 
								 const char *service,
								 const int flags, 
								 struct addrinfo **result) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = flags;

	if (getaddrinfo(host, service, &hints, result) != ERROR) {
		return SUCCESS;
	}
	return ERROR;
}

int _socket_addrinfo_iterate(socket_t *self, struct addrinfo *result) {
	int rt_code = ERROR;

	while (result != NULL && rt_code == ERROR) {
		rt_code = _socket_create_and_connect(self, &result);
	}
	return rt_code;
}

int _socket_create_and_connect(socket_t *self, struct addrinfo **result) {
	int rt_code = 0;
	self->file_descriptor = socket((*result)->ai_family, (*result)->ai_socktype, (*result)->ai_protocol);
	rt_code = connect(self->file_descriptor, (*result)->ai_addr, (*result)->ai_addrlen);

	if (rt_code == ERROR) {
		close(self->file_descriptor);
		*result = (*result)->ai_next;
	}
	return rt_code;
}
