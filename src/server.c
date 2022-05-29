#pragma once
#include <stdio.h>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */		
#include <stdlib.h>	         /* exit */
#include <ctype.h>	         /* toupper */
#include <pthread.h>   /* For threads  */

#define perror2(s,e) fprintf(stderr, "%s: %s\n", s, strerror(e))

// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic5-Sockets.pdf
// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic6-Threads.pdf

#include "validation.h"
#include "directory.h"
#include "queue.h"

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
    	if ((newsock = accept(sock, clientptr, &clientlen)) < 0) perror_exit("accept@ server.c line 58");

    	/* Find client's address */
        if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
            herror("gethostbyaddr"); 
            exit(1);
        }
        printf("Accepted connection from %s\n", rem->h_name);

        // creation of communication thread
        pthread_t communication_thread;
        int err, status;
        struct thread_funct_args args;  // passing struct of arguments to thread creation function
        args.newsock  = newsock;
        args.queue = Create_Queue();
        args.queue_size = queue_size;
        memset(args.directory, 0, 512); //directory name written by client will be stored here
        InitializeCondMtx();

         //array of worker threads
        pthread_t *worker_threads;
        if ((worker_threads = malloc(thread_pool_size * sizeof(pthread_t))) == NULL) {
            perror("malloc");  exit(1); }

        for (int i=0 ; i<thread_pool_size ; i++) {
            if (err = pthread_create(worker_threads+i, NULL, consumer, (void *) &args)) {
                perror2("pthread_create", err);   exit(1);} 
        }

        if (err = pthread_create(&communication_thread, NULL, receive_dir_name, (void *) &args)) { /* New thread */
            perror2("pthread_create @ server.c line 78", err);  exit(1);}

        if (err = pthread_join(communication_thread, NULL/*, (void **) &status*/)) { /* Wait for thread */
            perror2("pthread_join @ server.c line 83", err); /* termination */ exit(1);}
        
        for (int i=0 ; i<thread_pool_size ; i++) {
            if (err = pthread_join(*(worker_threads+i), NULL)) {
                perror2("pthread_join", err); exit(1);}
        }

        printf("\nClosing connection.\n\n\n");
    	close(newsock); /* parent closes socket to client */
        DestroyCondMtx();
    }

    return 0;
}


