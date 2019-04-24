// #include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

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

struct Queue *createQueue(QueueElement *firstElement) {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->head = firstElement;
    queue->size = 1;
    return queue;
}

int isEmpty(Queue *queue) { return (queue->size == 0); }

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
                if (ptr1->time > ptr1->next->time || ptr1->id > ptr1->next->id) {
                    swap(ptr1, ptr1->next);
                    swapped = 1;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);

//        while (leftElement->next != NULL) {
//            leftElement = leftElement->next;
//        }
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    printf("main\n");
    int rank, size;
    // char processor_name[MPI_MAX_PROCESSOR_NAME];
    int namelen;
    QueueElement *element = createRequest(1, 2, 3);
    Queue *queue = createQueue(element);

    for (int i = 2; i <= 10; i++) {
        QueueElement *secondElement = createRequest(i, rand() % 50, rand() % 70 + 50);
        enqueue(queue, secondElement);
    }


    printf("Queue size: %d\n", queue->size);
    printQueue(queue);
    queueBubbleSort(queue);
    printQueue(queue);

    // MPI_Init( &argc, &argv );

    // MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    // MPI_Comm_size( MPI_COMM_WORLD, &size );
    // MPI_Get_processor_name(processor_name,&namelen);
    // printf( "Jestem %d z %d na %s\n", rank, size, processor_name );

    // MPI_Finalize();
}
