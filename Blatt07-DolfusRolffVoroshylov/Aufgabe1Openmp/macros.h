#ifndef CUSTOM_MACROS_H
#define CUSTOM_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

/**
 * Check conditions and return
 * a message if fail
 */
#define assert_with_message(condition, message) \
  if (!(condition)) { \
    fprintf(stderr, "%s in file \"%s\" at line %lu\n", \
            message, __FILE__, (unsigned long) (__LINE__ - 1)); \
    exit(EXIT_FAILURE); \
  }

/**
 * Allocates memory and display a message
 * if the allocation failed.
 */
#define malloc_or_exit(pointer, size, message) \
  assert_with_message((pointer = malloc(size)) != NULL, message);

/**
 * Allocates memory and initialized int with zeros.
 * On failure display a message
 */
#define calloc_or_exit(pointer, elems, size, message) \
  assert_with_message((pointer = calloc(elems, size)) != NULL, message);
  
/**
 * Reallocate memory and display
 * a message if not success
 */
#define realloc_or_exit(pointer, size, message)\
  assert_with_message((pointer = realloc(pointer, size)) != NULL, message);

/**
 * Try to open file and
 * assert pointer not equal NULL
 */
#define fopen_or_exit(source, filename, mode) \
    if ((source = fopen(filename, mode)) == NULL) { \
        fprintf(stderr, "Can not open file %s in mode %s\n" \
                        "Error in file %s at line %lu\n", \
                        filename, mode, __FILE__, (unsigned long) __LINE__); \
        exit(EXIT_FAILURE); \
    }
    
#define minumum(x, y) (x) < (y) ? (x) : (y);
#define maximum(x, y) (x) < (y) ? (x) : (y);

#endif