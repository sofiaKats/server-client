#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// assigns command line arguments to corresponding variable or returns 0 if arguments are invalid
int server_valid_args(int argc, char **argv, int *port, int *thread_pool_size, int *queue_size, int *block_size);

// assigns command line arguments to corresponding variable or returns 0 if arguments are invalid
int client_valid_args(int argc, char** argv, char* server_ip, int *server_port, char* directory);

#endif //VALIDATION_H