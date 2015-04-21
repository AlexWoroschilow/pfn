/*
 ============================================================================
 Name        : memmanage-mac.c

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : Memory manage task
 ============================================================================
 */
#include <stdio.h>
#include "memmanage.h"

/**
 * Allocate memory
 */
#define MEM_MAN_ALLOC(ST,P,T,N)\
  mem_man_alloc(ST, __FILE__, __LINE__, P, sizeof(T), N);

/**
 * Free allocated memory
 */
#define MEM_MAN_DELETE_PTR(ST,P)\
  mem_man_delete_ptr(ST, __FILE__, __LINE__, P);

/**
 * if dynamic memory allocation enabled
 * this macro should resize a table
 */
#if DYNAMIC_MEMORY == 1
#define MEM_MAN_RESIZE(st, size, ptr)\
          mem_man_resize(st, size);
#else
/**
 * if dynamic memory allocation enabled
 * given pointer should not be NULL
 */
#define MEM_MAN_RESIZE(st, size, ptr)\
  assert_with_message(ptr != NULL, \
        "All posible memory entrys are used " \
        "so there is no free space for a new one\n" \
        "Please set DYNAMIC_MEMORY 1 " \
        "if you want dynamic memory.\n" \
        "Error happend in:");
#endif

/**
 * Validate memory table
 * and block space
 */
#define mem_man_validate(st) \
  assert_with_message(st != NULL, "The table can not be NULL!"); \
  assert_with_message(st->blockspace != NULL, \
                      "There are no memory allocated for the blocks"); \


/* Check conditions and return
 a message if fail */
#define assert_with_message(condition, message) \
  if(!(condition)) { \
    fprintf(stderr, "%s in file \"%s\" at line %lu\n", \
            message, __FILE__, (unsigned long) (__LINE__ - 1)); \
    exit(EXIT_FAILURE); \
  }

#define assert_with_message_and_position(condition, message, file, line) \
  if(!(condition)) { \
    fprintf(stderr, "%s, in file \"%s\" at line %lu\n", \
            message, file, line); \
    exit(EXIT_FAILURE); \
  }

/* Reallocate a memory and display
 a message if not success */
#define realloc_or_exit(pointer, size, message)\
  assert_with_message((pointer = realloc(pointer, size)) != NULL, message);

/* Allocate a memory and display
 a message and stop the program if the allocation failed
 */
#define malloc_or_exit(pointer, size, message) \
  assert_with_message((pointer = malloc(size)) != NULL, message);
