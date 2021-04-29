#include "common_maplib.h"

#define IN_RANGE 0
#define OUT_OF_RANGE -1
#define INVALID_CHARACTER 0xff

/**
 * @brief Funcion que mapea un caracter
 * entre 'A-Z' a '0-25' y en caso de otro caracter se mapea a 0xFF.
 * @return (NO DEBERIA RETORNAR NADA ES AL PEDO LUEGO REVISAR.)
 */
static int hill_cipher_map_char(unsigned char *character);

/**
 * @brief Funcion que desmapea el valor
 * entre '0-25' a 'A-Z' y en caso de otro valor no se toma accion
 * alguna, la funcion no puede retornar el caracter original.
 * @return (NO DEBERIA RETORNAR NADA ES AL PEDO LUEGO REVISAR.)
 */
static int hill_cipher_unmap_char(unsigned char *value);

/**
 * @brief Funcion que itera sobre todo el buffer y realiza la accion
 * que determina el callback dependiendo de cual sea.
 * @param buffer: Buffer de entrada a iterar.
 * @param length: Longitud del buffer.
 * @param callback: funcion que determina la accion sobre los 
 * caracteres del buffer.
 * @return (NO DEBERIA RETORNAR NADA ES AL PEDO LUEGO REVISAR.)
 */
static int hill_cipher_iterate(unsigned char *buffer, 
							   size_t length, 
							   int (*callback)(unsigned char* character));

int hill_cipher_map(unsigned char *buffer, size_t length) {
	return hill_cipher_iterate(buffer, length, hill_cipher_map_char);
}

int hill_cipher_unmap(unsigned char *buffer, size_t length) {
	return hill_cipher_iterate(buffer, length, hill_cipher_unmap_char);
}

int hill_cipher_is_valid_value(const unsigned char character) {
	if (character == INVALID_CHARACTER) {
		return OUT_OF_RANGE;
	}
	return IN_RANGE;
}

static int hill_cipher_map_char(unsigned char *character) {
	if (*character >= 'A' && *character <= 'Z') {
		*character = *character - 'A';
		return IN_RANGE;
	} 
	*character = INVALID_CHARACTER;
	return OUT_OF_RANGE;
}

static int hill_cipher_unmap_char(unsigned char *value) {
	if (*value >= ('A'-'A') && *value <= ('Z'-'A')) {
		*value = *value + 'A';
		return IN_RANGE;
	}
	return OUT_OF_RANGE;
}

static int hill_cipher_iterate(unsigned char *buffer, 
							   size_t length, 
							   int (*callback)(unsigned char* character)) {
	int status = IN_RANGE;
	for (int i = 0; i < length; i++) {
		status = callback(&(buffer[i]));
	}
	return status;
}
