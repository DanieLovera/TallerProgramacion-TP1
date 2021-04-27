#ifndef _HILL_CIPHER_
#define _HILL_CIPHER_

#include <stddef.h>

typedef struct hill_cipher {
	unsigned char *key;
	size_t key_length;
	unsigned char *result;
	size_t result_length;
} hill_cipher_t;

void hill_cipher_init(hill_cipher_t *self, 
					  const unsigned char *key, 
					  size_t key_length);
void hill_cipher_encode(hill_cipher_t *self, 
						const unsigned char *buffer, 
						size_t length);
void do_something(hill_cipher_t *self);
void hill_cipher_uninit(hill_cipher_t *self);

#endif
