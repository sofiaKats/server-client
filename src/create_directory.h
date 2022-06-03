#ifndef CREATE_DIRECTORY_H
#define CREATE_DIRECTORY_H

#pragma once
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "validation.h"

// function to keep only the filename/directory name excluding the path
char * basename (const char *filename);

// receives filenames from the server
void receive_filenames(char* directory, int sock);

#endif //CREATE_DIRECTORY_H