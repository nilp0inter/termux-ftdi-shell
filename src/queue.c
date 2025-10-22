#include "queue.h"
#include <string.h>

Queue* queue_create() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if (q) {
        q->head = q->tail = NULL;
    }
    return q;
}

void queue_destroy(Queue *q) {
    if (!q) return;
    while (q->head) {
        QueueNode *temp = q->head;
        q->head = q->head->next;
        free(temp->data);
        free(temp);
    }
    free(q);
}

int queue_enqueue(Queue *q, const unsigned char *data, size_t len) {
    QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) return -1;

    newNode->data = (unsigned char*)malloc(len);
    if (!newNode->data) {
        free(newNode);
        return -1;
    }

    memcpy(newNode->data, data, len);
    newNode->len = len;
    newNode->next = NULL;

    if (q->tail) {
        q->tail->next = newNode;
    }
    q->tail = newNode;

    if (!q->head) {
        q->head = newNode;
    }

    return 0;
}

QueueNode* queue_dequeue(Queue *q) {
    if (!q->head) return NULL;

    QueueNode *node = q->head;
    q->head = q->head->next;

    if (!q->head) {
        q->tail = NULL;
    }

    return node;
}
