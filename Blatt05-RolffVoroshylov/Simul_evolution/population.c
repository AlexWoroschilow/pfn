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
#include <assert.h>
#include "population.h"

/* Check conditions and return
 a message if fail */
#define assert_with_message(condition, message) \
  if(!(condition)) { \
    fprintf(stderr, "%s in file \"%s\" at line %lu\n", \
            message, __FILE__, (unsigned long) (__LINE__ - 1)); \
    exit(EXIT_FAILURE); \
  }

/* Allocate a memory and display
 a message and stop the program if the allocation failed
 */
#define malloc_or_exit(pointer, size, message) \
  assert_with_message((pointer = malloc(size)) != NULL, message);

/* Reallocate a memory and display
 a message if not success */
#define realloc_or_exit(pointer, size, message)\
  assert_with_message((pointer = realloc(pointer, size)) != NULL, message);

/**
 * Validate a simple bacteria
 */
#define ENPE_COMPLETII_VALIDATE(bacteria) \
    assert_with_message((bacteria != NULL), "EnpeCompletii can not be empty");\
    assert_with_message((bacteria->gene !=NULL), "EnpeCompletii gene can not be empty");\


/**
 * needs to minimize a
 * function-calls
 */
#define POPULATION_GENERATION_DUMP(export_file, population, step) \
  if((export_file)) { \
    population_generation_dump(population, step);\
  }

/**
 * Validate a population
 */
#define POPULATION_VALIDATE(population) \
    assert_with_message((population != NULL), "Population can not be empty");\
    assert_with_message((population->space != NULL), "Population space can not be empty");\
    assert_with_message((population->count > 0), "Count of individuums in population can not be 0");\

/**
 * Validate a population
 */
#define POPULATION_VALIDATE_LIGHT(population) \
    assert_with_message((population != NULL), "Population can not be empty");\
    assert_with_message((population->count > 0), "Count of individuums in population can not be 0");\


/**
 * Get random number from 0 to max
 */
#define RAND(max)\
    (rand() % ((max)+1))\

/**
 * Dolly gene with 2 variants (0,1)
 */
typedef enum Dolly {
  A, B
} Dolly;

/**
 * Structure to store gene-information
 * for each bacteria
 */
typedef struct Gene {
  Dolly type;
  float probability;
} Gene;

/**
 * Structure to store
 * a bacteria with gene information
 */
typedef struct EnpeCompletii {
  Gene * gene;
} EnpeCompletii;

/**
 * Just a place for one bacteria
 * in a population
 */
typedef struct PopulationSpot {
  EnpeCompletii * individuum;
} PopulationSpot;

/**
 * Structure to store a whole population data
 *
 * @variable count - count of bacteria in population
 * @variable count_a - count of bacteria with Dolly - A
 * @variable count_b - count of bacteria with Dolly - B
 */
typedef struct Population {
  unsigned long count;
  unsigned long count_a;
  unsigned long count_b;
  FILE * export;
  PopulationSpot * space;
} Population;

/**
 * Create a new bacteria with given data
 *
 * @variable type - type of Dolly-gene
 * @variable probability - reproduce-probability
 */
EnpeCompletii * enpe_completii_create(Dolly type, float probability) {

  EnpeCompletii * individual = NULL;

  malloc_or_exit(individual, sizeof(*individual),
      "Can not allocate memory for generation space");

  malloc_or_exit(individual->gene, sizeof(*individual->gene),
      "Can not allocate memory for generation space");

  individual->gene->type = type;
  individual->gene->probability = probability;

  return individual;
}

/**
 * Reproduce a current bacteria,
 * check for reproduce probability from Gene,
 * and create a new bacteria or not
 *
 * @variable current - bacteria to reproduce
 */
EnpeCompletii * enpe_completii_reproduce(EnpeCompletii * bacteria) {

  ENPE_COMPLETII_VALIDATE(bacteria);

  /* for zero probability do nothing */
  if (bacteria->gene->probability) {
    float random = RAND(100) / 100;
    if (bacteria->gene->probability == 1
        || bacteria->gene->probability > random) {
      return enpe_completii_create(bacteria->gene->type,
          bacteria->gene->probability);
    }
  }

  return NULL;
}

/**
 * Kill a given bacteria
 *
 * @variable current - bacteria to kill
 */
void enpe_completii_kill(EnpeCompletii * bacteria) {
  if (bacteria) {
    free(bacteria->gene);
    free(bacteria);
  }
}

/**
 * Initialize population with given parameters
 *
 * @variable n_a - count of Dolly - A bacteria
 * @variable p_a - reproduce probability of Dolly - A bacteria
 * @variable n_b - count of Dolly - B bacteria
 * @variable n_b - reproduce probability of Dolly - B bacteria
 */
Population * population_initialize(Population * population, unsigned long n_a,
    unsigned long n_b, FILE * export) {

  if (population == NULL) {
    realloc_or_exit(population, sizeof(*population),
        "Can not allocate memory for population");
  }

  population->space = NULL;
  population->export = export;
  population->count = n_a + n_b;
  population->count_a = n_a;
  population->count_b = n_b;

  return population;
}

/**
 * Append new bacteria into population
 *
 * @variable population - population to append a new bacteria
 * @variable spot - spot in this population to append a new bacteria
 * @variable children - bacteria to append
 */
void population_append(Population * population, PopulationSpot * spot,
    EnpeCompletii * children) {

  POPULATION_VALIDATE(population);

  spot->individuum = children;

  if (children->gene->type == A) {
    population->count_a++;
  } else {
    population->count_b++;
  }
}

/**
 * Kill a bacteria from in a given spot of population
 *
 * @variable population - affected population
 * @variable spot - affected spot
 */
void population_kill(Population * population, PopulationSpot * spot) {

  POPULATION_VALIDATE(population);

  EnpeCompletii * bacteria = spot->individuum;
  Dolly type = bacteria->gene->type;

  spot->individuum = NULL;
  enpe_completii_kill(bacteria);

  if (type == A) {
    population->count_a--;
  } else {
    population->count_b--;
  }
}

/**
 * Eliminate current population
 *
 * @variable population - population to eliminate
 */
void population_genocide(Population * population) {

  POPULATION_VALIDATE(population);

  unsigned int i;
  for (i = 0; i < population->count; i++) {
    PopulationSpot * population_spot = &population->space[i];
    enpe_completii_kill(population_spot->individuum);
    population_spot->individuum = NULL;
  }

  free(population->space);
  population->space = NULL;
}

/**
 * Free memory for current population
 *
 * @variable population - population to eliminate
 */
void population_free(Population * population) {

  if (population) {
    population_genocide(population);
    free(population->space);
    free(population);
  }
}

/**
 * Get random place in a population
 *
 * @variable population - population to get spot id from
 */
unsigned long population_random_spot_id(Population * population) {
  POPULATION_VALIDATE(population);
  return RAND(population->count);
}

/**
 * Create a new generation on given population
 *
 * @variable population - affected population
 */
void population_generation(Population * population, unsigned long step,
    float p_a, float p_b) {

  POPULATION_VALIDATE_LIGHT(population);

  /* Create a first generation */
  if (!population->space) {

    realloc_or_exit(population->space,
        (population->count * sizeof(*population->space)),
        "Can not allocate memory for population space");

    unsigned int i;
    for (i = 0; i < population->count; i++) {
      EnpeCompletii * bacteria = enpe_completii_create(
          ((i < population->count_a) ? A : B),
          ((i < population->count_a) ? p_a : p_b));
      population->space[i].individuum = bacteria;
    }

    /* Create other generations */
  } else {

    unsigned long i;
    for (i = 0; i < population->count; i++) {

      EnpeCompletii * children = NULL;
      if ((children = enpe_completii_reproduce(population->space[i].individuum))) {

        unsigned long id = population_random_spot_id(population);

        if (id == population->count) {
          enpe_completii_kill(children);
          continue;
        }

        PopulationSpot * spot = &population->space[id];
        population_kill(population, spot);
        population_append(population, spot, children);
      }
    }

  }

  /* dump population to file*/
  POPULATION_GENERATION_DUMP(population->export, population, step);
}

/**
 * Print statistic of a current population
 *
 * @variable population - population to print
 */
void population_generation_dump(Population * population, unsigned long step) {
  POPULATION_VALIDATE(population);
  if (population->export) {
    fprintf(population->export, "%lu\t%lu\t%lu\n", step, population->count_a,
        population->count_b);
  }
}

/**
 * Print a population statistics data
 * using a given print-function
 */
void population_generation_print(Population * population,
    void (*printer)(unsigned long count, unsigned long count_a,
        unsigned long count_b)) {
  POPULATION_VALIDATE(population);
  return (*printer)(population->count, population->count_a, population->count_b);
}

/**
 * Check a population properties
 * using a custom function
 */
unsigned int population_generation_check(Population * population,
    unsigned int (*checker)(unsigned long count, unsigned long count_a,
        unsigned long count_b)) {
  POPULATION_VALIDATE(population);
  return (*checker)(population->count, population->count_a, population->count_b);
}
