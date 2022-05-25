#include "server_funcs.h"

void * receive_dir_name(void *argp) {
    struct thread_funct_args *args = (struct thread_funct_args*) argp; // coping passed argument struct
    char buf[1];
    int err;
    printf("Given directiry name is: ");
    while(read(args->newsock, buf, 1) > 0) {  /* Receive 1 char */
    	putchar(buf[0]);           /* Print received char */
    	/* Capitalize character */
    	//buf[0] = toupper(buf[0]);
    	/* Reply */
    	// if (write(args->newsock, buf, 1) < 0)
        //     fprintf(stderr, "write @ child_server line 84\n");
    }
    printf("\nClosing connection.\n");
    close(args->newsock);	  /* Close socket */
    pthread_exit(NULL); 
}

