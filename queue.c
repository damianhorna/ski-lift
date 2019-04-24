#include "queue.h"

int isEmpty(Queue *queue) { return (queue->size == 0); }

struct Queue *createQueue(QueueElement *firstElement) {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->head = firstElement;
    queue->size = 1;
    return queue;
}

void enqueue(Queue *queue, QueueElement *queueElement) {
    if (queue->size == 0) {
        queue->head = queueElement;
        queue->size = 1;
    } else {
        QueueElement *elem = queue->head;
        while (elem->next != NULL) {
            elem = elem->next;
        }
        elem->next = queueElement;
        queue->size = queue->size + 1;
        printf("element enqueued to queue\n");
    }

}

struct QueueElement *dequeue(Queue *queue) {
    if (isEmpty(queue))
        return NULL;

    QueueElement *item = queue->head;
    queue->head = item->next;
    queue->size = queue->size - 1;
    return item;
}

QueueElement *createRequest(int id, int time, int weight) {
    QueueElement *element = (QueueElement *) malloc(sizeof(QueueElement));
    element->id = id;
    element->time = time;
    element->weight = weight;
    return element;
}

void printQueue(Queue *queue) {
    printf("*** QUEUE PRINT ***\n");
    if (!isEmpty(queue)) {
        QueueElement *elem = queue->head;
        printf("ID: %d, Time: %d, Weight: %d\n", elem->id, elem->time, elem->weight);
        while (elem->next != NULL) {
            elem = elem->next;
            printf("ID: %d, Time: %d, Weight: %d\n", elem->id, elem->time, elem->weight);
        }
        printf("*** END PRINT ***\n");
    } else {
        printf("queue empty!\n");
    }
}

void swap(QueueElement *a, QueueElement *b) {
    //DONT JUDGE ME OK
    int temp = a->id;
    a->id = b->id;
    b->id = temp;

    temp = a->weight;
    a->weight = b->weight;
    b->weight = temp;

    temp = a->time;
    a->time = b->time;
    b->time = temp;
}

void queueBubbleSort(Queue *queue) {
    if (queue->size >= 2) {
        int swapped;
        QueueElement *ptr1;
        QueueElement *lptr = NULL;
        do {
            swapped = 0;
            ptr1 = queue->head;

            while (ptr1->next != lptr) {
                if (ptr1->time > ptr1->next->time || (ptr1->time == ptr1->next->time && ptr1->id > ptr1->next->id)) {
                    swap(ptr1, ptr1->next);
                    swapped = 1;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }
}