//smalld server

#include "csapp.h"

#define MAXVARNUM 1000 //can set 1000 variables per server run
#define MAXVARVAL 100 //variable values limited to 255 characters
#define MAXVARNAME 15 //variable names limited to 15 characters
#define COMMANDLEN 31 //base length of digest command

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

//information received from smallDigest client
struct Digest {
	short length;
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

//serves smallSet request
void serveSet(int connfd, rio_t rio, char **varNames, char **varVals, int *numVars){
	size_t readStruct, readVal;
	short length = 0; //holds length of variable value
	struct Set receiveSet;
	struct GenSend genSet;
	readStruct = Rio_readnb(&rio, &receiveSet, sizeof(receiveSet)); //read struct sent from smallSet client
	if(readStruct<0){ //if failed to read
		genSet.status=-1; //fail status
	}
	length = ntohs(receiveSet.length); //length of value = length from struct
	char val[length-1];
	readVal = Rio_readnb(&rio, val, length+1); //read variable value from smallSet
	//printf("server received %d bytes\n", (int)readVal);
	if(readVal<0){ //if failed to read
		genSet.status=-1; //fail status
	}
	printf("Detail = %s: %s\n", receiveSet.variable, val);
	int matchIndex=0; //holds index of matching variable name in variable name array
	while(matchIndex<*numVars && strcmp(receiveSet.variable, varNames[matchIndex])!=0){
		matchIndex+=1; //find index of matching variable name in variable name array
	}
	if(matchIndex!=*numVars){ //if variable is already set
                strncpy(varVals[matchIndex], val, MAXVARVAL); //reassign variable value
		genSet.status=0;
		for(int paddingIndex=0; paddingIndex<3; paddingIndex+=1){ //make padding for struct
			genSet.padding[paddingIndex]=1;
		}
		Rio_writen(connfd, &genSet, sizeof(genSet)); //send struct back to client
        }
	else{ //no variable with this name has been set yet
		*numVars+=1; //increment number of variables by one
		printf("%i\n", *numVars);
		strncpy(varNames[*numVars-1], receiveSet.variable, strlen(receiveSet.variable)+1); //add variable name to variable name array
		strncpy(varVals[*numVars-1], val, strlen(val)+1); //add variable value to variable value array (in corresponding index --parallel arrays)
		genSet.status=0; //success status
		for(int paddingIndex=0; paddingIndex<3; paddingIndex+=1){ //make padding for struct
                        genSet.padding[paddingIndex]=1;
                }
		Rio_writen(connfd, &genSet, sizeof(genSet)); //send struct back to client
	}
	if(genSet.status==0){
		printf("Completion = success\n"); //print success message
	}
	else{
		printf("Completion = failed\n"); //print failure message
	}
	printf("%i\n", *numVars);

	//printf("%s\n", varNames[0]);
}

//serves smallGet
void serveGet(int connfd, rio_t rio, char **varNames, char **varVals, int *numVars){
	size_t readStruct;
	printf("%s\n", varNames[0]);
	struct Get receiveGet;
	struct GenSend genGet;
	struct ExtraSend extraGet;
	int matchIndex=0; //holds index of matching variable name in variable name array
	for(int paddingIndex=0; paddingIndex<3; paddingIndex+=1){ //make padding for struct
                        genGet.padding[paddingIndex]=1;
        }
	//char name[15];
	readStruct = Rio_readnb(&rio, &receiveGet, sizeof(receiveGet)); //read variable name from client
	if(readStruct<0){ //if read fails
		genGet.status=-1; //fail status
	}
	printf("Detail = %s\n", receiveGet.variable); 
	while(matchIndex<*numVars && strcmp(receiveGet.variable, varNames[matchIndex])!=0){ //find index of matching variable name
                matchIndex+=1;
        }
        if(matchIndex!=*numVars){ //if matching variable exists in variable name arrays
		genGet.status=0; //success status
		printf("Completion = success\n");
		extraGet.length=ntohs(strlen(varVals[matchIndex])); //set length = length of value of variable
		Rio_writen(connfd, &genGet, sizeof(genGet)); //send struct back to client
		Rio_writen(connfd, &extraGet, sizeof(extraGet)); //send length of value back to client
                Rio_writen(connfd, varVals[matchIndex], strlen(varVals[matchIndex])+1); //send value of variable back to client
        }
        else{ //if no variable with requested name exists in variable name array
		genGet.status=-1; //failure status
		printf("Completion = failure\n");
		Rio_writen(connfd, &genGet, sizeof(genGet)); //send struct back to client
        }
	
}

//serves smallDigest
void serveDigest(int connfd, rio_t rio){
	size_t readStruct, readVal;
	short length;
	struct Digest receiveDigest;
	struct ExtraSend extraDigest;
	struct GenSend genDigest;
	FILE *fd;
	char digestResult[MAXVARVAL];
	for(int paddingIndex=0; paddingIndex<3; paddingIndex+=1){
                genDigest.padding[paddingIndex]=1;
        }
	readStruct = Rio_readnb(&rio, &receiveDigest, sizeof(receiveDigest)); //read struct from smallDigest client
	if(readStruct<0){ //if read fails
		genDigest.status=-1; //fail status
	}
	length = ntohs(receiveDigest.length);
	char val[length-1];
	readVal = Rio_readnb(&rio, val, length+1);
	if(readVal<0){
		genDigest.status=-1;
	}
	printf("Detail = %s\n", val);
	char command[COMMANDLEN+length];
	sprintf(command, "/bin/echo %s | /usr/bin/sha256sum", val);
	fd = popen(command, "r");
	if(fd =='\0'){
		genDigest.status=-1;
	}
	if(fgets(digestResult, MAXVARVAL, fd)=='\0'){
		genDigest.status=-1;
	}
	else{
		genDigest.status=0;
	}
	short size = strlen(digestResult);
	digestResult[size]='\0';
	pclose(fd);
	extraDigest.length = htons(size);
	Rio_writen(connfd, &genDigest, sizeof(genDigest));
        Rio_writen(connfd, &extraDigest, sizeof(extraDigest));
        Rio_writen(connfd, digestResult, size+1);
	size = 0;
	if(genDigest.status==0){
		printf("Completion = success\n");
	}
	else if(genDigest.status==-1){
		printf("Completion = failure\n");
	}
}	

int main(int argc, char **argv)
{
	int listenfd, connfd, listenPort, serverSecretKey, clientSecretKey, numVars=0;
	short requestType;
	socklen_t clientlen;
	struct sockaddr_in clientAddr;
	rio_t rio;
	size_t readGenStruct;
	struct General receiveGen;
	char **varNames = (char**)malloc(MAXVARNUM*sizeof(char*)); //initialize 2D array to store variable names
	for (int i=0; i<MAXVARNUM; i++){
		varNames[i]=(char*)malloc(MAXVARNAME*sizeof(char));
	}
	char **varVals = (char**)malloc(MAXVARNUM*sizeof(char*)); //initialize 2D array to store variable values
	for (int i=0; i<MAXVARNUM; i++){
		varVals[i]=(char*)malloc(MAXVARVAL*sizeof(char));
	}
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
		Rio_readinitb(&rio, connfd);
		readGenStruct = Rio_readnb(&rio, &receiveGen, sizeof(receiveGen)); 
		printf("server received %d bytes\n", (int)readGenStruct);
		clientSecretKey = ntohl(receiveGen.secret); //get secret key from client
		requestType = ntohs(receiveGen.request); //get request type from client
		printf("\nSecret key = %i\n", clientSecretKey); 
		if(clientSecretKey!=serverSecretKey){ //if client and server secret keys don't match
			printf("Error: Secret keys do not match\n"); //don't serve client
			return -1;
		}
		if(requestType==0){	//if server got a smallSet request
			printf("Request type = set\n"); 
			printf("%i\n", numVars);
			serveSet(connfd, rio, varNames, varVals, &numVars); //serve smallSet
			printf("%i\n", numVars);
		}
		else if(requestType==1){ //if server got a smallGet request
			printf("Request type = get\n");
			serveGet(connfd, rio, varNames, varVals, &numVars); //serve smallGet
		}
		else if(requestType==2){ //if server got a smallDigest request 
			printf("Request type = digest\n");
			serveDigest(connfd, rio);
		}
		else{
			printf("Error\n");
		}
		printf("---------------------------------------------------\n");
		Close(connfd);
	}

	exit(0);
}

