#include "directory.h"

// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf

void * receive_dir_name(void *argp) {
    struct thread_funct_args *args = (struct thread_funct_args*) argp; // coping passed argument struct
    char buf[512];
    memset(buf, 0, 512);
    int err;
    while(read(args->newsock, buf, sizeof(buf)) > 0) { 
        strcpy(args->directory, buf);
    }
    printf("directory name received by server(dlt: server_funct.c line 13): %s\n", args->directory);
    close(args->newsock);	  /* Close socket */
	recursive_list_dirs(args->directory, &args->queue, args->newsock);
	printf("Thread %ld just before exiting\n", pthread_self());
    pthread_exit(NULL); 
}

void recursive_list_dirs(char dirname[], Queue** queue, int newsock)
{
	printf("Thread %ld in function recursive_list_dirs\n", pthread_self());
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
			
			// item is not a directory
			if(direntp->d_type != DT_DIR) {
				sprintf(buf, "%s/%s", dirname, direntp->d_name); // insert in queue
				//printf("%s\n", buf) ;
				Queue_Push(queue, buf, newsock);
			}
			else { // item is a directory
				char sub_dir[1024];
				sprintf(sub_dir, "%s/%s", dirname, direntp->d_name);
				recursive_list_dirs(sub_dir, queue, newsock);

			}
		}
		closedir(dir_ptr);
	}
}
