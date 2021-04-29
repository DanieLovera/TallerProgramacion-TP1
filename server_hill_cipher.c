#include "server_hill_cipher.h"
#include "common_maplib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SUCCESS 0
#define ERROR -1

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
 * @brief Inicializa el estado resultado del cipher. Si la longitud
 * length es mayor a la del buffer el comportamiento sera indefinido.
 * @param buffer: Guarda el contenido a encriptar.
 * @param length: Guarda el tamanio del buffer a encriptar
 */
static void _hill_cipher_init_result(hill_cipher_t *self, 
									 const unsigned char *buffer, 
									 size_t length);

/**
 * @brief Calcula la dimension que usa el algoritmo para encriptar y 
 * la devuelve en el parametro dimension.
 * @param dimension: Almacena la dimension del algoritmo.
 */
static void _hill_cipher_result_dimension(hill_cipher_t *self, 
										  size_t *dimension);

/**
 * @brief Calcula la nueva longitud del resultado en caso de que
 * no sea multiplo de la dimension de la llave.
 * @param dimension: Almacena la dimension del algoritmo.
 * @param length: Guarda y modifica la longitud del nuevo resultado.
 */
static void _hill_cipher_new_length(hill_cipher_t *self, 
									size_t dimension, 
									size_t *length);

/**
 * @brief Filtra los caracteres que no esten contenidos entre [A-Z], 
 * modifica el contenido del buffer por el filtrado.
 * @param buffer: Guarda el los caracteres que se analizaran para el
 * filtrado.
 * @param length: Guarda la longitud del buffer.
 */
static void _hill_cipher_filter(unsigned char *buffer, size_t *length);

/**
 * @brief Coloca second_char en first char y * viceversa.
 * @param first_char: Contiene el primer caracter.
 * @param second_char: Contiene el segundo caracter.
 */
static void _hill_cipher_swap(unsigned char *first_char, 
							  unsigned char *second_char);

/**
 * @brief Aplica los pasos del encriptado pos mapeo y los almacena
 * een su estado.
 * @return: Devuelve 0 en caso de exito o -1 en caso contrario.
 */
static int _hill_cipher_encode(hill_cipher_t *self);

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
 * @brief Reserva memoria y almacena en variable buff
 * @param nelem: numero de elementos a reservar.
 * @param elsize: tamanio de los elementos a reservar.
 * @return Devuelve 0 en caso de exito o -1 en caso de error.
 */
static int _hill_cipher_calloc(unsigned char **buff, 
							   size_t nelem, 
							   size_t elsize);

/**
 * @brief Libera memoria del buffer.
 */
static void _hill_cipher_free(unsigned char *buff);

void hill_cipher_init(hill_cipher_t *self, 
					  const unsigned char *key, 
					  size_t key_length) {
	_hill_cipher_init_key(self, key, key_length);
	_hill_cipher_init_result(self, NULL, 0);
}

void hill_cipher_uninit(hill_cipher_t *self) {
	_hill_cipher_free(self->_key);
	_hill_cipher_free(self->_result);
}

int hill_cipher_encode(hill_cipher_t *self, 
						const unsigned char *buffer, 
						size_t length) {
	_hill_cipher_init_result(self, buffer, length);
	_hill_cipher_filter(self->_result, &(self->_result_length));
	return _hill_cipher_encode(self);
}

ssize_t hill_cipher_send_result(hill_cipher_t *self, 
								ssize_t (*callback)(void *context, 
										 			unsigned char *buffer, 
						   							ssize_t size), 
								void *context) {
	return callback(context, self->_result, self->_result_length);
}

static void _hill_cipher_init_key(hill_cipher_t *self, 
								  const unsigned char *key, 
								  size_t length) {
	if (key != NULL && length > 0) {
		_hill_cipher_calloc(&(self->_key), length, sizeof(char));
		self->_key_length = length;
		memcpy(self->_key, key, self->_key_length);
	} else {
		self->_key = NULL;
		self->_key_length = 0;
	}
	hill_cipher_map(self->_key, self->_key_length);
}

static void _hill_cipher_init_result(hill_cipher_t *self, 
									 const unsigned char *buffer, 
									 size_t length) {	
	if (buffer != NULL && length > 0) {
		_hill_cipher_free(self->_result);

		if (_hill_cipher_calloc(&(self->_result), length, sizeof(char)) != ERROR) {
			self->_result_length = length;
			memcpy(self->_result, buffer, self->_result_length);
		}
	} else {
		self->_result = NULL;
		self->_result_length = 0;
	}
	hill_cipher_map(self->_result, self->_result_length);
}

static void _hill_cipher_result_dimension(hill_cipher_t *self, 
										  size_t *dimension) {
	*dimension = sqrt(self->_key_length);
}

static void _hill_cipher_new_length(hill_cipher_t *self, 
									size_t dimension, 
									size_t *length) {
	size_t remainder = (self->_result_length % (dimension));
	if (remainder != 0) {
		*length = ((self->_result_length / (dimension)) + 1) * (dimension);		
	} else {
		*length = self->_result_length;
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

static int _hill_cipher_encode(hill_cipher_t *self) {
	size_t dimension = 0;
	size_t result_length = 0;
	int status = 0;
	unsigned char *buffer = NULL;

	_hill_cipher_result_dimension(self, &dimension);
	_hill_cipher_new_length(self, dimension, &result_length);

	status = _hill_cipher_calloc(&buffer, result_length, sizeof(char));
	if(status != ERROR) {	
		memcpy(buffer, self->_result, self->_result_length);
		_hill_cipher_free(self->_result);
		
		status = _hill_cipher_calloc(&(self->_result), result_length, sizeof(char));
		if(status != ERROR) {
			self->_result_length = result_length;
			_hill_cipher_encode_math_ops(self, buffer, dimension);
		}
	}
	_hill_cipher_free(buffer);
	return status;
}

static void _hill_cipher_encode_math_ops(hill_cipher_t *self, 
										 unsigned char *chunk, 
										 size_t dimension) {
	int result_iterations = (self->_result_length) / dimension; 
	int key_iterations = (self->_key_length) / dimension;

	for (int i = 0; i < result_iterations; i++) {
		for (int j = 0; j < key_iterations; j++) {
			for (int k = 0; k < key_iterations; k++) {
				self->_result[j + i*dimension] += (self->_key[k + j*dimension] * 
										 		   chunk[k + i*dimension]);
			}
			self->_result[j + i*dimension] = (self->_result[j + i*dimension]) % 26;
		}
	}
}

static int _hill_cipher_calloc(unsigned char **buff, 
							   size_t nelem, 
							   size_t elsize) {
	int status = ERROR;
	if (nelem != 0 && elsize != 0) {
		status = SUCCESS;
		*buff = calloc(nelem, elsize);
		if (*buff == NULL){
			fprintf(stderr, "%s\n", strerror(0)); 
			status = ERROR;
		}
	}
	return status;
}

static void _hill_cipher_free(unsigned char *buff) {
	if (buff != NULL) {
		free(buff);
	}
}
