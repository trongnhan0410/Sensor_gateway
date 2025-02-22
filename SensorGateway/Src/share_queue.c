#include <stdio.h>
#include <pthread.h>
#include "share_queue.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void initialize(Queue *queue, int size){
    queue->items = (char**) malloc(sizeof(char)* size);
    queue->front = -1;
    queue->rear = -1;
    queue->size = size;
}

int is_empty(Queue queue){
    return queue.front == -1;
}

int is_full(Queue queue){
    return (queue.rear + 1) % queue.size == queue.front;
}

void enqueue(Queue *queue, char *data){
    pthread_mutex_lock(&mutex);
    if (!is_full(*queue)) {
        if (is_empty(*queue)) {
            queue->front = queue->rear = 0;
        } else {
            queue->rear = (queue->rear + 1) % queue->size;
        }
        queue->items[queue->rear] = strdup(data);
    } else {
        printf("Queue overflow\n");
    }
    pthread_mutex_unlock(&mutex);
}

char* dequeue(Queue *queue){
    pthread_mutex_lock(&mutex);
    if (!is_empty(*queue)) {
        char *dequeued_value = queue->items[queue->front]; 
        if (queue->front == queue->rear) {
            queue->front = queue->rear = -1;
        } else {
            queue->front = (queue->front + 1) % queue->size;
        }
        pthread_mutex_unlock(&mutex);
        return dequeued_value;  
    } else {
        printf("Queue underflow\n");
        pthread_mutex_unlock(&mutex);
        return NULL; 
    }
}
