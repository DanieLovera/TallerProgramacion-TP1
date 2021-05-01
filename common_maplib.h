#ifndef _MAPLIB_
#define _MAPLIB_

#include <stddef.h>

/**
 * @brief Funcion que mapea los caracteres del buffer
 * entre 'A-Z' a '0-25' y en caso de otro caracter se mapea a 0xFF.
 * @param length: Longitud del buffer.
 */
void hill_cipher_map(unsigned char *buffer, size_t length);

/**
 * @brief Funcion que desmapea los caracteres del buffer
 * entre '0-25' a 'A-Z' y en caso de otro caracter no se toma accion
 * alguna, la funcion no puede retornar el caracter original.
 * @param lenght: Longitud del buffer.
 */
void hill_cipher_unmap(unsigned char *buffer, size_t length);

/**
 * @brief Valida el caracter de entrada
 * @param character: valor a validar.
 * @return Devuelve 0 en caso de que el caracter pertenezca a 'A-Z' 
 * o -1 (out of range) en caso contrario.
 */
int hill_cipher_is_valid_value(unsigned char character);

#endif
