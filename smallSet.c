#include "csapp.h"
#include "smallLib.h"

int main(int argc, char **argv) 
{
	int port, SecretKey, length = 0;
	char *host, varName[MAXVARNAME], value[MAXVARVAL];

	if (argc != 6) {
		fprintf(stderr, "usage: %s <host> <port> <secretKey> <variableName> <variableValue>\n", argv[0]);
		exit(0);
	}
	else{
		host = argv[1]; //get request information from user input
		port = atoi(argv[2]);
		SecretKey = atoi(argv[3]);
		strncpy(varName, argv[4], strlen(argv[4]));
		strncpy(value, argv[5], strlen(argv[5]));
		length=strlen(value);

		int status = smallSet(host, port, SecretKey, varName, value, length); //send request information to server

		if (status==-1){ //fail status from request
			printf("failed\n");

		}
		exit(0);
	}
}

