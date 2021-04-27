#ifndef _MAPLIB_
#define _MAPLIB_

#include <stddef.h>

int hill_cipher_map(unsigned char *buffer, size_t length);
int hill_cipher_unmap(unsigned char *buffer, size_t length);
int hill_cipher_is_valid_value(const unsigned char character);

#endif
