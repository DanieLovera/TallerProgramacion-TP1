#include "server_hill_cipher.h"
#include "common_maplib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SUCCESS 0
#define ERROR -1

/**
 * @brief Inicializa el estado resultado del cipher. Si la longitud
 * length es mayor a la del buffer el comportamiento sera indefinido.
 * @param buffer: Guarda el contenido a encriptar.
 * @param length: Guarda el tamanio del buffer a encriptar
 */
static void _hill_cipher_init_result(hill_cipher_t *self, 
									 const unsigned char *buffer, 
									 size_t length);

/**
 * @brief Inicializa el estado para la key del cipher. Si la longitud
 * length es mayor a la del buffer el comportamiento sera indefinido.
 * @param key: Guarda la key para el encriptado del algoritmo.
 * @param length: Contiene el tamanio del buffer a encriptar
 */
static void _hill_cipher_init_key(hill_cipher_t *self, 
								  const unsigned char *key, 
								  size_t length);

/**
 * @brief Libera los recursos utilizados por el resultado del cipher.
 */
static void _hill_cipher_free_result(hill_cipher_t *self);

/**
 * @brief Libera los recursos utilizados por la key del cipher.
 */
static void _hill_cipher_free_key(hill_cipher_t *self);

/**
 * @brief Filtra los caracteres que no esten contenidos entre [A-Z], 
 * modifica el contenido del buffer por el filtrado.
 * @param buffer: Guarda el los caracteres que se analizaran para el
 * filtrado.
 * @param length: Guarda la longitud del buffer.
 */
static void _hill_cipher_filter(unsigned char *buffer, size_t *length);

/**
 * @brief Calcula la dimension que usa el algoritmo para encriptar y 
 * la devuelve en el parametro dimension.
 * @param dimension: Almacena la dimension del algoritmo.
 */
static void _hill_cipher_key_dimension(hill_cipher_t *self, size_t *dimension);

/**
 * @brief Calcula la nueva longitud del resultado si el filtrado
 * disminuyo el tamanio de este y la devuelve en el parametro length.
 * @param dimension: Almacena la dimension del algoritmo.
 * @param length: Guarda la longitud del nuevo resultado.
 */
static void _hill_cipher_new_length(hill_cipher_t *self, 
									size_t dimension, 
									size_t *length);

/**
 * @brief Calcula las operaciones matematicas necesarias para
 * el encriptado y modifica el resultado en el estado de self
 * @param chunk: Contiene el buffer que se utiliza para
 * operar con la key.
 * @param dimension: Contiene la dimension de la key del encriptado.
 */
static void _hill_cipher_encode_math_ops(hill_cipher_t *self, 
										 unsigned char *chunk, 
										 size_t dimension);

/**
 * @brief Aplica los pasos del encriptado pos mapeo y los almacena
 * en el resultado de self.
 */
static void _hill_cipher_encode(hill_cipher_t *self);

/**
 * @brief Coloca second_char en first char y * viceversa.
 * @param first_char: Contiene el primer caracter.
 * @param second_char: Contiene el segundo caracter.
 */
static void _hill_cipher_swap(unsigned char *first_char, 
							  unsigned char *second_char);

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
