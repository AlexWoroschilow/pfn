/*
 ============================================================================
 Name        : memmanage-mac.c

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : Memory manage task
 ============================================================================

HINWEIS:
Es wurde ein dynamischer Speicher verwendet, auch wenn dies in der Aufgabe
nicht explizit verlangt war.
Dieses macht aber ohne dynamische Speicherverwaltung keinen sind, da es
sich ja um einen Leak detector handelt und somit auch mit beliebigen
speicher block groessen umgehen koennen sollte.
 */
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "memmanage.h"
#include "memmanage-mac.h"

#define defaultCheck(st) \
  assert_with_message(st != NULL, "The table can not be NULL!"); \
  assert_with_message(st->blockspace != NULL, \
                      "There are no memory allocated for the blocks"); \

struct MMspaceblock {
  /*
  Pointer of the reserved memory region
  */
  void* ptr;

  /*
  The size of one object in the pointer
  */
  unsigned long size;

  /*
  The count of objects in the pointer
  */
  unsigned long number;

  /*
  The line in which the allocation happend
  */
  unsigned long line;

  /*
  The filename in which the allocation happend
  */
  char* file;
};

struct MMspacetable {
  /*
  The memory blocks of the space table
  */
  MMspaceblock* blockspace;

  /*
  The count of the blocks
  */
  unsigned long number;
};

/**
 * Create new memory
 * block table
 */
MMspacetable* mem_man_new(unsigned long numberofblocks) {
  unsigned long i;
  MMspacetable *table = NULL;
  MMspaceblock * block = NULL;
  assert_with_message(numberofblocks > 0,
                      "The passed argument must be bigger than zero\n");

  malloc_or_exit(table, sizeof(MMspacetable),
                 "Can not allocate memory for memory-table");

  malloc_or_exit(table->blockspace,
                 sizeof(MMspaceblock) * numberofblocks,
                 "Can not allocate memory for block-space");

  /*
  Initalize with default values
  */
  for (i = 0; i < numberofblocks; ++i) {
    block = &table->blockspace[i];
    block->ptr = NULL;
    block->size = 0;
    block->number = 0;
    block->line = 0;
    block->file = NULL;
  }

  table->number = numberofblocks;
  return table;
}

/**
 * Get block from
 * table by pointer
 */
static unsigned long get_block_id_by_pointer(MMspacetable *st, void *ptr) {
  unsigned long i;

  defaultCheck(st);
  if (ptr) {
    for (i = 0; i < st->number; ++i) {
      const MMspaceblock* check = &st->blockspace[i];
      if (check->ptr == ptr) {
        return i;
      }
    }
  }
  else {
    for (i = 0; i < st->number; ++i) {
      const MMspaceblock* check = &st->blockspace[i];
      if (!check->ptr) {
        return i;
      }
    }
  }

  /* nothing found */
  return st->number;
}

/*
  Resizes the inner table of the blockspace to ensure
  that new elements find there space in it.
*/
static void resize_spacetable(MMspacetable* st, unsigned long newsize) {
  unsigned long i;
  MMspaceblock* block = NULL;

  defaultCheck(st);
  assert_with_message(st->number < newsize,
                      "The resize function can only expand the table");
  realloc_or_exit(st->blockspace, sizeof(MMspaceblock) * newsize,
                  "Could not resize the blocktable in the spacetable");

  /*
  Initalize with default values
  */
  for (i = st->number; i < newsize; ++i) {
    block = &st->blockspace[i];
    block->ptr = NULL;
    block->size = 0;
    block->number = 0;
    block->line = 0;
    block->file = NULL;
  }

  st->number = newsize;
}

/**
 * Allocate memory block
 */
void *mem_man_alloc(MMspacetable *st, char *file,
                    unsigned long line, void *ptr,
                    unsigned long size, unsigned long number) {

  unsigned long i;
  MMspaceblock * block = NULL;

  defaultCheck(st);
  assert_with_message(file != NULL,
                      "The filename name can not be NULL!");

  i = get_block_id_by_pointer(st, ptr);
  if (i == st->number) {
    resize_spacetable(st, st->number * 2);
  }

  block = &st->blockspace[i];
  realloc_or_exit(block->ptr, size * number,
                  "Can not allocate memory for real data block");

  block->file = file;
  block->line = line;
  block->size = size;
  block->number = number;
  return block->ptr;
}

/**
 * Free a pointer,
 * show an error if pointer
 * was not allocated
 */
void mem_man_delete_ptr(MMspacetable *st, char *file, unsigned long line,
                        void *ptr) {
  unsigned long i;
  MMspaceblock* block = NULL;

  defaultCheck(st);
  assert_with_message(file != NULL, "The filename can not be NULL!");

  i = get_block_id_by_pointer(st, ptr);
  assert_with_message_and_position(i < st->number,
                                   "block was not allocated:",
                                   file, line);
  block = &st->blockspace[i];
  free(block->ptr);

  /* set default values */
  block->ptr = NULL;
  block->size = 0;
  block->number = 0;
  block->line = 0;
  block->file = NULL;
}

/**
 * Display memory table state
 */
void mem_man_info(const MMspacetable *st) {

  unsigned long i;
  defaultCheck(st);

  for (i = 0; i < st->number; i++) {
    const MMspaceblock * block = &st->blockspace[i];
    if(block->ptr) {
      printf("# active block %lu: allocated in file \"%s\", line %lu\n",
             i, block->file, block->line);
    }
  }
}

/**
 * Check for memory leaks
 */
void mem_man_check(const MMspacetable *st) {
  unsigned long i;
  defaultCheck(st);

  for (i = 0; i < st->number; i++) {
    const MMspaceblock * block = &st->blockspace[i];
    if (block->ptr) {
      fprintf(stderr, "space leak: main memory for "
                      "block %lu not freed\n", i);
      fprintf(stderr, "%lu cells of size %lu\n",
              block->number, block->size);
      fprintf(stderr, "allocated: file \"%s\", line %lu\n",
              block->file, block->line);

      /*
      Free as mutch as posible.
      The MMspacetable could not be freed becase it is
      declared as const.
      */
      freeMemory(st);
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * Free a memory table structure
 */
void mem_man_delete(MMspacetable *st) {
  if(st)
  {
    defaultCheck(st);

    freeMemory(st);
    st->blockspace = NULL;
    free(st);
    st = NULL;
  }
}

