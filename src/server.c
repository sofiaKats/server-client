#include <stdio.h>

#include "validation.h"

// ./dataServer -p 12500 -s 2 -q 2 -b 512
int main(int argc, char* argv[]) 
{
    int port, thread_pool_size, queue_size, block_size;
    // check command line arguments
    if(!server_valid_args(argc, argv, &port, &thread_pool_size, &queue_size, &block_size)) exit(1);
    printf("Server's parameters are: \n");
    printf("port: %d\nthread_pool_size: %d\nqueue_size: %d\nblock_size: %d\n", port, thread_pool_size, queue_size, block_size);
    
    return 0;
}
