/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

int open_listenfd(int port) {
	int listenfd, optval=1;
	struct sockaddr_in serveraddr;
	/*
	Create a socket descriptor
	*/
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	// AF_INET: internet; SOCK_STREAM: reliable; 0: normal
	/*
	Allow reuse of address
	*/
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,(const void*)&optval ,sizeof(int)) < 0)
		return-1;
	/*
	let listenfd be an endpoint for all requests
	to port on any IP address for this host
	*/
	bzero((char*) &serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);
	if(bind(listenfd,(SA*)&serveraddr, sizeof(serveraddr)) < 0)
		return -1;
	/*
	Ready listenfd to accept connection requests
	*/
	if(listen(listenfd, LISTENQ) < 0)
		return -1;
	return listenfd;
}// open_listenfd

void echo(int connfd);

int server(int argc, char **argv) 
{
    int listenfd, connfd, listenPort;
    socklen_t clientlen;
    struct sockaddr_in clientAddr;
    struct hostent *clientHostEntry;
    char *clientIP;
    unsigned short clientPort
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    listenPort = atoi(argv[1]);

    listenfd = open_listenfd(listenPort);
    while (1) { //loop forever awaiting connection
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	if (connfd<1){
		perror("accept error");
	}
	/* Determine the domain name and IP address of the client */
	clientHostEntry = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			   sizeof(clientAddr.sin_addr.s_addr), AF_INET);
	clientIP = inet_ntoa(clientAddr.sin_addr);
	clientPort = ntohs(clientAddr.sin_port);
	printf("server connected to %s (%s) on my new clientPort %u\n", clientHostEntry->h_name, clientIP, clientPort);

	echo(connfd); //read and then write
	Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */

