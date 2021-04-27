#include "common_maplib.h"

#define SUCCESS 0
#define ERROR -1
#define INVALID_CHARACTER 0xff

static int hill_cipher_map_char(unsigned char *character);
static int hill_cipher_unmap_char(unsigned char *value);
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
		return ERROR;
	}
	return SUCCESS;
}

static int hill_cipher_iterate(unsigned char *buffer, 
							   size_t length, 
							   int (*callback)(unsigned char* character)) {
	int status = SUCCESS;
	for (int i = 0; i < length; i++) {
		status = callback(&(buffer[i]));
	}
	return status;
}

static int hill_cipher_map_char(unsigned char *character) {
	if (*character >= 'A' && *character <= 'Z') {
		*character = *character - 'A';
		return SUCCESS;
	} 
	*character = INVALID_CHARACTER;
	return ERROR;
}

static int hill_cipher_unmap_char(unsigned char *value) {
	if (*value >= ('A'-'A') && *value <= ('Z'-'A')) {
		*value = *value + 'A';
		return SUCCESS;
	}
	return ERROR;
}
