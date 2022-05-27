#include "server_functs.h"

// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf

void * receive_dir_name(void *argp) {
    struct thread_funct_args *args = (struct thread_funct_args*) argp; // coping passed argument struct
    char buf[512];
    memset(buf, 0, 512);
    int err;
    printf("Given directory name is: ");
    while(read(args->newsock, buf, sizeof(buf)) > 0) { 
        strcpy(args->directory, buf);
    }
    printf("directory name received by server(dlt: server_funct.c line 13): %s\n", args->directory);
    printf("\nClosing connection.\n");
    close(args->newsock);	  /* Close socket */
    pthread_exit(NULL); 
}

void recursive_list_dirs(char dirname[])
{
	DIR 	       *dir_ptr;
	struct 	dirent *direntp;
	char buf[1024];

	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
			fprintf(stderr, "cannot open %s \n",dirname);
	else 
	{
		while ( ( direntp=readdir(dir_ptr) ) != NULL )
		{
			if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) continue;
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%s", direntp->d_name);
			printf("%s\n", direntp->d_name) ;
			if(direntp->d_type == DT_DIR) { // item is a directory
				char sub_dir[1024];
				sprintf(sub_dir, "%s/%s", dirname, direntp->d_name);
				recursive_list_dirs(sub_dir);

			}
		}
		closedir(dir_ptr);
	}
}

