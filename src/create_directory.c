#include "create_directory.h"

void receive_filenames(char* directory, int sock) {
    char* dir = basename(directory); // extracting directory name
    char buffer[512]; memset(buffer, 0, 512);
    char filename[512], *buf1, *buf2;
    FILE *fp;

    while(read(sock, buffer, sizeof(buffer)) > 0) {
        buffer[512] = '\0';
        int occurrence = check_if_directory_or_filename(buffer);
        // buffer received directory
        if( occurrence == DIRECTORY) {
            char dirname[512];
            buf1 = strstr(buffer, dir); //First occurrence of C string <directory> , keep string from <directory> to \0
            strcpy(dirname, buf1);
            printf("Received: %s\n", dirname);
            errno = 0;
            if (mkdir(dirname, S_IRWXU) == -1) {
                switch (errno) {
                    case EEXIST:
                        printf("directory already exists"); exit(0);
                    default:
                        perror("mkdir"); exit(0);
                }
            }
        }
        // buffer received file
        else {
            if(occurrence == FILENAME) {
                buf2 = strstr(buffer, dir); //First occurrence of C string <filename> , keep string from <filename> to \0
                strcpy(filename, buf2);
                printf("Received: %s\n", filename);
                // creating the file
                fp  = fopen (filename, "w");
                if ( fp == NULL ) { /* check for error */
                    printf ("errno = %d \n ", errno);
                    perror ("fopen");
                } 
            }

            // buffer received file contents
            if(occurrence == CONTENTS){
                //printf("In occurance 0 filename:%s buffer:%s\n", filename, buffer);
                fprintf(fp,"%s",buffer);
            }
        }
        memset(buffer, 0, 512);
    }
}
