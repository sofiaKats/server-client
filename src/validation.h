#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// assigns command line arguments to corresponding variable or returns 0 if arguments are invalid
int server_valid_args(int argc, char **argv, int *port, int *thread_pool_size, int *queue_size, int *block_size);

// assigns command line arguments to corresponding variable or returns 0 if arguments are invalid
int client_valid_args(int argc, char** argv, char* server_ip, int *server_port, char* directory);


// for the client to know if the accepted message is a directory 
// or just another file i add '#' character as the first letter of the 
// C string, then i know i received a directory from server (since # can't be a character
// of a filename or a directory name), returns 1 if name is directory else returns 0
int check_if_directory_or_filename(char *str);

// prints out error message
void perror_exit(char *message);

#endif //VALIDATION_H