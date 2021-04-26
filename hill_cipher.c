#include "hill_cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SUCCESS 0
#define ERROR -1
#define INVALID_CHARACTER -1

static void hill_cipher_map_char(unsigned char *character) {
	if (*character >= 'A' && *character <= 'Z') {
		*character = *character - 'A';
	} else {
		*character = INVALID_CHARACTER;
	}
}

static int hill_cipher_unmap_char(unsigned char *value) {
	if (*value >= ('A'-'A') && *value <= ('Z'-'A')) {
		*value = *value + 'A';
		return SUCCESS;
	}
	return ERROR;
}

size_t hill_cipher_key_dimension(hill_cipher_t *self) {
	return sqrt(self->key_length);
}

void hill_cipher_init_wrapper(hill_cipher_t *self, const unsigned char *buffer, size_t length) {	
	self->result = malloc(length);
	self->result_length = length;
	memcpy(self->result, buffer, self->result_length);
}

void hill_cipher_map(unsigned char *buffer, size_t length) {
	for (int i = 0; i < length; i++) {
		hill_cipher_map_char(&(buffer[i]));
	}
}

void hill_cipher_unmap(unsigned char *buffer, size_t length) {
	for (int i = 0; i < length; i++) {
		hill_cipher_unmap_char(&(buffer[i]));
	}
}

void hill_cipher_swap(unsigned char *first_char, unsigned char *second_char) {
	unsigned char aux_char = *first_char;
	*first_char = *second_char;
	*second_char = aux_char;
}

void hill_cipher_filter(unsigned char *buffer, size_t *length) {
	for (int i = 0; i < *length; i++) {
		if (buffer[i] == INVALID_CHARACTER) {
			for (int j = i; j < *length - 1; j++) {
				hill_cipher_swap(&(buffer[j]), &(buffer[j+1]));
			}
			*length = *length - 1;
			i = i - 1;
		}
	}
}

int hill_cipher_new_length(hill_cipher_t *self, int dimension) {
	int value = (self->result_length % dimension);
	if (value != 0) {
		return ((self->result_length / dimension) + 1) * dimension;		
	}
	return self->result_length;
}

void hill_cipher_encode_op(hill_cipher_t *self) {
	int dimension = hill_cipher_key_dimension(self);
	int result_length = hill_cipher_new_length(self, dimension);


	unsigned char *aux = calloc(result_length, sizeof(char));
	memcpy(aux, self->result, self->result_length);
	free(self->result);
	self->result = calloc(result_length, 1); //antes malloc
	self->result_length = result_length;
	
	int result_iterations = (self->result_length) / dimension; 
	int key_iterations = (self->key_length) / dimension;

	for (int i = 0; i < result_iterations; i++) {
		for (int j = 0; j < key_iterations; j++) {
			for (int k = 0; k < key_iterations; k++) {
				self->result[j + i*dimension] += (self->key[k + j*dimension] * 
										 		  aux[k + i*dimension]);
			}
			self->result[j + i*dimension] = (self->result[j + i*dimension]) % 26;
		}
	}
	free(aux);
}

void hill_cipher_encode(hill_cipher_t *self, const unsigned char *buffer, size_t length) {
	hill_cipher_init_wrapper(self, buffer, length);
	hill_cipher_map(self->key, self->key_length);
	hill_cipher_map(self->result, self->result_length);
	hill_cipher_filter(self->result, &(self->result_length));
	hill_cipher_encode_op(self);
	hill_cipher_unmap(self->key, self->key_length);
	hill_cipher_unmap(self->result, self->result_length);
}

void hill_cipher_init(hill_cipher_t *self, const unsigned char *key, size_t key_length) {
	self->key = malloc(key_length);
	self->key_length = key_length;
	memcpy(self->key, key, self->key_length);
	self->result = NULL;
}

void do_something(hill_cipher_t *self) {
	char strin[] = "ZXY";
	hill_cipher_encode(self, (unsigned char*)strin, strlen(strin));
	fwrite(self->key, 1, self->key_length, stdout);
	printf("\n");
	fwrite(self->result, 1, self->result_length, stdout);
	printf("\n");
}

void hill_cipher_free_key(hill_cipher_t *self) {
	if (self->key != NULL) {
		free(self->key);
	}	
}

void hill_cipher_free_result(hill_cipher_t *self) {
	if (self->result != NULL) {
		free(self->result);
	}
}

void hill_cipher_uninit(hill_cipher_t *self) {
	hill_cipher_free_key(self);
	hill_cipher_free_result(self);
}


