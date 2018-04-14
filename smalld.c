/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

//void echo(int connfd);


void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);
	Rio_writen(connfd, buf, n);
    }

}
/* $end echo */




int main(int argc, char **argv)
{
    int listenfd, connfd, listenPort;
    socklen_t clientlen;
    struct sockaddr_in clientAddr;
    struct hostent *clientHostEntry;
    char *clientIP;
    unsigned short clientPort;
    if (argc != 3) {
	fprintf(stderr, "usage: %s <port> <SecretKey>\n", argv[0]);
	exit(0);
    }
    listenPort = atoi(argv[1]);
    SecretKey = atoi(argv[2]);
    listenfd = open_listenfd(listenPort);
    while (1) { //loop forever awaiting connection
	clientlen = sizeof(clientAddr);
	connfd = Accept(listenfd, (SA *)&clientAddr, &clientlen);
	if (connfd<1){
		perror("accept error");
	}
	/* Determine the domain name and IP address of the client */
	clientHostEntry = Gethostbyaddr((const char *)&clientAddr.sin_addr.s_addr, 
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

