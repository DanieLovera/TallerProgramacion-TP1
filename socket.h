#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/types.h>
#include <stddef.h>

typedef struct socket {
	int file_descriptor;
} socket_t;

/**
 * @brief Inicializa un socket.
 */
void socket_init(socket_t *self);

/**
 * @brief Libera los recursos del socket.
 */
void socket_uninit(socket_t *self);

/**
 * @brief Enlaza y coloca un socket en estado pasivo.
 * @param host: Dominio de la maquina.
 * @param service: Puerto de la maquina en el que se ubicara un socket.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
int socket_bind_and_listen(socket_t *self, 
							const char *host,
							const char *service);

/**
 * @brief Acepta conexiones entrantes de un cliente al servidor.
 * @param listener: Socket que gestiona las conexiones entrantes.
 * @param peer: Socket aceptado.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
int socket_accept(socket_t *listener, socket_t *peer);

/**
 * @brief Conecta un socket a la direccion del host y servicio.
 * @param host: Dominio de la maquina.
 * @param service: Puerto de la maquina en el que se ubicara un socket.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
int socket_connect(socket_t *self, const char *host, const char *service);

/**
 * @brief Envia un stream de datos a traves del socket
 * @param buffer: Puntero generico a strem de datos.
 * @param length: Longitud del buffer a enviar.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
ssize_t socket_send(socket_t *self, const void *buffer, size_t length);

/**
 * @brief Recibe stream de datos a traves del socket.
 * @param buffer: Puntero generico a strem de datos.
 * @param length: Longitud del buffer a enviar.
 * @returns Devuelve un 0 en caso de exito o -1 en caso de error.
 */
ssize_t socket_receive(socket_t *self, void *buffer, size_t length);

#endif
