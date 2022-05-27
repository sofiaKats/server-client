#ifndef SERVER_FUNCTS_H
#define SERVER_FUNCTS_H

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

typedef struct thread_funct_args{
    int newsock;
    char directory[512];
}thread_funct_args;

// receives directory name from client
void *receive_dir_name(void *argp);

void recursive_list_dirs(char dirname[]);

#endif //SERVER_FUNCTS_H