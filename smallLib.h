#include "csapp.h"

#define MAXVARNAME 15
#define MAXVARVAL 100

int smallSet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int dataLength);

int smallGet(char *MachineName, int port, int SecretKey, char *variableName, char *value, short *resultLength);

int smallDigest(char *MachineName, int port, int SecretKey, char *data, short dataLength, char *result, short *resultLength);
