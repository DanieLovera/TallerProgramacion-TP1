#include "hill_cipher.h"
#include "maplib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SUCCESS 0
#define ERROR -1

static void _hill_cipher_init_result(hill_cipher_t *self, 
									 const unsigned char *buffer, 
									size_t length);
static void _hill_cipher_init_key(hill_cipher_t *self, 
								  const unsigned char* key, 
								  size_t length);
static void _hill_cipher_free_result(hill_cipher_t *self);
static void _hill_cipher_free_key(hill_cipher_t *self);
static void _hill_cipher_filter(unsigned char *buffer, size_t *length);
static void _hill_cipher_key_dimension(hill_cipher_t *self, size_t *dimension);
static void _hill_cipher_new_length(hill_cipher_t *self, 
									size_t dimension, 
									size_t *length);
static void _hill_cipher_encode_math_ops(hill_cipher_t *self, 
										 unsigned char *chunk, 
										 size_t dimension);
static void _hill_cipher_encode(hill_cipher_t *self);
static void _hill_cipher_swap(unsigned char *first_char, 
							  unsigned char *second_char);

/*void do_something(hill_cipher_t *self) {
	char strin[] = "ZXY";
	hill_cipher_encode(self, (unsigned char*)strin, strlen(strin));
	fwrite(self->key, 1, self->key_length, stdout);
	printf("\n");
	fwrite(self->result, 1, self->result_length, stdout);
	printf("\n");
}*/

void hill_cipher_encode(hill_cipher_t *self, 
						const unsigned char *buffer, 
						size_t length) {
	_hill_cipher_init_result(self, buffer, length);
	hill_cipher_map(self->key, self->key_length);
	hill_cipher_map(self->result, self->result_length);
	_hill_cipher_filter(self->result, &(self->result_length));
	_hill_cipher_encode(self);
	hill_cipher_unmap(self->key, self->key_length);
	hill_cipher_unmap(self->result, self->result_length);
}


void hill_cipher_init(hill_cipher_t *self, 
					  const unsigned char *key, 
					  size_t key_length) {
	_hill_cipher_init_key(self, key, key_length);
	_hill_cipher_init_result(self, NULL, 0);
}

void hill_cipher_uninit(hill_cipher_t *self) {
	_hill_cipher_free_key(self);
	_hill_cipher_free_result(self);
}

static void _hill_cipher_init_result(hill_cipher_t *self, 
									 const unsigned char *buffer, 
									 size_t length) {	
	if (buffer != NULL && length > 0) {
		self->result = calloc(length, sizeof(char));
		self->result_length = length;
		memcpy(self->result, buffer, self->result_length);
	} else {
		self->result = NULL;
		self->result_length = 0;
	}
}

static void _hill_cipher_init_key(hill_cipher_t *self, 
								  const unsigned char* key, 
								  size_t length) {
	if (key != NULL && length > 0) {
		self->key = calloc(length, sizeof(char));
		self->key_length = length;
		memcpy(self->key, key, self->key_length);
	} else {
		self->key = NULL;
		self->key_length = 0;
	}
}

static void _hill_cipher_free_key(hill_cipher_t *self) {
	if (self->key != NULL) {
		free(self->key);
	}	
}

static void _hill_cipher_free_result(hill_cipher_t *self) {
	if (self->result != NULL) {
		free(self->result);
	}
}


static void _hill_cipher_encode(hill_cipher_t *self) {
	size_t dimension = 0;
	size_t result_length = 0;

	_hill_cipher_key_dimension(self, &dimension);
	_hill_cipher_new_length(self, dimension, &result_length);

	unsigned char *aux = calloc(result_length, sizeof(char));
	memcpy(aux, self->result, self->result_length);
	free(self->result);
	self->result = calloc(result_length, sizeof(char)); 
	self->result_length = result_length;
	
	_hill_cipher_encode_math_ops(self, aux, dimension);
	free(aux);
}


static void _hill_cipher_key_dimension(hill_cipher_t *self, size_t *dimension) {
	*dimension = sqrt(self->key_length);
}

static void _hill_cipher_new_length(hill_cipher_t *self, 
									size_t dimension, 
									size_t *length) {
	size_t value = (self->result_length % (dimension));
	if (value != 0) {
		*length = ((self->result_length / (dimension)) + 1) * (dimension);		
	}
}

static void _hill_cipher_encode_math_ops(hill_cipher_t *self, 
										 unsigned char *chunk, 
										 size_t dimension) {
	int result_iterations = (self->result_length) / dimension; 
	int key_iterations = (self->key_length) / dimension;

	for (int i = 0; i < result_iterations; i++) {
		for (int j = 0; j < key_iterations; j++) {
			for (int k = 0; k < key_iterations; k++) {
				self->result[j + i*dimension] += (self->key[k + j*dimension] * 
										 		  chunk[k + i*dimension]);
			}
			self->result[j + i*dimension] = (self->result[j + i*dimension]) % 26;
		}
	}
}

static void _hill_cipher_filter(unsigned char *buffer, size_t *length) {
	for (int i = 0; i < *length; i++) {
		if (hill_cipher_is_valid_value(buffer[i]) == ERROR) {
			for (int j = i; j < *length - 1; j++) {
				_hill_cipher_swap(&(buffer[j]), &(buffer[j+1]));
			}
			*length = *length - 1;
			i = i - 1;
		}
	}
}

static void _hill_cipher_swap(unsigned char *first_char, 
							  unsigned char *second_char) {
	unsigned char aux_char = *first_char;
	*first_char = *second_char;
	*second_char = aux_char;
}
