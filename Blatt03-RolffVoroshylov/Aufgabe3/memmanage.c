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
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "memmanage.h"
#include "memmanage-mac.h"
/**
 * Create new memory
 * block table
 */
MMspacetable* mem_man_new(unsigned long numberofblocks) {

  MMspacetable *table = NULL;

  REALLOC_OR_EXIT(table, sizeof(MMspacetable),
      "Can not allocate memory for memory-table: file \"%s\", line %d\n",
      __FILE__, __LINE__);

  table->blockspace = NULL;

  REALLOC_OR_EXIT(table->blockspace, (sizeof(MMspaceblock) * numberofblocks),
      "Can not allocate memory for block-space: file \"%s\", line %d\n",
      __FILE__, __LINE__);

  table->next = 0;
  table->number = numberofblocks;

  return table;
}

/**
 * Get block from
 * table by pointer
 */
MMspaceblock * get_block_by_pointer(MMspacetable *st, void *ptr) {
  unsigned long i;
  for (i = 0; i < st->next; i++) {
    MMspaceblock * check = (st->blockspace + i);
    if (check->ptr == ptr) {
      return check;
    }
  }
  return NULL;
}

/**
 * Allocate memory block
 */
void *mem_man_alloc(MMspacetable *st, char *file, unsigned long line, void *ptr,
    unsigned long size, unsigned long number) {

  MMspaceblock * block = get_block_by_pointer(st, ptr);

  if (block == NULL) {
    block = (st->blockspace + st->next++);
    block->ptr = NULL;
  }

  REALLOC_OR_EXIT(block->ptr, size * number,
      "Can not allocate memory for real data block: file \"%s\", line %lu\n",
      file, line);

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

  MMspaceblock * block = get_block_by_pointer(st, ptr);
  ASSERT_WITH_MESSAGE(block != NULL,
      "block was not allocated: file \"%s\", line %lu\n", file, line);

  free(block->ptr);
  block->ptr = NULL;
}

/**
 * Display memory table state
 */
void mem_man_info(const MMspacetable *st) {

  unsigned long i;
  for (i = 0; i < st->next; i++) {
    MMspaceblock * block = (st->blockspace + i);
    printf("# active block %lu: allocated in file \"%s\", line %lu\n", i,
        block->file, block->line);
  }
}

/**
 * Check for memory leaks
 */
void mem_man_check(const MMspacetable *st) {
  unsigned long i;
  for (i = 0; i < st->next; i++) {
    MMspaceblock * block = (st->blockspace + i);
    if (block->ptr == NULL) {
      continue;
    }
    fprintf(stderr, "space leak: main memory for block %lu not freed\n", i);
    fprintf(stderr, "%lu cells of size %lu\n", block->number, block->size);
    fprintf(stderr, "allocated: file \"%s\", line %lu\n", block->file,
        block->line);
    exit(EXIT_FAILURE);

  }
}

/**
 * Free a memory table structure
 */
void mem_man_delete(MMspacetable *st) {
  free(st->blockspace);
  free(st);
}

