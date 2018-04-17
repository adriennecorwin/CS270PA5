#include "smallLib.h"
#include "csapp.h"

struct General {

	unsigned int secret;

	short request;

	short padding;
};

struct Set {

	char variable[15];

	short length;

};

struct Get {
	char variable[15];
};

struct GenRec {
	char status;
	char padding[3];
};

struct RecLen{
	short length;
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
	//printf("%i\n", dataLength);
	struct General data;
	struct Set info;
	char *val;
	data.secret = htonl(SecretKey);
	data.request = htons(0);
	data.padding = htons(1);
	strncpy(info.variable, variableName, strlen(variableName));
	/*for (int index = 0; index < strlen(variableName); index += 1){
		info.variable[index] = variableName[index];
	}*/
	info.length = htons(dataLength);
	/*for (int index = 0; index < strlen(value); index += 1){
		info.val[index] = value[index];
	}*/
	//strncpy(val, value, strlen(value));
	int clientfd;
	rio_t rio;
	// struct hostent *hp;
	//  struct sockaddr_in serveraddr;
//	printf("%s\n", info.val);
	//printf("%i\n", info.length);
	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);
	//bzero(&data, sizeof(data));
	Rio_writen(clientfd, &data, sizeof(data));
	//printf("%i\n", sizeof(info));
	//printf("%s\n", value);
	Rio_writen(clientfd, &info, sizeof(info));
	Rio_writen(clientfd, value, dataLength+1);
	//Rio_readlineb(&rio, buf, MAXLINE);
	//  Fputs(buf, stdout);
	//}
	Close(clientfd); //line:netp:echoclient:close
	return clientfd;
}

int smallGet(char *MachineName, int port, int SecretKey, char *variableName, int *resultLength)
{
        int clientfd;
        rio_t rio;
	struct General genInfo;
	struct Get getInfo;
	struct GenRec receiveGet;
	struct RecLen getLen;
        // struct hostent *hp;
        //  struct sockaddr_in serveraddr;
	strcpy(getInfo.variable, variableName);
        genInfo.secret = htonl(SecretKey);
        genInfo.request = htons(1);
        genInfo.padding = htons(1);
        clientfd = Open_clientfd(MachineName, port);
        Rio_readinitb(&rio, clientfd);
        //bzero(buf, MAXLINE);
        //  while (Fgets(buf, MAXLINE, stdin) != NULL) {
//	printf("%s\n", variableName);
	Rio_writen(clientfd, &genInfo, sizeof(genInfo));
//	printf("%i\n", sizeof(variableName));
        Rio_writen(clientfd, &getInfo, sizeof(getInfo));
        //bzero(buf, MAXLINE);
        //  Rio_readlineb(&rio, buf, MAXLINE);
        //Fputs(buf, stdout);
        //}
	size_t n;
	n = Rio_readnb(&rio, &receiveGet, sizeof(receiveGet));
	n = Rio_readnb(&rio, &getLen, sizeof(getLen));
	*resultLength = ntohs(getLen.length);
	char value[*resultLength-1];
	n = Rio_readnb(&rio, value, sizeof(value));
	if (receiveGet.status==0){
		printf("%s\n", value);
	}
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
