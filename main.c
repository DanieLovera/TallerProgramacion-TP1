#include "socket.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	socket_t socket;
	socket_init(&socket);
	int err_code = socket_connect(&socket, "localhost", "7777");
	char buff[50];
	int i = scanf("%s", buff);
	while(strcmp(buff,"0") != 0) {
		socket_send(&socket, buff, strlen(buff));
		i = scanf("%s", buff);
	}
	i += i;
	socket_uninit(&socket);
	return err_code;	
}
