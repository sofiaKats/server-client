#include "directory.h"

// code used from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf

pthread_mutex_t mtx;
pthread_cond_t cond_nonempty;
pthread_cond_t cond_nonfull;
// since i am creating the consumer first i want to make sure direntp
// was used at least once by producer(communication thread) so that 
// the consumer can enter while loop and the program doesn't freeze
struct 	dirent *direntp;

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
    while(read(args->newsock, buf, sizeof(buf)) > 0) { 
        strcpy(args->directory, buf);
    }
    //printf("directory name received by server(dlt: server_funct.c line 13): %s\n", args->directory);
    close(args->newsock);	  /* Close socket */
	recursive_list_dirs(args->directory, &args->queue, args->newsock, args->queue_size);
    pthread_exit(NULL); 
}

void recursive_list_dirs(char dirname[], Queue** queue, int newsock, int queue_size)
{
	printf("Thread: %ld]: About to scan directory %s\n", pthread_self(), dirname);
	DIR 	       *dir_ptr;
	char buf[1024];

	if ( ( dir_ptr = opendir( dirname ) ) == NULL ) {
		fprintf(stderr, "cannot open %s \n",dirname);
		exit(1);
	}
	else 
	{
		while ( ( direntp=readdir(dir_ptr) ) != NULL )
		{
			//printf("IN @ recursive producer direntp_in_usage=%d\n",flag);
			if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) continue;
			memset(buf, 0, sizeof(buf));
			
			// item is not a directory
			if(direntp->d_type != DT_DIR) {
				sprintf(buf, "%s/%s", dirname, direntp->d_name); // insert in queue
				//printf("%s\n", buf) ;
				//Queue_Push(queue, buf, newsock);
				// producer functionality
				place(buf, queue, newsock, queue_size);
            	pthread_cond_signal(&cond_nonempty);
				//usleep(300000);
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
	//Print_Queue(*queue);
	printf("[Thread: %ld]: Adding file %s to the queue…\n", pthread_self(), filepath);
	Queue_Push(queue, filepath, newsock);
    pthread_mutex_unlock(&mtx);
}

// worker thread function
void * consumer(void *argp)
{
	struct thread_funct_args *args = (struct thread_funct_args*) argp;
    //while(Get_QueueSize(args->queue) >= 0) {
	do {
		obtain(&args->queue);
		pthread_cond_signal(&cond_nonfull);
		usleep(500000);
	} while(Get_QueueSize(args->queue) > 0);
    pthread_exit(0); // worker thread
}

int obtain(Queue** queue) {
    pthread_mutex_lock(&mtx);
    while (Get_QueueSize(*queue) <= 0) {
        pthread_cond_wait(&cond_nonempty, &mtx);
    }
	Q_node* temp = Queue_Top(*queue);
	//Print_Queue(*queue);
	printf("[Thread: %ld]: Received task: <%s, %d>\n", pthread_self(), temp->filepath, temp->socket);
	Queue_Pop(queue);
    pthread_mutex_unlock(&mtx);
}