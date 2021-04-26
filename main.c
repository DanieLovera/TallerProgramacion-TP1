#include "socket.h"
#include "hill_cipher.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
	hill_cipher_t hill_cipher;
	char *cadena = "CDIB";

	hill_cipher_init(&hill_cipher, cadena, strlen(cadena));
	do_something(&hill_cipher);
	/*char character = 0;
	scanf("%c", &character);
	while (character != '0') {
		do_something(&character);
		scanf(" %c", &character);
	}*/
	hill_cipher_uninit(&hill_cipher);

	return 0;	
}
