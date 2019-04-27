// #include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "queue.h"

int main(int argc, char **argv) {
    srand(time(NULL));
    printf("main\n");
    int rank, size;
    // char processor_name[MPI_MAX_PROCESSOR_NAME];
    int namelen;
    QueueElement *element = createRequest(1, 2, 3);
    Queue *queue = createQueue(element);

    for (int i = 2; i <= 30; i++) {
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
