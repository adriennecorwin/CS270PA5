/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
#include "smallLib.h"

/*
int open_clientfd(char *hostname, int port) {
        int toserverfd;
        struct hostent *serverHostEntry;
        struct sockaddr_in serveraddr;
        if ((toserverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                return -1; // check errno 
                // AF_INET: internet; SOCK_STREAM: reliable; 0: normal
        }
        // Fill in the server.s IP address and port 
        if ((serverHostEntry = gethostbyname(hostname)) == NULL)
                return -2; // check h_errno for cause of error 
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)serverHostEntry->h_addr_list[0],
                        (char *)&serveraddr.sin_addr.s_addr,
                        serverHostEntry->h_length);
        serveraddr.sin_port = htons(port);
        // Establish a connection with the server 
        if (connect(toserverfd, (SA *) &serveraddr,
                                sizeof(serveraddr)) < 0) {
                return -1;
        }
        return toserverfd;
} // open_clientfd
*/



//typedef struct g g;

/*struct Set
{

unsigned int secret;

short set;

short padding;

char variable[15];

short length;

char *val;


};

*/


/*
int smallSet(char* buf, char *MachineName, int port, int SecretKey, char *variableName, char *value, int dataLength)
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
 struct hostent *hp;
    struct sockaddr_in serveraddr;

 clientfd = Open_clientfd(MachineName, port);
    Rio_readinitb(&rio, clientfd);
bzero(buf, MAXLINE);
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        Rio_writen(clientfd, buf, strlen(buf));
bzero(buf, MAXLINE);
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close


return clientfd;

}

*/


int main(int argc, char **argv) 
{
    int port, SecretKey, length = 0;
    char *host, buf[MAXLINE], *var_name, *value;
//    rio_t rio;

   if (argc != 6) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
else{
printf("HIII\n");
    host = argv[1];
    port = atoi(argv[2]);
SecretKey = atoi(argv[3]);
var_name = argv[4];
value = argv[5];
//length = atoi(argv[6]);

printf("HIIIIIIII\n");
int delta = smallSet(host, port, SecretKey, var_name, value, length);
//int secret = atoi(argv[3]);
/*    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL) {
	Rio_writen(clientfd, buf, strlen(buf));
	Rio_readlineb(&rio, buf, MAXLINE);
	Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
  */ 

if (delta < 0)
{

printf("error\n");

}
printf("server = %s\n", buf);
 exit(0);
}
}
/* $end echoclientmain */
