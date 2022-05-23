#include <stdio.h>

#include "validation.h"

// ./remoteClient -i 127.0.0.1 -p 12500 -d Server
int main(int argc, char* argv[]) 
{
    int server_port;
    char directory[512], server_ip[512];
    // check command line arguments
    if(!client_valid_args(argc, argv, server_ip, &server_port, directory)) exit(1);
    return 0;
}
