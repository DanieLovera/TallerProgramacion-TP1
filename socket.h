#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/types.h>
#include <stddef.h>

typedef struct socket {
	int file_descriptor;
} socket_t;

void socket_init(socket_t *self);
void socket_uninit(socket_t *self);
void socket_bind_and_listen(socket_t *self, 
							const char *host,
							const char *service);
void socket_accept(socket_t *listener, socket_t *peer);
int socket_connect(socket_t *self, const char *host, const char *service);
ssize_t socket_send(socket_t *self, const char *buffer, size_t length);
ssize_t socket_receive(socket_t *self, char *buffer, size_t length);

#endif
