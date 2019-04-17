#include "mpi.h"
#include <stdio.h>
#include <stdlib.h> 
#include <limits.h> 

typedef struct QueueElement
{
	int id;
	int time;
	int weight;
	struct QueueElement *previous;
	struct QueueElement *next;
} QueueElement;

typedef struct Queue 
{ 
	QueueElement *head;
	int size;
}Queue; 

struct Queue* createQueue(QueueElement* firstElement) 
{ 
	Queue* queue = (Queue*) malloc(sizeof(Queue)); 
	queue->head = firstElement; 
	queue->size = 1;  
	return queue; 
} 
  
int isEmpty(Queue* queue) 
{  return (queue->size == 0); } 
  
void enqueue(Queue* queue, QueueElement* queueElement) 
{ 
	QueueElement *elem = queue->head;
	while(elem->next != NULL)
	{
		elem = elem->next;
	}
	elem->next = queueElement;
	queue->size = queue->size + 1; 
	printf("element enqueued to queue\n"); 
} 
  
struct QueueElement* dequeue(Queue* queue) 
{ 
	if (isEmpty(queue)) 
		return NULL; 

	QueueElement* item = queue->head;
	queue->head = item->next;
	queue->size = queue->size - 1; 
	return item; 
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
