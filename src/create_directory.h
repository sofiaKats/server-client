#ifndef CREATE_DIRECTORY_H
#define CREATE_DIRECTORY_H

#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

// function to keep only the filename/directory name excluding the path
char * basename (const char *filename);

// for the client to know if the accepted message is a directory 
// or just another file i add '#' character as the first letter of the 
// C string, then i know i received a directory from server (since # can't be a character
// of a filename or a directory name), returns 1 if name is directory else returns 0
int sanitize(char *str);

// receives filenames from the server
void receive_filenames(char* directory, int sock);

#endif //CREATE_DIRECTORY_H