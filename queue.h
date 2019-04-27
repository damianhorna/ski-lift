#ifndef SKI_LIFT_QUEUE_H
#define SKI_LIFT_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueElement {
    int id;
    int time;
    int weight;
    struct QueueElement *next;
} QueueElement;

typedef struct Queue {
    QueueElement *head;
    int size;
} Queue;


int isEmpty(Queue *queue);

struct Queue *createQueue(QueueElement *firstElement);

void enqueue(Queue *queue, QueueElement *queueElement);

struct QueueElement *dequeue(Queue *queue);

QueueElement *createRequest(int id, int time, int weight);

void printQueue(Queue *queue);

void swap(QueueElement *a, QueueElement *b);

void queueBubbleSort(Queue *queue);

#endif //SKI_LIFT_QUEUE_H
