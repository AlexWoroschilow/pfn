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

/* Check conditions and return
 a message if fail */
#define ASSERT_WITH_MESSAGE(condition, message, file, line) \
  if(!(condition)) \
  { \
    fprintf(stderr, message, file, line); \
    exit(EXIT_FAILURE); \
  }

/* Reallocate a memory and display
 a message if not success */
#define REALLOC_OR_EXIT(pointer, size, message, file, line)\
  ASSERT_WITH_MESSAGE((pointer = realloc(pointer, size)) != NULL, message, file, line);
