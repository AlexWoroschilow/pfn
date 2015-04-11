#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "int_conv.h"
#include "subsetsum.h"

#define USAGE(name) \
    fprintf(stderr, "USAGE: %s k\n", name); \
    fprintf(stderr, "where k is a positive integer\n"); \
    return EXIT_FAILURE

int main(int argc, char *argv[])
{
  long arg;
  unsigned long k,
                numofvalues,
                testarray[] = {34,   38,  39,  43,  55,  66,  67,  84,  85,  91,
                               101, 117, 128, 138, 165, 168, 169, 182, 184, 186,
                               234, 238, 241, 276, 279, 288, 386, 387, 388, 389,
                               413, 444, 487, 513, 534, 535, 616, 722, 786, 787}
                               ;
  bool result, *mark;

  if (argc != 2) {
    USAGE(argv[0]);
  }
  arg = get_long(argv[1]);
  if (arg < 0) {
    fprintf(stderr, "k is not a positive number\n");
    USAGE(argv[0]);
  }

  k = (unsigned long) arg;

  numofvalues = sizeof testarray/sizeof testarray[0];
  mark = calloc(numofvalues, sizeof *mark);
  result = subsetsum(k, mark, testarray, numofvalues);
  if (result)
  {
    unsigned long idx;

    printf("sum of");
    for (idx = 0; idx < numofvalues; idx++)
    {
      if (mark[idx])
      {
        printf(" %lu",testarray[idx]);
      }
    }
    printf(" is %lu\n",k);
  } else
  {
    printf("no subset of sum %lu found\n",k);
  }
  free(mark);
  return EXIT_SUCCESS;
}
