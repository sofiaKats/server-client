#include "queue.h"

Queue* Create_Queue(void)
{
    // Creation of queue
	Queue * queue = malloc(sizeof(Queue));	
	
	if(queue == NULL){
		printf("failed to create queue");
		return NULL;
	}

    // allocation of memory for first and last node of deque
    queue->front = malloc(sizeof(Q_node));
    queue->rear = malloc(sizeof(Q_node));
    pthread_mutex_init(&(queue->mutex), 0);

    // Initialization of empty data members
    queue->size = 0;
    queue->front = NULL;
    queue->rear = NULL;
	
	return queue;
}

void Queue_Push(Queue** queue, char* item, int socket)
{
    pthread_mutex_lock(&((*queue)->mutex));

    // Creation of newnode
	Q_node * NewNode;
	NewNode = malloc(sizeof(Q_node));
    // copy filepath to node
	memset(NewNode->filepath, 0, 512);
    strcpy(NewNode->filepath, item);
    // insert socket to node
    NewNode->socket = socket;
	NewNode->next = NULL;
	
	(*queue)->size++;
	
    // if queue is empty
	if((*queue)->front == NULL){
		(*queue)->front = (*queue)->rear = NewNode;
		
	}else{ // queue has elements
        (*queue)->front->next = NewNode;
		(*queue)->front = NewNode;	
	}
    pthread_mutex_unlock(&((*queue)->mutex));
}


void Queue_Pop(Queue** queue)
{// Queue follows the order of First In First Out (FIFO)
    pthread_mutex_lock(&((*queue)->mutex));
    if((*queue)->rear == NULL) {
        //printf("Queue already empty, nothing to delete here.\n");
        pthread_mutex_unlock(&((*queue)->mutex));
        return;
    }

    Q_node* temp;
    temp = (*queue)->rear;

    (*queue)->rear = (*queue)->rear->next;
    (*queue)->size--;
    if((*queue)->size == 0) (*queue)->rear = (*queue)->front = NULL;
    free(temp);
    pthread_mutex_unlock(&((*queue)->mutex));
}

Q_node* Queue_Top(Queue* queue)
{
    return queue->rear;
}

int queue_is_empty(Queue* queue)
{
    return (queue->rear == NULL);
}

int Get_QueueSize(Queue* queue) {
    return queue->size;
}


void Print_Queue(Queue* queue)
{
    if(queue->rear == NULL) {
        printf("Queue is empty, nothing to print here.\n");
        return;
    }

    Q_node *current;
	current = queue->rear;

	while(current != NULL){
		printf("< %s, %d >",current->filepath, current->socket);
		if(current->next != NULL) {
			printf(",");
		}
		current = current->next;
		
	}
	printf("\n");
}