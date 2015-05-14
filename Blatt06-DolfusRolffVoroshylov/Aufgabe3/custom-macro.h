#ifndef CUSTOMMACRO
#define CUSTOMMACRO
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
 * Reallocate a memory and display
 * a message if not success
 */
#define realloc_or_exit(pointer, size, message)\
  assert_with_message((pointer = realloc(pointer, size)) != NULL, message);

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

#endif
