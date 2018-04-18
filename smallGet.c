/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
#include "smallLib.h"

int main(int argc, char **argv)
{
	size_t n;
	int port, secretKey;//, *length;
	char *host, *variableName, *value;
	short dataLength;
	//    rio_t rio;

	if (argc != 5) {
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
	}
	else{
		host = argv[1];
		port = atoi(argv[2]);
		secretKey = atoi(argv[3]);
		variableName = argv[4];
		//value = argv[5];

		dataLength = (short)atoi(argv[6]);
		//char value[dataLength-1];
		//rio_t rio;
		int delta = smallGet(host, port, secretKey, variableName, value, &dataLength);
		//Rio_readinitb(&rio, delta);
		//char value[15];
		//n = Rio_readnb(&rio, value, strlen(value));
		//printf("%s\n", value);
		//int secret = atoi(argv[3]);
		/*    clientfd = Open_clientfd(host, port);
		      Rio_readinitb(&rio, clientfd);

		      while (Fgets(buf, MAXLINE, stdin) != NULL) {
		      Rio_writen(clientfd, buf, strlen(buf));
		      Rio_readlineb(&rio, buf, MAXLINE);
		      Fputs(buf, stdout);
		      }
		      Close(clientfd); //line:netp:echoclient:close
		 */

		if (delta < 0)
		{

			printf("error\n");

		}
		//printf("server = %s\n", buf);
		exit(0);
	}
}
/* $end echoclientmain */

