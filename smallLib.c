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

//smallSet-smallSet client request
int smallSet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int dataLength)
{
	int clientfd;
        rio_t rio;
	struct General genInfo;
	struct Set setInfo;
	struct GenRec receiveSet;
	size_t readSet;
	genInfo.secret = htonl(SecretKey); //fill in struct to send to server
	genInfo.request = htons(0);
	genInfo.padding = htons(1);
	if(strlen(variableName)<=MAXVARNAME){ //check if user is setting a variable name longer than 15 characters
		strncpy(setInfo.variable, variableName, strlen(variableName)+1); //put variable name into struct
	}
	else{
		printf("warning: variable names can only hold 15 characters or less\n"); //if variable name is longer than 15 char print warning
		strncpy(setInfo.variable, variableName, MAXVARNAME); //only send first 15 characters of variable name to server
	}
	if(dataLength<=MAXVARVAL){ //check if variable value is over 100 characters
                setInfo.length = htons(dataLength); //send length of value
        }
        else{ //if over 100 characters
                setInfo.length = htons(MAXVARVAL);
        }
	clientfd = Open_clientfd(MachineName, port); 
	Rio_readinitb(&rio, clientfd);
	Rio_writen(clientfd, &genInfo, sizeof(genInfo)); //send general struct info to server
	Rio_writen(clientfd, &setInfo, sizeof(setInfo)); //send smallSet info as struct to server
	if(dataLength<=MAXVARVAL){
		Rio_writen(clientfd, value, dataLength+1);
	}
	else{
		printf("warning: variable values can only hold 100 characters or less\n");
		value[MAXVARVAL+1]='\0';
		Rio_writen(clientfd, value, MAXVARVAL+1); //send only 100 characters
	}
	readSet = Rio_readnb(&rio, &receiveSet, sizeof(receiveSet)); //read struct from server
	if (readSet<0 || receiveSet.status==-1){ //if read fails or server sent fail status
		Close(clientfd);
		return -1;
	}
	else{
		Close(clientfd);
		return 0;
	}
}

//smallGet-smallGet client request
int smallGet(char *MachineName, int port, int SecretKey, char *variableName, char *value, short *resultLength)
{
	int clientfd;
	rio_t rio;
	size_t readStruct1, readStruct2, readVal;
	struct General genInfo;
	struct Get getInfo;
	struct GenRec receiveGet;
	struct RecLen getLen;
	strcpy(getInfo.variable, variableName); //get variable name from client
	genInfo.secret = htonl(SecretKey); //fill structure to send to server
	genInfo.request = htons(1); //request status-get
	genInfo.padding = htons(1);
	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);
	Rio_writen(clientfd, &genInfo, sizeof(genInfo)); //send general info structure to server
	Rio_writen(clientfd, &getInfo, sizeof(getInfo)); //send smallGet info structure to server
	readStruct1 = Rio_readnb(&rio, &receiveGet, sizeof(receiveGet)); //read struct from server
	readStruct2 = Rio_readnb(&rio, &getLen, sizeof(getLen)); //read variable value length from server
	if(readStruct1<0||readStruct2<0){ //if read fails
		return -1;
	}
	*resultLength = htons(getLen.length); 
	char val[*resultLength-1]; //holds value of variable
	readVal = Rio_readnb(&rio, val, *resultLength+1); //read value of variable from server
	if(readVal<0){
                return -1;
        }
	strncpy(value, val, *resultLength+1); //set value to value of variable 
	//printf("server received %d bytes\n", (int)readVal);
	if (receiveGet.status==-1){
		Close(clientfd);
		return -1;
	}
	else{
		Close(clientfd);
		return 0;
	}
}

//smallDigest-smallDigest client request
int smallDigest(char *MachineName, int port, int SecretKey, char *data, short dataLength, char *result, short *resultLength){
	int clientfd;
	rio_t rio;
	size_t readStruct1, readStruct2, readResult;
	struct General genInfo;
	struct Digest digestInfo;
	struct GenRec receiveDigest;
	struct RecLen getLen;
	genInfo.secret = htonl(SecretKey);
	genInfo.request = htons(2);
	genInfo.padding = htons(1);
	digestInfo.length = htons(dataLength);
	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);
	Rio_writen(clientfd, &genInfo, sizeof(genInfo));
	Rio_writen(clientfd, &digestInfo, sizeof(digestInfo));
	Rio_writen(clientfd, data, dataLength+1);
	readStruct1 = Rio_readnb(&rio, &receiveDigest, sizeof(receiveDigest));
	if(receiveDigest.status == -1){
		return -1;
	}
	readStruct2 = Rio_readnb(&rio, &getLen, sizeof(getLen));
	if (readStruct1<0 || readStruct2<0){
		return -1;
	}
	*resultLength = htons(getLen.length);
	char res[*resultLength-1];
	readResult = Rio_readnb(&rio, res, *resultLength+1);
	if (readResult<0){
		return -1;
	}
	strncpy(result, res, *resultLength+1);
	if (receiveDigest.status==-1){
                Close(clientfd);
                return -1;
        }
        else{
                Close(clientfd);
                return 0;
        }
}
