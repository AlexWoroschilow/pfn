#include <stdbool.h>

typedef int Queueelement;

typedef struct {
  Queueelement *queuespace;     /* the space to store the queue elements */
  unsigned long enqueueindex,   /* points to entry into which element is to be
                                   enqueud */
                dequeueindex,   /* last element of queue */
                queuesize,      /* size of the queue (max no_of_elements)*/
                no_of_elements; /* no of elements between
                                   enqueueindex+1 and dequeindex */
} Queue;

/* The following function delivers an empty queue with a reservoir of
   size elements to be stored in the queue. The reservoir can, if
   necessary, be enlarged. */
Queue *queue_new(unsigned long queuesize);

/* The following function returns true iff the queue is empty. */
bool queue_is_empty(const Queue *q);

/* The following function resizes the queue by doubling the space reservoir  */
void queue_double_size(Queue *q);

/* The following function adds an element elem to the end of the queue. */
void queue_enqueue(Queue *q, Queueelement elem);

/* The following function removes the element elem from the start of the queue.
   */
Queueelement queue_dequeue(Queue *q);

/* print the contents of <*q> on screen */
void queue_print(const Queue *q);

/* The following function frees the space required for the queue. */
void queue_delete(Queue *q);
