/*
 ============================================================================
 Name        : queue.c
 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : Queue with array loop
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "queue.h"

/* Check conditions and return
 a message if fail */
#define assert_with_message(condition, message) \
  if(!(condition)) \
  { \
    fprintf(stderr, message); \
    exit(EXIT_FAILURE); \
  }

/* Allocate a memory and display
 a message if not success */
#define malloc_or_exit(pointer, size, message)\
  assert_with_message((pointer = malloc(size)) != NULL, message);\

/* Reallocate a memory and display
 a message if not success */
#define realloc_or_exit(pointer, size, message)\
  assert_with_message((pointer = realloc(pointer, size)) != NULL, message);\

/* Check is queue valid */
#define queue_validate(queue) \
  assert_with_message(queue != NULL, "Queue size can not be 0");\
  assert_with_message(queue->queuespace != NULL, "Queue size can not be 0");\
  assert_with_message(queue->queuesize > 0, "Queue size can not be 0");\

/* Check that indexes are not equal */
#define queue_validate_indexes(queue) \
    assert_with_message(queue->enqueueindex != queue->dequeueindex, "Enqueue index and dequeue index can not be equal");\


/* The following function delivers an empty queue with a reservoir of
 size elements to be stored in the queue. The reservoir can, if
 necessary, be enlarged. */
Queue *queue_new(unsigned long queuesize) {

  Queue *q = NULL;

  malloc_or_exit(q, sizeof(Queue), "Can not allocate memory for queue");

  q->queuesize = queuesize;
  q->enqueueindex = 0UL;
  q->dequeueindex = 0UL;
  q->no_of_elements = 0UL;
  q->queuespace = NULL;

  malloc_or_exit(q->queuespace, queuesize * sizeof(Queueelement),
      "Can not allocate memory for queue elements");

  return q;
}

/* The following function returns true if the queue is empty. */
bool queue_is_empty(const Queue *q) {

  queue_validate(q);

  return (q->no_of_elements == 0);
}

/* The following function resizes the queue by doubling the space reservoir  */
void queue_double_size(Queue *q) {
  /* This is a public function, in case of
   external usage (not from queue_enqueue) needs
   to validate given queue */
  queue_validate(q);

  realloc_or_exit(q->queuespace, (q->queuesize * sizeof(Queueelement)) * 2,
      "Can not reallocate memory for queue elements");

  q->queuesize *= 2;
}

/* The following function adds an element elem to the end of the queue. */
void queue_enqueue(Queue *q, Queueelement elem) {

  queue_validate(q);

  /* Check for start and end points intersection
   increase queue size in this case */
  if (abs(q->enqueueindex - q->dequeueindex) == 1) {
    queue_double_size(q);
  }

  q->queuespace[q->enqueueindex] = elem;

  q->enqueueindex++;
  if (q->enqueueindex == q->queuesize) {
    q->enqueueindex = 0;
  }

  q->no_of_elements++;

  /* check that indexes are not equal */
  queue_validate_indexes(q);
}

/* The following function removes the element elem from the start of the queue. */
Queueelement queue_dequeue(Queue *q) {

  queue_validate(q);

  Queueelement e = q->queuespace[q->dequeueindex];

  q->dequeueindex++;
  if (q->dequeueindex == q->queuesize) {
    q->dequeueindex = 0;
  }

  q->no_of_elements--;

  /* check that indexes are not equal */
  queue_validate_indexes(q);

  return e;
}

/* print the contents of <*q> on screen */
void queue_print(const Queue *q) {

  bool printed = false;
  unsigned long enqueueindex = q->enqueueindex;

  queue_validate(q);

  printf("queue=\n");
  if ((q->no_of_elements > 0)) {
    while (q->dequeueindex != enqueueindex) {

      if (enqueueindex == 0) {
        enqueueindex = q->queuesize;
      }

      enqueueindex--;

      printf((printed ? "  %d" : "%d"), q->queuespace[enqueueindex]);

      if (!printed) {
        printed = true;
      }

    }
  }

  printf("\n");
}

/* The following function frees the space required for the queue. */
void queue_delete(Queue *q) {

  queue_validate(q);

  free(q->queuespace);
  free(q);
}

