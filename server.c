#include "common_socket.h"
#include "server_protocol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR -1
#define SERVICE argv[1]
#define KEY argv[2]

int main(int argc, const char *argv[]) {
	int status = ERROR;
	server_protocol_t server_protocol;
	
	server_protocol_init(&server_protocol);
	if (argc == 3) {
		server_protocol_run(&server_protocol, 
							SERVICE, 
							(const unsigned char*)KEY, 
							strlen(KEY));
		status = SUCCESS;
	} else {
		fprintf(stderr, "Formato necesario: ./server service key\n");
	}
	server_protocol_uninit(&server_protocol);

	return status;	
}
