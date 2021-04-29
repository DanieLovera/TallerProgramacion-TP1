#include "common_socket.h"
#include "client_protocol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR -1
#define HOST argv[1]
#define SERVICE argv[2]
#define FILE_PATH argv[3]

int main(int argc, const char *argv[]) {
	const char *file_path = FILE_PATH;
	int status = ERROR;
	FILE* file = (strcmp(file_path, "-") == SUCCESS) ? stdin : fopen(argv[3], "r");
	client_protocol_t client_protocol;
	
	client_protocol_init(&client_protocol);
	if (argc == 4) {
		status = SUCCESS;
		client_protocol_run(&client_protocol, HOST, SERVICE, file);
	} else {
		fprintf(stderr, "Formato invalido, debe ser de la forma: ./client host service filename\n");
	}
	client_protocol_uninit(&client_protocol);
	
	if (file != stdin) {fclose(file);}

	return status;	
}
