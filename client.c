/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"


int open_clientfd(char *hostname, int port) {
	int toserverfd;
	struct hostent *serverHostEntry;
	struct sockaddr_in serveraddr;
	if ((toserverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return -1; /* check errno */
		// AF_INET: internet; SOCK_STREAM: reliable; 0: normal
	}
	/* Fill in the server.s IP address and port */
	if ((serverHostEntry = gethostbyname(hostname)) == NULL)
		return -2; /* check h_errno for cause of error */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)serverHostEntry->h_addr_list[0],
			(char *)&serveraddr.sin_addr.s_addr,
			serverHostEntry->h_length);
	serveraddr.sin_port = htons(port);
	/* Establish a connection with the server */
	if (connect(toserverfd, (SA *) &serveraddr,
				sizeof(serveraddr)) < 0) {
		return -1;
	}
	return toserverfd;
} // open_clientfd








int main(int argc, char **argv) 
{
	int clientfd, port;
	char *host, buf[MAXLINE];
	rio_t rio;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	port = atoi(argv[2]);

	clientfd = Open_clientfd(host, port);
	Rio_readinitb(&rio, clientfd);

	while (Fgets(buf, MAXLINE, stdin) != NULL) {
		Rio_writen(clientfd, buf, strlen(buf));
		Rio_readlineb(&rio, buf, MAXLINE);
		Fputs(buf, stdout);
	}
	Close(clientfd); //line:netp:echoclient:close
	exit(0);
}
/* $end echoclientmain */
