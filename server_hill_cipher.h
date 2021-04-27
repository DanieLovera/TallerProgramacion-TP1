#ifndef _HILL_CIPHER_
#define _HILL_CIPHER_

#include <stddef.h>

typedef struct hill_cipher {
	unsigned char *key;
	size_t key_length;
	unsigned char *result;
	size_t result_length;
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
 * @brief Encripta el mensaje del buffer y se guarda 
 * el resultado en el estado.
 * @param buffer: Valores a encriptar.
 * @param Longitud de los valores a encriptar.
 */
void hill_cipher_encode(hill_cipher_t *self, 
						const unsigned char *buffer, 
						size_t length);

/**
 * @brief Libera los recursos utilizados por el cipher.
 */
void hill_cipher_uninit(hill_cipher_t *self);


void do_something(hill_cipher_t *self);

#endif
