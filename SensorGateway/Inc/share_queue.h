#ifndef _SHARE_QUEUE_H_
#define _SHARE_QUEUE_H_

typedef struct Queue {
    char** items;
    int size;
    int front, rear;
} Queue;

void initialize(Queue *queue, int size);
int is_empty(Queue queue);
int is_full(Queue queue);
void enqueue(Queue *queue, char *data);
char* dequeue(Queue *queue);

#endif