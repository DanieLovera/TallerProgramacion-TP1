#ifndef _HILL_CIPHER_
#define _HILL_CIPHER_

#include <stddef.h>
#include <sys/types.h>

typedef struct hill_cipher {
	unsigned char *_key;
	size_t _key_length;
	unsigned char *_result;
	size_t _result_length;
} hill_cipher_t;

/**
 * @brief Inicializa el cipher.
 * @param key: Clave que se utiliza en el cifrado.
 * @param key_length: Longitud de la clave.
 */
void hill_cipher_init(hill_cipher_t *self, 
					  const unsigned char *key, 
					  size_t key_length);

/**
 * @brief Libera los recursos utilizados por el cipher.
 */
void hill_cipher_uninit(hill_cipher_t *self);

/**
 * @brief Encripta el mensaje del buffer y se guarda 
 * el resultado en el estado.
 * @param buffer: Valores a encriptar.
 * @param Longitud de los valores a encriptar.
 * @return: Devuelve 0 en caso de exito y -1 en caso de error.
 */
int hill_cipher_encode(hill_cipher_t *self, 
					   const unsigned char *buffer, 
					   size_t length);

ssize_t hill_cipher_send_result(hill_cipher_t *self, 
								ssize_t (*callback)(void *context, 
										 			unsigned char *buffer, 
						   							ssize_t size), 
								void *context);

#endif
