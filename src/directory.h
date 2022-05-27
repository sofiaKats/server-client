#ifndef DIRECTORY_H
#define DIRECTORY_H

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

// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf

typedef struct thread_funct_args{
    int newsock;
    char directory[512];
}thread_funct_args;

// receives directory name from client
void *receive_dir_name(void *argp);

// given a directory name, the function recursively stores filenames to given queue
void recursive_list_dirs(char dirname[]);

#endif //DIRECTORY_H