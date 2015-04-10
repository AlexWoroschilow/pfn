#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

#define RANDOMSEED 777L


int main(int argc, char **argv)
{
  unsigned long idx;
  unsigned int max;
  Queue *queue;

  if (argc != 2) {
    printf("USAGE: %s <positive number>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (1 != sscanf(argv[1], "%u", &max)) {
    printf("can't extract number from input: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  srand48(RANDOMSEED);

  queue = queue_new(10);

  for (idx=1; idx < 66; idx++) {
    if (idx%3) {
      int val = (int) max * drand48();
      queue_enqueue(queue, val);
    }
    else {
      printf("dequed: %d\n", queue_dequeue(queue));
      queue_print(queue);
    }
  }
  printf("queue as it is now:\n");
  queue_print(queue);
  queue_delete(queue);

  exit(EXIT_SUCCESS);
}
