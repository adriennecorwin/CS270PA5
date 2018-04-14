CFLAGS = -Wall -g
LDLIBS = -lpthread

SERVER = smalld
CLIENTS = smallSet smallGet smallDigest smallRun
CSAPP = csapp.h csapp.c

all: $(CSAPP) $(SERVER) $(CLIENTS)

csapp.h:
	wget http://csapp.cs.cmu.edu/2e/ics2/code/include/csapp.h

csapp.c:
	wget http://csapp.cs.cmu.edu/2e/ics2/code/src/csapp.c

csapp.o: csapp.h csapp.c

smallLib.o: smallLib.h smallLib.c
	gcc $(CFLAGS) -c smallLib.c

$(SERVER): csapp.o
$(CLIENTS): csapp.o smallLib.o

.PHONY: clean
clean:
	/bin/rm -rf csapp.h csapp.c *.o smalld smallSet smallGet smallDigest smallRun smallLib
