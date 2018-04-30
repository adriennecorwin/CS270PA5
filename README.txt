


Files:

smalld.c
smallLib.c
smallLib.h
smallSet.c
smallGet.c
smallDigest.c
makefile
README.txt


Names:

Tabitha Holloway
Adrienne Corwin


Documentation:

We used parallel arrays to store the variable name and value of the variable name for the smallSet function, so that when the user wants access to the value of a variable, using smallGet, the function will be able to search for the value based on the name the user provides. For the smallDigest function, we used popen to store the output of the invocation, /bin/echo value | /usr/bin/sha256sum, into a file descriptor, where then the contents of the file get stored into a buffer that is written to the client. The data structures that we use for sending messages from server to client, and from client to server, are structs.


Limitations:

One limitation of our project is that the user cannot set a variable to an empty string. Another limitation is that the variable names can only be 15 characters long, and the values can only be 100 characters long. Not only so, but we are unsure if the program can accept any binary data to perform its functions. Another limitation is that our program cannot except input in between tick marks (` `).

