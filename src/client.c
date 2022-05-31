#include <stdio.h>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */

#include "validation.h"

// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic5-Sockets.pdf
// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic6-Threads.pdf


// ./remoteClient -i 127.0.0.1 -p 12500 -d Server
// ./remoteClient -i linux12.di.uoa.gr -p 12500 -d Server
int main(int argc, char* argv[]) 
{
    int server_port;
    char directory[512], server_ip[512];
    // check command line arguments
    if(!client_valid_args(argc, argv, server_ip, &server_port, directory)) exit(1);
    printf("Client's parameters are:\n");
    printf("serverIP: %s\nport: %d\ndirectory: %s\n", server_ip, server_port, directory);

    int sock, i;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

    /* Create socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    	perror_exit("socket @ client.c line 32\n");

	/* Find server address */
    if ((rem = gethostbyname(server_ip)) == NULL) {	
	   herror("gethostbyname @ client.c line 36\n"); 
       exit(1);
    }

    server.sin_family = AF_INET;       /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(server_port);         /* Server port */

    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0)
	   perror_exit("connect @ client.c line 46\n");
    printf("Connecting to %s on port %d\n", server_ip, server_port);

    // write directory name to be copied to server
    if (write(sock, directory, strlen(directory)) < 0) perror_exit("write @client.c line 52\n");
    
    char buffer[512]; memset(buffer, 0, 512);
    while(read(sock, buffer, sizeof(buffer)) > 0) {
        buffer[512] = '\0';
        printf("client received: %s\n", buffer);
        memset(buffer, 0, 512);
    }

    close(sock);                 /* Close socket and exit */
    return 0;
}


