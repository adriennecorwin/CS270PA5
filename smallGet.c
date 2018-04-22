#include "csapp.h"
#include "smallLib.h"

int main(int argc, char **argv)
{
	int port, secretKey;
	char *host, variableName[MAXVARNAME], value[MAXVARVAL];
	short resultLength=0;

	if (argc != 5) {
		fprintf(stderr, "usage: %s <host> <port> <secretKey> <variableName>\n", argv[0]);
		exit(0);
	}
	else{
		host = argv[1]; //get request information to send to server
		port = atoi(argv[2]);
		secretKey = atoi(argv[3]);
		strncpy(variableName, argv[4], strlen(argv[4]));
		int status = smallGet(host, port, secretKey, variableName, value, &resultLength); //send information to server
		if(status==-1){ //request failed 
			printf("failed\n");
		}
		else if(status==0){ //request was successful
			printf("%s\n", value); //print out the value of the variable requested
		}
		exit(0);
	}
}

