#include "directory.h"

// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf
// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic6-Threads.pdf

pthread_mutex_t mtx;
pthread_cond_t cond_nonempty;
pthread_cond_t cond_nonfull;

void InitializeCondMtx(void)
{
	pthread_mutex_init(&mtx, 0);
    pthread_cond_init(&cond_nonempty, 0);
    pthread_cond_init(&cond_nonfull, 0);
}

void DestroyCondMtx(void)
{
	pthread_cond_destroy(&cond_nonempty);
    pthread_cond_destroy(&cond_nonfull);
    pthread_mutex_destroy(&mtx);
}

void * receive_dir_name(void *argp) {
    struct thread_funct_args *args = (struct thread_funct_args*) argp; // coping passed argument struct
    char buf[512];
    memset(buf, 0, 512);
    int err;
    if(read(args->newsock, buf, sizeof(buf)) > 0) { 
        strcpy(args->directory, buf);
    }
    //close(args->newsock);	  /* Close socket */
	recursive_list_dirs(args->directory, &args->queue, args->newsock, args->queue_size);
    pthread_exit(NULL); 
}

void recursive_list_dirs(char dirname[], Queue** queue, int newsock, int queue_size)
{
	printf("[Thread: %ld]: About to scan directory %s\n", pthread_self(), dirname);
	DIR 	       *dir_ptr;
	struct 	dirent *direntp;
	char buf[1024];

	if ( ( dir_ptr = opendir( dirname ) ) == NULL ) {
		fprintf(stderr, "cannot open %s \n",dirname);
		exit(1);
	}
	else 
	{
		char tempdir[512]; sprintf(tempdir, "~#@%s", dirname);
		// if(write(newsock, tempdir, strlen(tempdir)) < 0)
		// 			perror_exit("write @ directory.c line 52\n");
		place(tempdir, queue, newsock, queue_size);
        pthread_cond_signal(&cond_nonempty);
		usleep(300000);

		while ( ( direntp=readdir(dir_ptr) ) != NULL )
		{
			if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) continue;
			memset(buf, 0, sizeof(buf));
			
			// item is not a directory
			if(direntp->d_type != DT_DIR) {
				sprintf(buf, "%s/%s", dirname, direntp->d_name); 
				// producer functionality
				place(buf, queue, newsock, queue_size);
            	pthread_cond_signal(&cond_nonempty);
				usleep(300000);
			}
			else { // item is a directory
				char sub_dir[1024];
				sprintf(sub_dir, "%s/%s", dirname, direntp->d_name);
				recursive_list_dirs(sub_dir, queue, newsock, queue_size);

			}
		}
		closedir(dir_ptr);
	}
}

void place(char* filepath, Queue** queue, int newsock, int queue_size) {
    pthread_mutex_lock(&mtx);
    while ( Get_QueueSize(*queue) >= queue_size ) {
        pthread_cond_wait(&cond_nonfull, &mtx);
    }
	printf("[Thread: %ld]: Adding file %s to the queue…\n", pthread_self(), filepath);
	Queue_Push(queue, filepath, newsock);
    pthread_mutex_unlock(&mtx);
}

// worker thread function
void * consumer(void *argp)
{
	//printf("mphka1\n");
	struct thread_funct_args *args = (struct thread_funct_args*) argp;
	do {
		//printf("mphka2\n");
		obtain(&args->queue, args->block_size);
		//printf("mphka3\n");
		pthread_cond_signal(&cond_nonfull);
		//printf("mphka4\n");
		usleep(500000);
	} while(Get_QueueSize(args->queue) > 0);
    pthread_exit(0); // worker thread
}

int obtain(Queue** queue, int block_size) {
    pthread_mutex_lock(&mtx);
    while (Get_QueueSize(*queue) <= 0) {
        pthread_cond_wait(&cond_nonempty, &mtx);
    }
	Q_node* temp = Queue_Top(*queue);
	printf("[Thread: %ld]: Received task: <%s, %d>\n", pthread_self(), temp->filepath, temp->socket);
	char tempfile[512]; sprintf(tempfile, "$^$*%s", temp->filepath);
	if(write(temp->socket, tempfile, strlen(tempfile)) < 0)
		perror_exit("write @ directory.c line 104\n");
	
	// checking if it's a file or directory, if its a directory do nothing
	// if it's a file, open it and send it's contents to client
	if(check_if_directory_or_filename(tempfile) == FILENAME) {
		// File pointer to read from file
		FILE* file = fopen(temp->filepath, "r");
		if (file == NULL) perror_exit("fopen @ directory.c line 118\n");
 
		char string[block_size];
		// Read contents from file
		while (fgets(string, block_size, file) != NULL) {
        	if(write(temp->socket, string, strlen(string)) < 0)
				perror_exit("write @ directory.c line 126\n");
    	}	
		fclose(file);
	}
	Queue_Pop(queue);
    pthread_mutex_unlock(&mtx);
}
