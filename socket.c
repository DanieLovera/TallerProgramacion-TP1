//#define _POSIX_C_SOURCE 200112L
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
#define MAX_PENDING_CONNECTIONS 10
#define SERVER_SIDE_FLAGS AI_PASSIVE
#define CLIENT_SIDE_FLAGS 0

/**
 * @brief Llama a la funcion callback para cada nodo de la lista result.
 * @param callback: Puntero a funcion que realiza una acción sobre los
 * nodos de la lista result.
 * @param result: Guarda el resultado con la informacion de las address 
 * obtenidas.
 * @param context: Puntero generico que sirve de contexto al callback.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_addrinfo_iterate(int (*callback)(struct addrinfo *result, 
													void *context),  
									struct addrinfo *result, 
									void *context);

/**
 * @brief Crea y conecta un socket, generalmente usado por el cliente.
 * @param result: Guarda el resultado con la informacion de las address 
 * obtenidas.
 * @param context: Recibe la direccion de un socket como puntero generico
 * para adaptarse a la firma del callback.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_create_and_connect(struct addrinfo *result, void *context);

/**
 * @brief Crea y enlaza un socket, generalmente usado por el servidor.
 * @param result: Giarda el resultado con la informacion de las address 
 * obtenidas.
 * @param context: Recibe la direccion de un socket como puntero generico
 * para adaptarse a la firma del callback.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_create_and_bind(struct addrinfo *result, void *context);

/**
 * @brief Obtiene la informacion sobre la direccion de una maquina.
 * @param host: Dominio de la maquina.
 * @param service: Puerto de la maquina en el que se ubicara un socket.
 * @param flags: Informacion sobre el uso del socket, corresponden
 * a AI_FLAGS de getaddrinfo.
 * @param result: Guarda el resultado con la informacion de las address 
 * obtenidas.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_get_addrinfo(socket_t *self, 
							    const char *host, 
						  		const char *service,
						  		const int flags, 
						  		struct addrinfo **result);

/**
 * @brief Crea un relacionado la informacioon de result.
 * @param result: Guarda el resultado con la informacion de las address 
 * obtenidas.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_create(socket_t *self, struct addrinfo *result);

/**
 * @brief Enlaza un socket con la informacion de result.
 * @param result: Guarda el resultado con la informacion de las address 
 * obtenidas.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_bind(socket_t *self, struct addrinfo *result);

/**
 * @brief Coloca un socket pasivo a escuchar conexiones entrantes
 * @param result: Guarda el resultado con la informacion de las address 
 * obtenidas.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_listen(socket_t *self, int backlog);

/**
 * @brief Conecta un socket con la informacíon de result.
 * @param result: Contiene el resultado con la informacion de las
 * direcciones de las maquinas.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_connect(socket_t *socket, struct addrinfo *result);

/**
 * @brief Cierra la comunicacion de un socket, en lectura escritura o
 * ambos.
 * @param channel: Canal de comunicacion que sera cerrado, macros
 * que estan declaradas en <sys/socket.h>
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_shutdown(socket_t *self, int channel);

/**
 * @brief Libera los recursos usados por un socket.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
static int _socket_close(socket_t *self);

/**
 * @brief Valida el estado del socket.
 * @returns Devuelve un 0 en caso de exito o -s1 en caso de error.
 */
static int _socket_is_in_valid_state(socket_t *self);

void socket_init(socket_t *self) {
	self->file_descriptor = ERROR;
}

void socket_uninit(socket_t *self) {
	if (_socket_is_in_valid_state(self)) {
		_socket_shutdown(self, SHUT_RDWR);
		_socket_close(self);
	}
}

int socket_bind_and_listen(socket_t *self, 
						   const char *host,
						   const char *service) {
	struct addrinfo *result = NULL;
	int status = _socket_get_addrinfo(self, 
									  host, 
									  service, 
									  SERVER_SIDE_FLAGS, 
									  &result);
	if (status == SUCCESS) {
		status = _socket_addrinfo_iterate(_socket_create_and_bind, result, self);
		freeaddrinfo(result);
	}
	return _socket_listen(self, MAX_PENDING_CONNECTIONS); 
}

int socket_accept(socket_t *listener, socket_t *peer) {
	peer->file_descriptor = accept(listener->file_descriptor, NULL, NULL);
	return (peer->file_descriptor == ERROR) ? ERROR : SUCCESS;
}

int socket_connect(socket_t *self, const char *host, const char *service) {
	struct addrinfo *result = NULL;

	int status = _socket_get_addrinfo(self, 
									   host, 
									   service, 
									   CLIENT_SIDE_FLAGS, 
									   &result);
	if (status == SUCCESS) {
		status = _socket_addrinfo_iterate(_socket_create_and_connect, result, self);
		freeaddrinfo(result);
	}
	return status;
}

ssize_t socket_send(socket_t *self, const void *buffer, size_t length) {
	const char *current_buffer = buffer;
	ssize_t sent_bytes = 0;
	ssize_t sent_bytes_aux = 0;
	size_t remaining_bytes = (length - sent_bytes);

	while (remaining_bytes > 0 && sent_bytes_aux != ERROR) {
		sent_bytes_aux = send(self->file_descriptor, 
							  (current_buffer + sent_bytes), 
							  remaining_bytes, 
							  MSG_NOSIGNAL);
		sent_bytes += sent_bytes_aux;
		remaining_bytes = (length - sent_bytes);
	}
	return sent_bytes;
}

ssize_t socket_receive(socket_t *self, void *buffer, size_t length) {
	char *current_buffer = buffer;
	ssize_t received_bytes = 0;
	ssize_t received_bytes_aux = 0;
	size_t remaining_bytes = (length - received_bytes);

	while (remaining_bytes > 0 && received_bytes != ERROR) {
		received_bytes_aux = recv(self->file_descriptor, 
 							      (current_buffer + received_bytes), 
							  	  remaining_bytes, 
							  	  MSG_NOSIGNAL);
		received_bytes += received_bytes_aux;
		remaining_bytes = (length - received_bytes);
	}
	return received_bytes;
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

	int status = getaddrinfo(host, service, &hints, result);
	if (status == SUCCESS) {
		return status;
	}
	return status;
}

static int _socket_addrinfo_iterate(int (*callback)(struct addrinfo *result, 
													void *context),  
									struct addrinfo *result, 
									void *context) {
	int status = ERROR;

	while (result != NULL && status == ERROR) {
		status = callback(result, context);
		result = result->ai_next;
	}
	return status;
}

static int _socket_create_and_connect(struct addrinfo *result, void *context) {
	socket_t *socket = context;
	int status = ERROR;

	_socket_create(socket, result);
	status = _socket_connect(socket, result);
	if (status == ERROR) {
		_socket_close(socket);
	}
	return status;
}

static int _socket_create(socket_t *self, struct addrinfo *result) {
	self->file_descriptor = socket(result->ai_family, 
								   result->ai_socktype, 
								   result->ai_protocol);
	return self->file_descriptor == SUCCESS ? SUCCESS : ERROR;
}

static int _socket_connect(socket_t *self, struct addrinfo *result) {
	return connect(self->file_descriptor, 
	       		   result->ai_addr, 
				   result->ai_addrlen);
}

static int _socket_shutdown(socket_t *self, int channel) {
	int status = shutdown(self->file_descriptor, channel);
	return status == SUCCESS ? SUCCESS : ERROR;
}

static int _socket_close(socket_t *self) {
	int status = close(self->file_descriptor);
	return status == SUCCESS ? SUCCESS : ERROR;
}

static int _socket_bind(socket_t *self, struct addrinfo *result) {
	int val = 1;
	setsockopt(self->file_descriptor, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	return bind(self->file_descriptor, result->ai_addr, result->ai_addrlen);
}

static int _socket_create_and_bind(struct addrinfo *result, void *context) {
	socket_t *socket = context;
	int status = ERROR;

	_socket_create(socket, result);
	status = _socket_bind(socket, result);
	if (status == ERROR) {
		_socket_close(socket);
	}
	return status;
}

static int _socket_listen(socket_t *self, int backlog) {
	return listen(self->file_descriptor, backlog) == SUCCESS ? SUCCESS : ERROR;
}
