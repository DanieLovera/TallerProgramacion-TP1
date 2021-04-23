#include "socket.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	socket_t socket;
	socket_init(&socket);
	int err_code = socket_connect(&socket, "localhost", "7777");
	printf("%d\n", err_code);
	socket_uninit(&socket);
	return err_code;	
}
