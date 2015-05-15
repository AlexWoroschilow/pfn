/*
 ============================================================================
 Name        : intset.c
 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : a efficient intset implementation
 ============================================================================
 */

#ifndef ASSERTIONS_H
#define ASSERTIONS_H

#include <stdio.h>
#include <stdlib.h>

#define assert_with_message(x, message) \
  if (!(x)) { \
    fprintf(stderr, "%s\n", message); \
    exit(EXIT_FAILURE); \
  }

#define malloc_or_exit(x, size) \
  if (!(x = malloc(size))) { \
    fprintf(stderr, "Coult not allocate memory of size %lu\n", \
            (unsigned long) size); \
    exit(EXIT_FAILURE); \
  }

#define calloc_or_exit(x, elems, size) \
  if (!(x = calloc(elems, size))) { \
    fprintf(stderr, "Could not calloc memory of size %lu\n", \
           (unsigned long) (elems) * (size)); \
    exit(EXIT_FAILURE); \
  }

#define realloc_or_exit(x, size) \
  if (!(x = realloc(x, size))) { \
    fprintf(stderr, "Could not realloc memory of size %lu\n", \
           (unsigned long) size); \
    exit(EXIT_FAILURE); \
  }

#endif
