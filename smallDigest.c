#include "csapp.h"
#include "smallLib.h"

int main(int argc, char **argv)
{
	int port, SecretKey;
	short dataLength, resultLength;
	char *host, *data, result[MAXVARVAL];

	if (argc != 5) {//if user does not enter 5 inputs
		fprintf(stderr, "usage: %s <host> <port> <value>\n", argv[0]);
		exit(0);
	}
	else{
		host = argv[1];//stores machine name
		port = atoi(argv[2]);//stores the port being used
		SecretKey = atoi(argv[3]);//stores the secret key
		data = argv[4];//stores the data being digested
		dataLength = strlen(data);//stores length of data that the user enters
		int status = smallDigest(host, port, SecretKey, data, dataLength, result, &resultLength);//calls function smallDigest
		if(status==0){//if success, print result
			printf("%s\n", result);
		}
		else{//otherwise print failed
			printf("failed\n");
		}
		exit(0);
	}
}


