#ifndef DIRECTORY_H
#define DIRECTORY_H

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
#include <string.h>
#include  <dirent.h>

#include "queue.h"
#include "validation.h"
// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf
// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic6-Threads.pdf

typedef struct thread_funct_args{
    int newsock;
    char directory[512];
    Queue* queue;
    int queue_size;
    int block_size;
}thread_funct_args;

// receives directory name from client
void *receive_dir_name(void *argp);

// given a directory name, the function recursively stores filenames to given queue
void recursive_list_dirs(char dirname[], Queue** queue, int newsock, int queue_size);

// Same function as the producer-cosumer place function 
void place(char* filepath, Queue** queue, int newsock, int queue_size);

// worker thread function
void * consumer(void *argp);

int obtain(Queue** queue, int block_size);

// Initialize thread conds and mutexes
void InitializeCondMtx(void);

// Destroy thread conds and mutexes
void DestroyCondMtx(void);

#endif //DIRECTORY_H