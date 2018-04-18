/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

#define MAXVARNUM 1000 //can set 1000 variables per server run
#define MAXVARVAL 100 //variable values limited to 255 characters
#define MAXVARNAME 15 //variable names limited to 15 characters

//void echo(int connfd);

//General information received from every client request
struct General{

	unsigned int secret;

	short request;

	short padding;
};

//information received from smallSet client
struct Set{

	char variable[MAXVARNAME];

	short length;

};

//information received from smallGet client
struct Get{
	
	char variable[MAXVARNAME];

};

struct Digest {
	short length;
	char *val;
};

//general information sent back to client
struct GenSend{
	char status;
	char padding[3];
};

//information sent back to client from serveGet and serveDigest
struct ExtraSend{
	short length;
};

void serveSet(int connfd, rio_t rio, char **varNames, char **varVals, int *numVars){
	size_t n;
	short length = 0;
	struct Set receiveSet;
	struct GenSend genSet;
	n = Rio_readnb(&rio, &receiveSet, sizeof(receiveSet));
	length = ntohs(receiveSet.length);
	char val[length-1];
//	printf("server received %d bytes\n", (int)n);
	n = Rio_readnb(&rio, val, length+1);
	printf("Detail = %s: %s\n", receiveSet.variable, val);
//	printf("server received %d bytes\n", (int)n);
	int matchIndex=0;
	while(matchIndex<*numVars && strcmp(receiveSet.variable, varNames[matchIndex])!=0){
		matchIndex+=1;
	}
	if(matchIndex!=*numVars){
                strncpy(varVals[matchIndex], val, MAXVARVAL);
		genSet.status=0;
		for(int paddingIndex=0; paddingIndex<3; paddingIndex+=1){
			genSet.padding[paddingIndex]=1;
		}
		Rio_writen(connfd, &genSet, sizeof(genSet));
        }
	else{
		*numVars+=1;
		strncpy(varNames[*numVars-1], receiveSet.variable, strlen(receiveSet.variable));
		strncpy(varVals[*numVars-1], val, strlen(val));
		genSet.status=0;
		for(int paddingIndex=0; paddingIndex<3; paddingIndex+=1){
                        genSet.padding[paddingIndex]=1;
                }
		Rio_writen(connfd, &genSet, sizeof(genSet));
	}
//	printf("%s\n", varNames[0]);
	printf("Completion = success\n");
	//store in parallel arrays
}

void serveGet(int connfd, rio_t rio, char **varNames, char **varVals, int *numVars){
	size_t n;
	struct Get receiveGet;
	struct GenSend genGet;
	struct ExtraSend extraGet;
	int matchIndex=0;
	for(int paddingIndex=0; paddingIndex<3; paddingIndex+=1){
                        genGet.padding[paddingIndex]=1;
        }
//	printf("%i\n", sizeof(genGet.padding));
	//char name[15];
	n = Rio_readnb(&rio, &receiveGet, sizeof(receiveGet));
	//n = Rio_readnb(&rio, name, strlen(name));
//	printf("%s\n", receiveGet.variable);
//	printf("server received %d bytes\n", (int)n);
	/*while(matchIndex<*numVars && strcmp(name, varNames[matchIndex])!=0){
		matchIndex+=1;
	}
	if(matchIndex!=*numVars){
		Rio_writen(connfd, varVals[matchIndex], strlen(varVals[matchIndex]));
	}
	else{
		printf("Error: variable %s does not exist\n", name);
	}*/
	while(matchIndex<*numVars && strcmp(receiveGet.variable, varNames[matchIndex])!=0){
                matchIndex+=1;
        }
        if(matchIndex!=*numVars){
		genGet.status=0;
		extraGet.length=ntohs(strlen(varVals[matchIndex]));
//		printf("%i\n", extraGet.length);
		Rio_writen(connfd, &genGet, sizeof(genGet));
		Rio_writen(connfd, &extraGet, sizeof(extraGet));
                Rio_writen(connfd, varVals[matchIndex], strlen(varVals[matchIndex])+1);
        }
        else{
		genGet.status=-1;
		Rio_writen(connfd, &genGet, sizeof(genGet));
                printf("Error: variable %s does not exist\n", receiveGet.variable);
        }

	//find matching name in parallel arrays and get corresponding value
	//send value to client
	
}

void serveDigest(int connfd, rio_t rio){
	size_t n;
	short length;
	struct Digest receiveDigest;
	n = Rio_readnb(&rio, &receiveDigest, sizeof(receiveDigest));
	length = ntohs(receiveDigest.length);
	char val[length-1];
	n = Rio_readnb(&rio, val, length+1);
}	

int main(int argc, char **argv)
{
	int listenfd, connfd, listenPort, clientSecretKey, numVars=0;
	short requestType;
	int serverSecretKey;
	socklen_t clientlen;
	struct sockaddr_in clientAddr;
	struct hostent *clientHostEntry;
	char *clientIP;
	unsigned short clientPort;
	size_t n;
	struct General receiveGen;
	struct Set receiveSet;
	char **varNames = (char**)malloc(MAXVARNUM*sizeof(char*)); //initialize 2D array to store variable names
	for (int i=0; i<MAXVARNUM; i++){
		varNames[i]=(char*)malloc(MAXVARNAME*sizeof(char));
	}
	char **varVals = (char**)malloc(MAXVARNUM*sizeof(char*)); //initialize 2D array to store variable values
	for (int i=0; i<MAXVARNUM; i++){
		varVals[i]=(char*)malloc(MAXVARVAL*sizeof(char));
	}
	rio_t rio;
	if (argc != 3) {
		fprintf(stderr, "usage: %s <port> <SecretKey>\n", argv[0]);
		exit(0);
	}

	listenPort = atoi(argv[1]);
	serverSecretKey = atoi(argv[2]);
	listenfd = open_listenfd(listenPort);
	while (1) { //loop forever awaiting connection
		clientlen = sizeof(clientAddr);
		connfd = Accept(listenfd, (SA *)&clientAddr, &clientlen);
		if (connfd<1){
			perror("accept error");
		}
		/* Determine the domain name and IP address of the client */
		clientHostEntry = Gethostbyaddr((const char *)&clientAddr.sin_addr.s_addr, sizeof(clientAddr.sin_addr.s_addr), AF_INET);
		clientIP = inet_ntoa(clientAddr.sin_addr);
		clientPort = ntohs(clientAddr.sin_port);
		printf("server connected to %s (%s) on my new clientPort %u\n", clientHostEntry->h_name, clientIP, clientPort);	
		Rio_readinitb(&rio, connfd);
		n = Rio_readnb(&rio, &receiveGen, sizeof(receiveGen)); 
		printf("server received %d bytes\n", (int)n);
		clientSecretKey = ntohl(receiveGen.secret);
		requestType = ntohs(receiveGen.request);
		printf("\nSecret key = %i\n", clientSecretKey);
		if(clientSecretKey!=serverSecretKey){
			printf("Error: Secret keys do not match\n");
		}
		if(requestType==0){	
			printf("Request type = set\n");
			serveSet(connfd, rio, varNames, varVals, &numVars);
		}
		else if(requestType==1){
			printf("Request type = get\n");
			serveGet(connfd, rio, varNames, varVals, &numVars);
		}
		else if(requestType==2){
			printf("Request type = digest\n");
		}
		else{
			printf("Error\n");
		}
		printf("---------------------------------------------------\n");
		Close(connfd);
	}

//	exit(0);
}
/* $end echoserverimain */

