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
