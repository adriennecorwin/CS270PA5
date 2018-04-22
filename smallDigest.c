#include "csapp.h"
#include "smallLib.h"

int main(int argc, char **argv)
{
	int port, SecretKey;
	short dataLength, resultLength;
	char *host, *data, result[MAXVARVAL];

	if (argc != 5) {
		fprintf(stderr, "usage: %s <host> <port> <value>\n", argv[0]);
		exit(0);
	}
	else{
		host = argv[1];
		port = atoi(argv[2]);
		SecretKey = atoi(argv[3]);
		data = argv[4];
		dataLength = strlen(data);
		int status = smallDigest(host, port, SecretKey, data, dataLength, result, &resultLength);
		if(status==0){
			printf("%s\n", result);
		}
		else{
			printf("failed\n");
		}
		exit(0);
	}
}


