#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void place(pool_t * pool, int data) {
    pthread_mutex_lock(&mtx);
    while (queue.size() >= qsizeargv ) {
        pthread_cond_wait(&cond_nonfull, &mtx);
    }
    queue_push(queue, temp_queue.filename , temp_queue.newsock);
    pthread_mutex_unlock(&mtx);
}

int obtain(pool_t * pool) {
    pthread_mutex_lock(&mtx);
    while (queue_size() <= 0) {
        pthread_cond_wait(&cond_nonempty, &mtx);
    }
    pop_queue(queue);
    pthread_mutex_unlock(&mtx);
}

// cheat: create a vector to keep all files of dir a priori
// then by the given size just add them to the list
void * producer(void * ptr, queue, buf, newsock)
{       
        while(item = (temp)queue_pop) {

            place(&pool, item, buf, newsock);
            pthread_cond_signal(&cond_nonempty);    
        }
        pthread_exit(0); // communication thread
}

// worker thread function
void * consumer(void * ptr)
{
    while( 1 ) {
        obtain(&pool, queue);
        pthread_cond_signal(&cond_nonfull);
    }
    pthread_exit(0); // worker thread
}

int main(){
        pthread_t cons, prod;

        initialize(&pool);
        pthread_mutex_init(&mtx, 0);
        pthread_cond_init(&cond_nonempty, 0);
        pthread_cond_init(&cond_nonfull, 0);

        // creation of communication thread
        pthread_t communication_thread;
        int err, status;
        struct thread_funct_args args;  // passing struct of arguments to thread creation function
        args.newsock  = newsock;
        args.temp_queue = temp_queue;
        memset(args.directory, 0, 512); //directory name written by client will be stored here
        if (err = pthread_create(&communication_thread, NULL, producer, (void *) &args)) { /* New thread */
            perror2("pthread_create @ server.c line 78", err);  exit(1);}

        if (err = pthread_join(communication_thread, NULL/*, (void **) &status*/)) { /* Wait for thread */
            perror2("pthread_join @ server.c line 83", err); /* termination */ exit(1);}

        int n, i, sl, err;
        pthread_t *worker_threads;
       
        if ((worker_threads = malloc(pool * sizeof(pthread_t))) == NULL) {
            perror("malloc");  exit(1); }

        for (i=0 ; i<pool ; i++) {
            if (err = pthread_create(worker_threads+i, NULL, consumer, (void *) args)) {
                perror2("pthread_create", err);   exit(1);} 
        }

        for (i=0 ; i<n ; i++) {
            if (err = pthread_join(*(worker_threads+i), NULL)) {
                perror2("pthread_join", err); exit(1);}
        }

        pthread_cond_destroy(&cond_nonempty);
        pthread_cond_destroy(&cond_nonfull);
        pthread_mutex_destroy(&mtx);
        return 0;
}