#include "server_funcs.h"

void * receive_dir_name(void *argp) {
    struct thread_funct_args *args = (struct thread_funct_args*) argp; // coping passed argument struct
    char buf[512];
    memset(buf, 0, 512);
    int err;
    printf("Given directory name is: ");
    while(read(args->newsock, buf, sizeof(buf)) > 0) { 
        strcpy(args->directory, buf);
    }
    printf("directory name received by server(dlt: server_funct.c line 13): %s\n", args->directory);
    printf("\nClosing connection.\n");
    close(args->newsock);	  /* Close socket */
    pthread_exit(NULL); 
}

