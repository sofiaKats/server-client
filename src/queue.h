#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Q_node {
    char filepath[512];
    int socket;
    struct Q_node* next;
} Q_node;

typedef struct Queue{
    int size;
    Q_node* front;
    Q_node* rear;
}Queue;

// Queue initialization and creation
Queue* Create_Queue(void);

// Insert filepath and socket number in queue
void Queue_Push(Queue** queue, char* item, int socket);

// Delete front item from queue
void Queue_Pop(Queue** queue);

// Print elements in queue
void Print_Queue(Queue* queue);

// returns 1 if queue is empty, 0 if queue is NOT empty
int queue_is_empty(Queue* queue);

// returns the number of nodes that exist in the node
int Get_QueueSize(Queue* queue);

#endif