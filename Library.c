#include "smallLib.h"
#include "csapp.h"


#define MAXVARNAME 15
#define MAXVARVAL 100

//General information passed to server from each client application
struct General {

	unsigned int secret;

	short request;

	short padding;
};

//information passed to the server from smallSet
struct Set {

	char variable[MAXVARNAME];

	short length;

};

//information passed to the server from smallGet
struct Get {
	char variable[MAXVARNAME];
};

struct Digest
{
	short length;
//char *val;
};

//General information received from server and passed back to each application called
struct GenRec {
	char status;
	char padding[3];
};

//Result length received from server and passed to either smllGet or smallDigest
struct RecLen{
	short length;
};

int smallSet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int dataLength)
{
	struct General genInfo;
	struct Set setInfo;
	genInfo.secret = htonl(SecretKey);
	genInfo.request = htons(0);
	genInfo.padding = htons(1);
	if(strlen(variableName)<=MAXVARNAME){
		strncpy(setInfo.variable, variableName, strlen(variableName));
	}
	else{
		printf("variable names must be 15 characters or less\n");
		return -1;
	}
	setInfo.length = htons(dataLength);
	int clientfd;
	rio_t rio;
	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);
	Rio_writen(clientfd, &genInfo, sizeof(genInfo));
	Rio_writen(clientfd, &setInfo, sizeof(setInfo));
	if (strlen(value)<=MAXVARVAL){
		Rio_writen(clientfd, value, dataLength+1);
	}
	else{
		printf("variable values must be 100 characters or less\n");
		return -1;
	}
	Close(clientfd); //line:netp:echoclient:close

/*	if (receiveSet.status<0){
		clientfd = -1;
	}
*/
	return clientfd;
}

int smallGet(char *MachineName, int port, int SecretKey, char *variableName, char *value, short *resultLength)
{
	int clientfd;
	rio_t rio;
	struct General genInfo;
	struct Get getInfo;
	struct GenRec receiveGet;
	struct RecLen getLen;
	strcpy(getInfo.variable, variableName);
	genInfo.secret = htonl(SecretKey);
	genInfo.request = htons(1);
	genInfo.padding = htons(1);
	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);
	Rio_writen(clientfd, &genInfo, sizeof(genInfo));
	Rio_writen(clientfd, &getInfo, sizeof(getInfo));
	size_t n;
	n = Rio_readnb(&rio, &receiveGet, sizeof(receiveGet));
	n = Rio_readnb(&rio, &getLen, sizeof(getLen));
	*resultLength = htons(getLen.length);
	value=malloc(*resultLength-1);
	printf("%i\n", *resultLength);
	n = Rio_readnb(&rio, value, *resultLength+1);
	printf("server received %d bytes\n", (int)n);
	if (receiveGet.status==0){
		printf("%s\n", value);
	}
	Close(clientfd); //line:netp:echoclient:close

	if (receiveGet.status<0){
		clientfd = -1;
	}
	return clientfd;





}

int smallDigest(char *MachineName, int port, int SecretKey, char *data, int dataLength, char *result, short *resultLength)
{
	/* ***********************  /bin/echo value | /usr/bin/sha256sum ******************  */
	int clientfd;
	size_t n;
	char value[1000];

	rio_t rio;
	struct General genInfo;
	struct Digest digestInfo;
	struct GenRec receiveDigest;
	struct RecLen getLen;

	genInfo.secret = htonl(SecretKey);
	genInfo.request = htons(2);
	genInfo.padding = htons(1);
//printf("%d\n", dataLength);
// strcpy(digestInfo.val, data);
printf("data = %s\n", data);
	digestInfo.length = htons(dataLength);
	//strncpy(digestInfo.val, data, dataLength);
//printf("len = %s\n", dataLength);
printf("HEELLOE\n");
	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);
	Rio_writen(clientfd, &genInfo, sizeof(genInfo));
	Rio_writen(clientfd, &digestInfo, sizeof(digestInfo));
	Rio_writen(clientfd, data, dataLength+1);
	//printf("Hello\n");
printf("reeeeeeeeeee\n");
	n = Rio_readnb(&rio, &receiveDigest, sizeof(receiveDigest));
	n = Rio_readnb(&rio, &getLen, sizeof(getLen));
	//printf("kj\n");
printf("tttttttttttttttttt\n");
	*resultLength = htons(getLen.length);
printf("%d\n", *resultLength);
printf("ppppppppp8resupppppppppp\n");
	char res[*resultLength];
//FILE *fp;
//fp = popen(res, "w");

	//strncpy(result, res, *resultLength+1);
//	result=malloc(*resultLength-1);
	n = Rio_readnb(&rio, res, *resultLength+1);
 printf("server received %d bytes\n", (int)n);
printf("%s\n", res);
/*fp = popen(res, "r");
char c = fgetc(fp);

while (c != EOF)
{
printf("%c", c);
c = fgetc(fp);
//size += 1;
}
pclose(fp);*/
	printf("HIIII\n");
	//result=malloc(*resultLength-1);
	//      printf("%i\n", result);
// strncpy(result, res, *resultLength+1);

//	printf("%s\n", result);

//pclose(fp);

	//        n = Rio_readnb(&rio, result, *resultLength+1);
	//       printf("server received %d bytes\n", (int)n);
	//     if (receiveDigest.status==0){
	//           printf("%s\n", result);
	// }


	/*
	   printf("Detail = : %s\n", *result);

	 */
//	char buf[*resultLength];
//	snprintf(buf, resultLength-1,"/bin/echo %s | /usr/bin/sha256sum", result);
	printf("\n");
//	system(res);
	printf("\n");

	Close(clientfd); //line:netp:echoclient:close

	if (receiveDigest.status<0){
		clientfd = -1;
	}

	return clientfd;



}
