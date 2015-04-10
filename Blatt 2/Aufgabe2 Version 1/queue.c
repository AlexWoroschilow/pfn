/*Tim Rolff, Oleksand Voroshylov*/
/*
 ==========================================================================
 Name        : queue.c
 Author      : Tim Rolff, Oleksand Voroshylov
 Version     : 1.0
 Copyright   : 2015
 Description : a simple queue implementation with a ring buffer
 ==========================================================================
 */

#include "queue.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>

/*
Allokiert speicher und prueft ihn direkt.
Gibt im fehlerfall einen fehler aus und beendet
das Program
*/
#define SAVEALLOC(X, SIZE, STR)\
  X = malloc(SIZE); \
  if(!(X)) \
  { \
    fprintf(stderr, STR); \
    exit(EXIT_FAILURE); \
  }

/*
Einfaches assert makro
*/
#define ASSERT(X, STR) \
  if(!(X)) \
  { \
    fprintf(stderr, STR); \
    exit(EXIT_FAILURE); \
  }

/*
Default Assert fuer die queue
*/
#define QUEUECHECK(Q) \
  ASSERT(Q != NULL, "The given queue should not be NULL\n"); \
  ASSERT(Q->queuespace != NULL, "The queue elements should not be NULL\n"); \
  ASSERT(Q->queuesize > 0, "The queue size must be bigger than zero\n");

Queue *queue_new(unsigned long queuesize)
{
  Queue* q = NULL;
  ASSERT(queuesize > 0,
         "The parameter queuesize must be greater than zero\n");
  SAVEALLOC(q, sizeof(Queue),
            "Could not allocate space for a new queue\n");
  SAVEALLOC(q->queuespace, queuesize * sizeof(Queueelement),
            "Could not allocate space for the queue elements\n");

  q->enqueueindex   = 0;
  q->dequeueindex   = 0;
  q->queuesize      = queuesize;
  q->no_of_elements = 0;
  return q;
}

bool queue_is_empty(const Queue *q)
{
  QUEUECHECK(q);
  return q->no_of_elements == 0;
}

void queue_double_size(Queue *q)
{
  QUEUECHECK(q);
  q->queuespace = realloc(q->queuespace,
                          sizeof(Queueelement) * q->queuesize * 2);
  ASSERT(q->queuespace != NULL, "Could not resize the queue\n");
  /*
  Den speicher in den neu allocierten bereich verschieben,
  damit luecken innerhalb der queue vermieden werden und
  wieder q->enqueueindex != q->dequeueindex gilt.
  Bsp:

  Element j einfuegen und queue voll:
  Queue: a b c d e f g h i

  a b c d e f g h i >j<

  Damit wuerde gelten
  a/j b c d e f g h i
  ^ Start und Ende

  Daher erst allokieren:
  => a b c d e f g h i 0 0 0 0 0 0 0 0 0
     ^ Start         ^ Ende
  Dann kopieren:
  => a b c d e f g h i a b c d e f g h i
                       ^ Start         ^ Ende
  und zum Schluss element einfuegen:
  => j b c d e f g h i a b c d e f g h i
     ^ Ende            ^ Start
  */
  memcpy(q->queuespace + q->queuesize, q->queuespace,
         q->no_of_elements * sizeof(Queueelement));
  q->queuesize *= 2;
  q->dequeueindex += q->no_of_elements; //index auch verschieben
}

void queue_enqueue(Queue *q, Queueelement elem)
{
  QUEUECHECK(q);
  //falls zu wenig speicher neuen allokieren
  if(q->queuesize == q->no_of_elements)
  {
    queue_double_size(q);
  }

  q->no_of_elements++;
  q->queuespace[q->dequeueindex] = elem;
  //modulo aufgrund von ring buffer impl.
  q->dequeueindex = (q->dequeueindex + 1) % q->queuesize;
}

Queueelement queue_dequeue(Queue *q)
{
  Queueelement elem;
  QUEUECHECK(q);
  ASSERT(q->no_of_elements > 0,
         "The queue is empty so the dequeue operation "
         "is not possible");

  elem = q->queuespace[q->enqueueindex];
  q->enqueueindex = (q->enqueueindex + 1) % q->queuesize;
  q->no_of_elements--;
  return elem;
}

void queue_print(const Queue *q)
{
  unsigned long i = 0;
  unsigned long idx = 0;
  QUEUECHECK(q);
  printf("queue=\n");
  for(i = 0; i < q->no_of_elements - 1; ++i)
  {
    idx = (q->enqueueindex + (q->no_of_elements - i) - 1) % q->queuesize;
    printf("%d  ", q->queuespace[idx]);
  }

  /*
  letztes element seperat ausgeben, damit keine zwei whitespaces
  entstehen.
  */
  printf("%d\n", q->queuespace[q->enqueueindex]);
}

void queue_delete(Queue *q)
{
  free(q->queuespace);
  free(q);
}


