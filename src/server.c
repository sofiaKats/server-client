#include <stdio.h>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */		
#include <stdlib.h>	         /* exit */
#include <ctype.h>	         /* toupper */
#include <signal.h>          /* signal */
#include <pthread.h>   /* For threads  */
#define perror2(s,e) fprintf(stderr, "%s: %s\n", s, strerror(e))


void *child_server(void *argp);

struct thread_funct_args{
    int newsock;
};

#include "validation.h"

// ./dataServer -p 12500 -s 2 -q 2 -b 512
int main(int argc, char* argv[]) 
{
    int port, thread_pool_size, queue_size, block_size;
    // check command line arguments
    if(!server_valid_args(argc, argv, &port, &thread_pool_size, &queue_size, &block_size)) exit(1);
    printf("Server's parameters are: \n");
    printf("port: %d\nthread_pool_size: %d\nqueue_size: %d\nblock_size: %d\n", port, thread_pool_size, queue_size, block_size);
    
    int sock, newsock;
    struct sockaddr_in server, client;
    socklen_t clientlen;
    struct sockaddr *serverptr=(struct sockaddr *)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;

    /* Create socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket @ server.c line 34\n");

    server.sin_family = AF_INET;       /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);      /* The given port */
    
    /* Bind socket to address */
    if (bind(sock, serverptr, sizeof(server)) < 0)
        perror_exit("bind @ server.c line 42\n");
    printf("Server was successfully initialized...\n");

    /* Listen for connections */
    if (listen(sock, 5) < 0) 
        perror_exit("listen @ server.c line 46\n");
    printf("Listening for connections to port %d\n", port);

    while (1) 
    { 
        clientlen = sizeof(client);
        /* accept connection */
    	if ((newsock = accept(sock, clientptr, &clientlen)) < 0) perror_exit("accept");

    	/* Find client's address */
        if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
            herror("gethostbyaddr"); 
            exit(1);
        }
        printf("Accepted connection from %s\n", rem->h_name);

        // creation of communication thread
        pthread_t thr;
        int err, status;
        struct thread_funct_args args;
        args.newsock  = newsock;
        if (err = pthread_create(&thr, NULL, child_server, (void *) &args)) { /* New thread */
            perror2("pthread_create", err);
            exit(1);
            }

        if (err = pthread_join(thr, (void **) &status)) { /* Wait for thread */
            perror2("pthread_join", err); /* termination */
            exit(1);
            }
    	// switch (fork()) {    /* Create child for serving client */
    	// case -1:     /* Error */
    	//     perror("thread @ line 65\n"); break;
    	// case 0:	     /* Child process */
    	//     close(sock); child_server(newsock);
    	//     exit(0);
    	// }
    	close(newsock); /* parent closes socket to client */
    }

    return 0;
}

void * child_server(void *argp) {
    struct thread_funct_args *args = (struct thread_funct_args*) argp;
    char buf[1];
    while(read(args->newsock, buf, 1) > 0) {  /* Receive 1 char */
    	putchar(buf[0]);           /* Print received char */
    	/* Capitalize character */
    	buf[0] = toupper(buf[0]);
    	/* Reply */
    	if (write(args->newsock, buf, 1) < 0)
    	    perror_exit("write @ child_server line 84\n");
    }
    printf("Closing connection.\n");
    close(args->newsock);	  /* Close socket */
    pthread_exit(NULL); 
}


