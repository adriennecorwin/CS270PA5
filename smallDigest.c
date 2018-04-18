
#include "csapp.h"
#include "smallLib.h"




int main(int argc, char **argv)
{
    int port, SecretKey, resultLength;
	short dataLength;
    char *host, buf[MAXLINE], *data, *result;
//    rio_t rio;

   if (argc != 5) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
else{
    host = argv[1];
    port = atoi(argv[2]);
SecretKey = atoi(argv[3]);
data = argv[4];
//value = argv[5];

dataLength = (int)atoi(argv[6]);
int delta = smallDigest(host, port, SecretKey, data, dataLength, result, &resultLength);
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


