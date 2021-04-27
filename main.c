#include "socket.h"
#include "hill_cipher.h"
#include "client_protocol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_PATH argv[3]
#define END -1

int main(int argc, const char *argv[]) {
	
	const char *file_path = FILE_PATH;
	int cmp_status = strcmp(file_path, "-");
	FILE* file = (cmp_status == 0) ? stdin : fopen(argv[3], "r");

	client_protocol_t client_protocol;
	client_protocol_init(&client_protocol, argv[1], argv[2]);
	client_protocol_run(&client_protocol, file);
	client_protocol_uninit(&client_protocol);
	
	if (file != stdin) {fclose(file);}

	return 0;	
}
