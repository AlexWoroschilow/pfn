/*
 ============================================================================
 Name        : simul_evoution.c

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : simulate evolution
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "population.h"

/* set deterministic to 1 if you want deterministic resutlts */
#define DETERMINISTIC 0

/* Try to open file and
 assert pointer not equal NULL
 */
#define fopen_or_exit(source, filename, mode) \
    if ((source = fopen(filename, mode)) == NULL) { \
        fprintf(stderr, "Can not open file %s in mode %s\n" \
                        "Error in file %s at line %lu\n", \
                        filename, mode, __FILE__, (unsigned long) __LINE__); \
        exit(EXIT_FAILURE); \
    }

/**
 * Check if a all bacteria
 * have a same gene
 */
unsigned int checker(unsigned long count, unsigned long count_a,
                     unsigned long count_b) {
  return ((count == count_a) || (count == count_b));
}

/**
 * Display a loop-end message
 */
void printer_fail(__attribute__((unused)) unsigned long count,
                  unsigned long count_a, unsigned long count_b) {
  printf("(A:%lu,B:%lu)", count_a, count_b);
}

/**
 * Print a message for a found
 * fixed step
 */
void printer_success(unsigned long count, unsigned long count_a,
                     __attribute__((unused)) unsigned long count_b) {
  printf("fixed:%s", ((count == count_a) ? "A" : "B"));
}

void init_seed() {
#if DETERMINISTIC == 0
  const time_t current_time = time(NULL);
  srand48(current_time);
#else
  srand48(0);
#endif
}

/**
 * Do simulation
 */
void simul_evolution(unsigned long n_a, float p_a, unsigned long n_b, float p_b,
                     unsigned long maxsteps, char * filename) {
  unsigned long i;
  FILE * dump = NULL;
  Population * population = NULL;

  if (filename != NULL) {
    fopen_or_exit(dump, filename, "w");
  }

  init_seed();
  population = population_initialize(NULL, n_a, n_b, dump);
  for (i = 1; i <= maxsteps; i++) {
    population_generation(population, i, p_a, p_b);
    if ((population_generation_check(population, &checker))) {
      population_generation_print(population, &printer_success);
      printf("\tsteps:%lu\n", i);
      break;
    }
  }

  if (i > maxsteps) {
    printf("simulation stopped after %lu steps ", maxsteps);
    population_generation_print(population, &printer_fail);
    printf("\n");
  }

  population_free(population);

  if (dump) {
    fclose(dump);
  }
}

/**
 * Main function
 */
int main(int argc, char * argv[]) {
  bool error;
  float p_a, p_b;
  unsigned long n_a, n_b, maxsteps;
  char* filename = NULL;

  error = false;
  if (argc == 6 || argc == 7) {
    /* Try to find all integer parameters */
    if (sscanf(argv[1], "%lu", &n_a) == 1 &&
        sscanf(argv[3], "%lu", &n_b) == 1 &&
        sscanf(argv[5], "%lu", &maxsteps) == 1 &&
        /* try to find all float parameters */
        sscanf(argv[2], "%f", &p_a) == 1 && p_a >= 0.0 && p_a <= 1.0 &&
        sscanf(argv[4], "%f", &p_b) == 1 && p_b >= 0.0 && p_b <= 1.0) {
      if (argc == 7) {
        filename = argv[6];
      }

      /* do simulation */
      simul_evolution(n_a, p_a, n_b, p_b, maxsteps, filename);
    } else {
      error = true;
    }
  } else {
    error = true;
  }

  if (error) {
    fprintf(stderr,
            "Usage: %s <n_a> <p_a> <n_b> <p_b> <maxsteps> [<filename>]\n"
            "Where n_a and n_b have to be natural numbers\n"
            "and p_a and p_b are in range between 0 and 1\n",
            argv[0]);
  }

  return EXIT_SUCCESS;
}
