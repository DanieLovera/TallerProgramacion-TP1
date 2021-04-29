#include "common_maplib.h"

#define IN_RANGE 0
#define OUT_OF_RANGE -1
#define OUT_OF_RANGE_CHAR 0xFF
#define MIN_CHAR 'A'
#define MAX_CHAR 'Z'
#define MIN_VALUE (MIN_CHAR - MIN_CHAR)
#define MAX_VALUE (MAX_CHAR - MIN_CHAR)

/**
 * @brief Funcion que mapea un caracter
 * entre 'A-Z' a '0-25' y en caso de otro caracter se mapea a 0xFF.
 */
static void hill_cipher_map_char(unsigned char *character);

/**
 * @brief Funcion que desmapea el valor
 * entre '0-25' a 'A-Z' y en caso de otro valor no se toma accion
 * alguna, la funcion no puede retornar el caracter original.
 */
static void hill_cipher_unmap_char(unsigned char *value);

/**
 * @brief Funcion que itera sobre todo el buffer y realiza la accion
 * que determina el callback dependiendo de cual sea.
 * @param buffer: Buffer de entrada a iterar.
 * @param length: Longitud del buffer.
 * @param callback: funcion que determina la accion sobre los 
 * caracteres del buffer.
 */
static void hill_cipher_iterate(unsigned char *buffer, 
							   size_t length, 
							   void (*callback)(unsigned char* character));

void hill_cipher_map(unsigned char *buffer, size_t length) {
	hill_cipher_iterate(buffer, length, hill_cipher_map_char);
}

void hill_cipher_unmap(unsigned char *buffer, size_t length) {
	hill_cipher_iterate(buffer, length, hill_cipher_unmap_char);
}

int hill_cipher_is_valid_value(const unsigned char character) {
	if (character == OUT_OF_RANGE_CHAR || 
		(character > MAX_VALUE || character < MIN_VALUE)) {
		return OUT_OF_RANGE;
	}
	return IN_RANGE;
}

static void hill_cipher_map_char(unsigned char *character) {
	if (*character >= MIN_CHAR && *character <= MAX_CHAR) {
		*character = *character - MIN_CHAR;
	} else {
		*character = OUT_OF_RANGE_CHAR;
	}
}

static void hill_cipher_unmap_char(unsigned char *value) {
	if (*value >= (MIN_CHAR - MIN_CHAR) && *value <= (MAX_CHAR - MIN_CHAR)) {
		*value = *value + MIN_CHAR;
	}
}

static void hill_cipher_iterate(unsigned char *buffer, 
							   	size_t length, 
							   	void (*callback)(unsigned char* character)) {
	for (int i = 0; i < length; i++) {
		callback(&(buffer[i]));
	}
}
