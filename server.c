#include "common_socket.h"
#include "server_protocol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define END -1

int main(int argc, const char *argv[]) {

	server_protocol_t server_protocol;
	server_protocol_init(&server_protocol);
	server_protocol_run(&server_protocol, argv[1], (const unsigned char*)"CDIB", 4);
	server_protocol_uninit(&server_protocol);

	return 0;	
}