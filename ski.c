#include "mpi.h"
#include <stdio.h>
#include <stdlib.h> 
#include <limits.h> 

struct Queue 
{ 
    int front, rear, size; 
    unsigned capacity; 
    int* array; 
}; 
  
// function to create a queue of given capacity.  
// It initializes size of queue as 0 
struct Queue* createQueue(unsigned capacity) 
{ 
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue)); 
    queue->capacity = capacity; 
    queue->front = queue->size = 0;  
    queue->rear = capacity - 1;  // This is important, see the enqueue 
    queue->array = (int*) malloc(queue->capacity * sizeof(int)); 
    return queue; 
} 
  
// Queue is full when size becomes equal to the capacity  
int isFull(struct Queue* queue) 
{  return (queue->size == queue->capacity);  } 
  
// Queue is empty when size is 0 
int isEmpty(struct Queue* queue) 
{  return (queue->size == 0); } 
  
// Function to add an item to the queue.   
// It changes rear and size 
void enqueue(struct Queue* queue, int item) 
{ 
    if (isFull(queue)) 
        return; 
    queue->rear = (queue->rear + 1)%queue->capacity; 
    queue->array[queue->rear] = item; 
    queue->size = queue->size + 1; 
    printf("%d enqueued to queue\n", item); 
} 
  
// Function to remove an item from queue.  
// It changes front and size 
int dequeue(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return INT_MIN; 
    int item = queue->array[queue->front]; 
    queue->front = (queue->front + 1)%queue->capacity; 
    queue->size = queue->size - 1; 
    return item; 
} 
  
// Function to get front of queue 
int front(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return INT_MIN; 
    return queue->array[queue->front]; 
} 
  
// Function to get rear of queue 
int rear(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return INT_MIN; 
    return queue->array[queue->rear]; 
} 

int main( int argc, char **argv )
{
	int rank, size;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int namelen;

	MPI_Init( &argc, &argv );
	
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Get_processor_name(processor_name,&namelen);
	printf( "Jestem %d z %d na %s\n", rank, size, processor_name );

	MPI_Finalize();
}
