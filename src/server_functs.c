#include "server_funcs.h"

void * receive_dir_name(void *argp) {
    struct thread_funct_args *args = (struct thread_funct_args*) argp; // coping passed argument struct
    char buf[512];
    int err;
    printf("Given directory name is: ");
    while(read(args->newsock, buf, sizeof(buf)) > 0) {  /* Receive 1 char */
    	printf("%s\n", buf);           /* Print received char */
        strcpy(args->directory, buf);
    	/* Capitalize character */
    	//buf[0] = toupper(buf[0]);
    	/* Reply */
    	// if (write(args->newsock, buf, 1) < 0)
        //     fprintf(stderr, "write @ child_server line 84\n");
    }
    printf("directory is (server_funct.c): %s\n", args->directory);
    printf("\nClosing connection.\n");
    close(args->newsock);	  /* Close socket */
    pthread_exit(NULL); 
}

