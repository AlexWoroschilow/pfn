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
#include <string.h>
#include <assert.h>
#include "population.h"

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

/**
 * Do simulation
 */
void simul_evolution(unsigned long n_a, float p_a, unsigned long n_b, float p_b,
    unsigned long maxsteps, char * filename) {

  Population * population = population_initialize(n_a, n_b, filename);

  unsigned long i;
  for (i = 1; i <= maxsteps; i++) {

    population_generation(population, i, p_a, p_b);

    if ((population_generation_check(population, &checker))) {
      population_generation_print(population, &printer_success);
      printf("\tsteps:%lu\n", i);
      break;
    }
  }

  if (i > maxsteps) {
    printf("simulation stopped after %lu steps ", i);
    population_generation_print(population, &printer_fail);
    printf("\n");
  }

  population_free(population);
}

/**
 * Main function
 */
int main(int argc, char * argv[]) {

  float p_a, p_b;
  unsigned long n_a, n_b, maxsteps;
  char * filename = NULL;

  if (argc == 6 || argc == 7) {

    /* Try to find all integer parameters */
    if (sscanf(argv[1], "%lu", &n_a) == 1 && sscanf(argv[3], "%lu", &n_b) == 1
        && sscanf(argv[5], "%lu", &maxsteps) == 1) {

      /* try to find all float parameters */
      if (sscanf(argv[2], "%f", &p_a) == 1
          && sscanf(argv[4], "%f", &p_b) == 1) {

        if (argc == 7) {
          filename = strdup(argv[6]);
          if (!sscanf(argv[6], "%s", filename)) {
            free(filename);
          }
        }

        /* do simulation */
        simul_evolution(n_a, p_a, n_b, p_b, maxsteps, filename);

        free(filename);

        exit(EXIT_SUCCESS);
      }
    }
  }
  printf("Usage: %s <n_a> <p_a> <n_b> <p_b> <maxsteps> [<filename>]\n",
      argv[0]);
  exit(EXIT_FAILURE);
}
