#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int server_valid_args(int argc, char **argv, int *port, int *thread_pool_size, int *queue_size, int *block_size);

int main(int argc, char* argv[]) 
{
    int port, thread_pool_size, queue_size, block_size;
    // check command line arguments
    if(!server_valid_args(argc, argv, &port, &thread_pool_size, &queue_size, &block_size)) exit(1);
    return 0;
}

// ./dataServer -p <port> -s <thread_pool_size> -q <queue_size> -b <block_size>
// Όπου:
// 1. port: Η θύρα στην οποία ο server θα ακούει για εξωτερικές συνδέσεις.
// 2. thread pool size: Ο αριθμός των worker threads στο thread pool.
// 3. queue size: Ο αριθμός θέσεων στην ουρά εκτέλεσης.
// 4. block size: Το μέγεθος των μπλοκ των αρχείων σε bytes που θα στέλνουν οι worker threads.

int server_valid_args(int argc, char **argv, int *port, int *thread_pool_size, int *queue_size, int *block_size)
{
    if (argc != 9) {
        printf("Please give exactly 9 arguments.\n\n");
        return 0;
    }
    
    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-p")) 
            *port = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-s"))
            *thread_pool_size = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-q"))
            *queue_size = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-b"))
            *block_size = atoi(argv[++i]);
        else
        {
            printf("Invalid command line argument option given: %s \n\n\n",argv[i]);
            return 0;
        }
    }

    return 1; // all arguments are ok
}