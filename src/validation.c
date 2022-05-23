#include "validation.h"

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