#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int server_valid_args(int argc, char **argv, int *port, int *thread_pool_size, int *queue_size, int *block_size);


#endif //VALIDATION_H