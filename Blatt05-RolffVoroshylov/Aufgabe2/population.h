/*
 ============================================================================
 Name        : simul_evoution.c

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : simulate evolution
 ============================================================================
 */

/**
 * Structure to store a whole population data
 */
typedef struct Population Population;

/**
 * Initialize population with given parameters
 *
 * @variable n_a - count of Dolly - A bacteria
 * @variable n_b - count of Dolly - B bacteria
 * @variable n_b - reproduce probability of Dolly - B bacteria
 */
Population * population_initialize(Population * population,
                                   unsigned long n_a,
                                   unsigned long n_b, FILE * export);

/**
 * Eliminate current population
 *
 * @variable population - population to eliminate
 */
void population_genocide(Population * population);

/**
 * Free memory for current population
 *
 * @variable population - population to eliminate
 */
void population_free(Population * population);

/**
 * Create a new generation on given population
 *
 * @variable population - affected population
 * @variable i - step
 * @variable p_a - reproduce probability of Dolly - A bacteria
 * @variable p_b - reproduce probability of Dolly - B bacteria
 */
void population_generation(Population * population, unsigned long step,
                           float p_a, float p_b);

/**
 * Print statistic of a current population
 *
 * @variable population - population to print
 */
void population_generation_dump(Population * population, unsigned long step);

/**
 * Print a population statistics data
 * using a given print-function
 */
void population_generation_print(Population * population,
                                 void (*printer)(unsigned long count,
                                                 unsigned long count_a,
                                                 unsigned long count_b));

/**
 * Check a population properties
 * using a custom function
 */
unsigned int
population_generation_check(Population * population,
                            unsigned int (*checker)(unsigned long count,
                                                    unsigned long count_a,
                                                    unsigned long count_b));
