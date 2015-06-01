#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

#include "macros.h"
#include "multiseq.h"
#include "bestkvals.h"
#include "unit_edist.h"

static int compare(const void *x, const void *y) {
  const MultiseqPaar * paar1 = (const MultiseqPaar *) x;
  const MultiseqPaar * paar2 = (const MultiseqPaar *) y;

  if (paar1->distance < paar2->distance) {
    return -1;
  } else if (paar1->distance > paar2->distance) {
    return 1;
  } else if (paar1->id1 < paar2->id1) {
    return -1;
  } else if (paar1->id1 > paar2->id1) {
    return 1;
  } else if (paar1->id2 < paar2->id2) {
    return -1;
  } else if (paar1->id2 > paar2->id2) {
    return 1;
  }
  return 0;
}

static void eval_seqrange(Multiseq * multiseq, unsigned long i, unsigned long j,
    EvalMultiseq compare) {

  unsigned long p, q;

  const unsigned long n = muliseq_items(multiseq);
  for (p = i; p <= j; p++) {
    for (q = p + 1; q < n; q++) {

      const MultiseqItem * item1 = muliseq_item(multiseq, p);
      const MultiseqItem * item2 = muliseq_item(multiseq, q);

      const unsigned long distance = compare(
          (const unsigned char *) item1->start, item1->length,
          (const unsigned char *) item2->start, item2->length);

      muliseq_fill_paar(multiseq, distance, p, q);
    }
  }
}

void * eval_seqrange_thread(void * object) {
  MultiseqTread * thread = (MultiseqTread *) object;
  eval_seqrange(thread->multiseq, thread->i, thread->j, thread->compare);
  return object;
}

unsigned long calculate_start(unsigned long t, unsigned long i,
    unsigned long rest, unsigned long addition) {
  unsigned long a;
  a = rest - (rest / (t - i));
  a -= a > addition ? addition : a;
  return a;
}

unsigned long calculate_addition(unsigned long t, unsigned long i,
    unsigned long rest) {
  return (rest / (t - i)) * 0.4;
}

unsigned long calculate_rest(unsigned long start) {
  return start > 0 ? start - 1 : 0;
}

/**
 * Main function with all logic
 * deal with threads and other things
 */
void process(const char * filename, unsigned long k, unsigned long t) {

  unsigned long n, i, start, end, rest, addition;

  Multiseq * multiseq = muliseq_new(filename);
  BestKVals * kval = best_k_vals_new(k, compare, sizeof(MultiseqPaar));
  MultiseqTreadSpace * threads = multiseq_threads(t);

  n = rest = muliseq_items(multiseq);

  MultiseqTread * thread = NULL;
  while ((thread = multiseq_thread_next(threads)) != NULL) {

    addition = calculate_addition(t, thread->id, rest);
    start = calculate_start(t, thread->id, rest, addition);
    end = rest;
    rest = calculate_rest(start);

    thread->i = start;
    thread->j = end;
    thread->multiseq = multiseq;
    thread->compare = eval_unit_edist;
    pthread_create_or_die(&thread->identifier, eval_seqrange_thread, thread);
  }

  while ((thread = multiseq_thread_next(threads)) != NULL) {
    pthread_join(thread->identifier, NULL);
  }

  n = muliseq_paars(multiseq);
  for (i = 0; i < n; i++) {
    const MultiseqPaar * element = muliseq_paar(multiseq, i);
    best_k_vals_insert(kval, element);
  }

  best_k_vals_reset_iter(kval);
  for (i = 0; i < k; i++) {
    const MultiseqPaar * element = (MultiseqPaar *) best_k_vals_next(kval);
    printf("%lu\t%lu\t%lu\n", element->id1, element->id2, element->distance);
  }

  best_k_vals_delete(kval);
  multiseq_threads_delete(threads);
  muliseq_delete(multiseq);
}

/**
 * Just parse a user-defined parameters
 */
int main(int argc, char * argv[]) {

  unsigned long k, t;

  if (argc == 4) {
    if (sscanf(argv[1], "%lu", &k) == 1) {
      if (sscanf(argv[2], "%lu", &t) == 1) {
        process(argv[3], k, t);
        exit(EXIT_SUCCESS);
      }
    }
  }
  fprintf(stderr, "Usage: %s <k-value> <threads> <filename>", argv[0]);
  exit(EXIT_FAILURE);
}
