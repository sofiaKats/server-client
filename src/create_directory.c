#include "create_directory.h"

void receive_filenames(char* directory, int sock) {
    char* dir = basename(directory); // extracting directory name
    char buffer[512]; memset(buffer, 0, 512);

    while(read(sock, buffer, sizeof(buffer)) > 0) {
        buffer[512] = '\0';
        // buffer received directory
        if(sanitize(buffer)) {
            char* dirname = strstr(buffer, dir); //First occurrence of C string <directory> , keep string from <directory> to \0
            printf("client received directory: %s\n", dirname);
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
            char* filename = strstr(buffer, dir); //First occurrence of C string <directory> , keep string from <directory> to \0
            printf("client received: %s\n", filename);
            FILE *fp;
            fp  = fopen (filename, "w");
            if ( fp == NULL ) { /* check for error */
                printf ("errno = %d \n ", errno);
                perror ("fopen");
            } 
        }
        
        memset(buffer, 0, 512);
    }
}

int sanitize(char *str)
{
    char *src, *dest;
	for ( src = dest = str ; *src ; src++ )
    {
        if (*src == '#') {
            printf("client received dir: %s\n", src);
            return 1;
        }
		*dest++ = *src;
    }
	*dest = '\0';
    return 0;
}

// char * basename (const char *filename)
// {
//   char *p = strrchr (filename, '/');
//   return p ? p + 1 : (char *) filename;
// }

// void sanitize(char *str)
// {
// 	char *src, *dest;
// 	for ( src = dest = str ; *src ; src++ )
//     {
//         if (*src == '#') printf("client received dir: %s\n", src);
//         if (*src == '_' || *src == '-' || isalnum(*src) ) //acceptable chars for dirname
// 			*dest++ = *src;
//     }
// 	*dest = '\0';
// }
