#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#define NUMVALUES (1 << 20)
/* 128 */
#define NUMSORTS (1<<7)

static int compare(const void *a, const void *b)
{
  return *((int *) a) - *((int *) b);
}

static int timeval_subtract(struct timeval *result,
                            struct timeval *x,
                            struct timeval *y)
{
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;
  return x->tv_sec < y->tv_sec;
}

static int timeval_add(struct timeval *result,
                       const struct timeval *x,
                       const struct timeval *y)
{
  result->tv_sec = x->tv_sec + y->tv_sec;
  result->tv_usec = x->tv_usec + y->tv_usec;
  while (result->tv_usec > 1000000) {
    result->tv_usec -= 1000000;
    result->tv_sec++;
  }
  return 0;
}

int main(void)
{
  int *values = malloc(NUMVALUES * sizeof (*values));
  unsigned long idx, sortnum;
  struct timeval start, end, diff, tmp, runtime = {0,0};

  assert(values != NULL);
  for (sortnum = 0; sortnum < NUMSORTS; ++sortnum) {
    for (idx = 0; idx < NUMVALUES; ++idx) {
      values[idx] = rand();
    }
    (void) gettimeofday(&start, NULL);
    qsort(values, sizeof(*values), NUMVALUES, compare);
    (void) gettimeofday(&end, NULL);
    (void) timeval_subtract(&diff, &end, &start);
    tmp = runtime;
    (void) timeval_add(&runtime, &tmp, &diff);
    /* printf("Runtime (hh:mm:ss:us)\t%02lu:%02lu:%02lu:%06lu\n",
           runtime.tv_sec / 60LU / 60LU,
           (runtime.tv_sec / 60LU) % 60LU,
           runtime.tv_sec % 60LU,
           runtime.tv_usec); */
  }

  printf("Runtime (hh:mm:ss:us)\t%02lu:%02lu:%02lu:%06lu\n",
         runtime.tv_sec / 60LU / 60LU,
         (runtime.tv_sec / 60LU) % 60LU,
         runtime.tv_sec % 60LU,
         runtime.tv_usec);

  free(values);
  return 0;
}
