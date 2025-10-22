#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

typedef struct QueueNode {
    unsigned char *data;
    size_t len;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *head;
    QueueNode *tail;
} Queue;

Queue* queue_create(void);
void queue_destroy(Queue *q);
int queue_enqueue(Queue *q, const unsigned char *data, size_t len);
QueueNode* queue_dequeue(Queue *q);

#endif // QUEUE_H
