#include "smallLib.h"
#include "csapp.h"


struct Set
{

	unsigned int secret;

	short set;

	short padding;

	char variable[15];

	short length;

	char *val;


};

struct Get
{

	unsigned int secret;

	short set;

	short padding;

	char variable[15];

};


struct Digest
{
	unsigned int secret;

	short set;

	short padding;

	short length;

	char *val;


};


int smallSet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int dataLength)
{

	struct Set info;
	info.secret = htonl(SecretKey);
	info.set = htons(0);
	info.padding = htons(1);

	for (int index = 0; index < strlen(variableName); index += 1)
	{

		info.variable[index] = variableName[index];

	}

	//i.variable = variableName;
	info.length = htons(dataLength);
	info.val = value;

	int clientfd;
	rio_t rio;
	// struct hostent *hp;
	//  struct sockaddr_in serveraddr;

	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);
	//bzero(buf, MAXLINE);
	//  while (Fgets(buf, MAXLINE, stdin) != NULL) {
	Rio_writen(clientfd, &info, sizeof(info));
	//bzero(buf, MAXLINE);
	//Rio_readlineb(&rio, buf, MAXLINE);
	//  Fputs(buf, stdout);
	//}
	Close(clientfd); //line:netp:echoclient:close
	return clientfd;

}


int smallGet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int *resultLength)
{
	int clientfd;
	rio_t rio;
	struct Get info;
	// struct hostent *hp;
	//  struct sockaddr_in serveraddr;

	info.secret = htonl(SecretKey);
	info.set = htons(1);
	info.padding = htons(1);

	for (int index = 0; index < strlen(variableName); index += 1)
	{

		info.variable[index] = variableName[index];

	}

	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);
	//bzero(buf, MAXLINE);
	//  while (Fgets(buf, MAXLINE, stdin) != NULL) {
	Rio_writen(clientfd, &info, sizeof(info));
	//bzero(buf, MAXLINE);
	//  Rio_readlineb(&rio, buf, MAXLINE);
	//Fputs(buf, stdout);
	//}
	Close(clientfd); //line:netp:echoclient:close


	return clientfd;





}

int smallDigest(char *MachineName, int port, int SecretKey, char *data, int dataLength, char *result, int *resultLength)
{

	int clientfd;
	rio_t rio;
	struct Digest info;
	info.secret = htonl(SecretKey);
	info.set = htons(2);
	info.padding = htons(1);


	info.length = htons(dataLength);
	info.val = data;

 clientfd = Open_clientfd(MachineName, port);
        Rio_readinitb(&rio, clientfd);
 Rio_writen(clientfd, &info, sizeof(info));

 Close(clientfd); //line:netp:echoclient:close


        return clientfd;



}
