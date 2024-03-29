#include "validation.h"

// ./dataServer -p 12500 -s 2 -q 2 -b 512
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

// ./remoteClient -i 127.0.0.1 -p 12500 -d Server
int client_valid_args(int argc, char** argv, char* server_ip, int *server_port, char* directory)
{
    if (argc != 7) {
        printf("Please give exactly 7 arguments.\n\n");
        return 0;
    }
    
    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-i")) 
            strcpy(server_ip, argv[++i]);
        else if (!strcmp(argv[i], "-p"))
            *server_port = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-d"))
            strcpy(directory, argv[++i]);
        else
        {
            printf("Invalid command line argument option given: %s \n\n\n",argv[i]);
            return 0;
        }
    }

    return 1; // all arguments are ok
}

int check_if_directory_or_filename(char *str)
{
    char temp[512]; strcpy(temp, str);
    // str is a directory
    if(strstr(temp, "~#@") != NULL) return DIRECTORY;

    // str is a filename
    memset(temp, 0, 512); strcpy(temp, str);
    if(strstr(temp, "$^$*") != NULL) return FILENAME;

    // str is contents of file
    return CONTENTS;
}


void perror_exit(char *message) {
    // perror(message);
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}