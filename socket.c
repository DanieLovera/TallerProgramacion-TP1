#include "socket.h"
#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#define SUCCESS 0
#define ERROR -1
#define SERVER_SIDE_FLAGS AI_PASSIVE
#define CLIENT_SIDE_FLAGS 0

#define DEBUG_INFO
#define DEBUG_PRINT(MSG, VALUE) fprintf(stderr, MSG, VALUE);

static int _socket_is_in_valid_state(socket_t *self);
static int _socket_addrinfo_iterate(socket_t *self, struct addrinfo *result);
static int _socket_create_and_connect(socket_t *self, struct addrinfo **result);
static int _socket_get_addrinfo(socket_t *self, 
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
		#ifdef DEBUG_INFO
		DEBUG_PRINT("'shutdown' in socket_uninit:socket.c: %s\n", 
					 strerror(errno))
		#endif
		close(self->file_descriptor);
		#ifdef DEBUG_INFO
		DEBUG_PRINT("'close' in socket_uninit:socket.c: %s\n", 
					 strerror(errno))
		#endif
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
	int rt_code = _socket_get_addrinfo(self, 
									   host, 
									   service, 
									   CLIENT_SIDE_FLAGS, 
									   &result);
	
	if (rt_code == SUCCESS) {
		rt_code = _socket_addrinfo_iterate(self, result);
		freeaddrinfo(result);
	}
	return rt_code;
}

ssize_t socket_send(socket_t *self, const void *buffer, size_t length) {
	const char *current_buffer = buffer;
	int size_of_long_send = sizeof(uint32_t);
	int size_of_short_send = sizeof(char);
	uint32_t bytes_to_send = 0;
	size_t sent_bytes = 0;
	size_t sent_bytes_aux = 0;

	while (sent_bytes < length && sent_bytes != ERROR) {
		if ((length - sent_bytes) >= size_of_long_sent) {
			bytes_to_send = *((uint32_t*)current_buffer);
			bytes_to_send = htonl(bytes_to_send);
			sent_bytes_aux = send(self->file_descriptor, &bytes_to_send, size_of_long_send, MSG_NOSIGNAL);
		} else {
			sent_bytes_aux = send(self->file_descriptor, current_buffer, size_of_short_send, MSG_NOSIGNAL);
		}
		sent_bytes += sent_bytes_aux;
		current_buffer += sent_bytes_aux;
	}
	return sent_bytes;
}

ssize_t socket_receive(socket_t *self, void *buffer, size_t length) {
	return -1;
}

static int _socket_is_in_valid_state(socket_t *self) {
	return (self != NULL && self->file_descriptor != ERROR);
}

static int _socket_get_addrinfo(socket_t *self, 
								const char *host, 
								const char *service, 
								const int flags, 
								struct addrinfo **result) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = flags;

	int rt_code = getaddrinfo(host, service, &hints, result);
	if (rt_code == SUCCESS) {
		return rt_code;
	}
	#ifdef DEBUG_INFO
	DEBUG_PRINT("'getaddrinfo' in _socket_get_addrinfo:socket.c: %s\n", 
				 gai_strerror(rt_code))
	#endif
	return rt_code;
}

static int _socket_addrinfo_iterate(socket_t *self, struct addrinfo *result) {
	int rt_code = ERROR;

	while (result != NULL && rt_code == ERROR) {
		rt_code = _socket_create_and_connect(self, &result);
	}
	return rt_code;
}

static int _socket_create_and_connect(socket_t *self, 
									  struct addrinfo **result) {
	int rt_code = ERROR;
	self->file_descriptor = socket((*result)->ai_family, 
								   (*result)->ai_socktype, 
								   (*result)->ai_protocol);
	#ifdef DEBUG_INFO
	DEBUG_PRINT("'socket' in _socket_create_and_connect:socket.c: %s\n", 
				 strerror(errno))
	#endif
	rt_code = connect(self->file_descriptor, 
					  (*result)->ai_addr, 
					  (*result)->ai_addrlen);
	#ifdef DEBUG_INFO
	DEBUG_PRINT("'connect' in _socket_create_and_connect:socket.c: %s\n", 
				 strerror(errno))
	#endif
	if (rt_code == ERROR) {
		close(self->file_descriptor);
		#ifdef DEBUG_INFO
		DEBUG_PRINT("'close' in _socket_create_and_connect:socket.c: %s\n", 
					 strerror(errno))
		#endif
		*result = (*result)->ai_next;
	}
	return rt_code;
}
