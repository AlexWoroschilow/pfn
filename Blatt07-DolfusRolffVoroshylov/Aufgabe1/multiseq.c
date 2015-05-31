#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "macros.h"
#include "multiseq.h"

#define open_or_exit(filename, file, status) \
    file = open(filename, O_RDONLY);\
    assert_with_message(!fstat(file, status), "Can not get file status");

#define mmap_or_exit(filemap, file, status) \
    filemap = mmap(0, status->st_size, PROT_READ, MAP_SHARED, file, 0);\
    assert_with_message((filemap != MAP_FAILED), "Can not map a file");

struct MultiseqItem {
  char * start;
  unsigned long length;
};

typedef struct MultiseqItemSpace {
  unsigned long size;
  unsigned long current;
  MultiseqItem * items;
} MultiseqItemSpace;

struct Multiseq {
  char * filemap;
  char * filename;
  struct stat * status;
  MultiseqItemSpace * space;
};

static void muliseq_item_space_resize(MultiseqItemSpace * space,
    unsigned long append) {

  unsigned long i, resize;
  MultiseqItem * cache;

  resize = space->size + append;
  realloc_or_exit(space->items, sizeof(*space->items) * resize,
      "Can not allocate memory");

  for (i = space->size; i < resize; i++) {
    cache = &space->items[i];
    cache->start = NULL;
    cache->length = 0UL;
  }

  space->size = resize;
}

static void muliseq_item_space_fill(Multiseq * multiseq, char * stream,
    unsigned long index) {

  unsigned long i;
  MultiseqItem * item;

  if (multiseq->space->size <= index) {
    muliseq_item_space_resize(multiseq->space, 10UL);
  }

  item = &multiseq->space->items[index];
  for (i = 0; i < strlen(stream); i++) {
    if (stream[i] == '\n') {
      break;
    }
  }

  item->start = stream;
  item->length = i;
}

static MultiseqItemSpace * muliseq_item_space_new() {

  MultiseqItemSpace * space = NULL;
  realloc_or_exit(space, sizeof(*space), "Can not allocate memory");

  space->size = 0UL;
  space->current = 0UL;
  space->items = NULL;

  muliseq_item_space_resize(space, 10UL);

  return space;
}

static bool muliseq_item_start(char * stream) {
  return (stream[0] == '\n' && stream[1] != '>');
}

Multiseq * muliseq_new(char * filename) {

  unsigned long file, i = 0, j = 0;

  Multiseq * multiseq = NULL;
  realloc_or_exit(multiseq, sizeof(*multiseq), "Can not allocate memory");

  multiseq->filename = filename;
  multiseq->space = muliseq_item_space_new();

  multiseq->status = NULL;
  realloc_or_exit(multiseq->status, sizeof(*multiseq->status),
      "Can not allocate memory");

  open_or_exit(multiseq->filename, file, multiseq->status);
  mmap_or_exit(multiseq->filemap, file, multiseq->status);

  for (i = 0; (i + 1) < multiseq->status->st_size; i++) {
    if (muliseq_item_start(&multiseq->filemap[i])) {
      muliseq_item_space_fill(multiseq, &multiseq->filemap[i + 1], j++);
    }
  }

  return multiseq;
}

MultiseqItem * muliseq_item_next(Multiseq * multiseq) {
  if (multiseq->space->current < multiseq->space->size) {
    MultiseqItem * item = &multiseq->space->items[multiseq->space->current++];
    if (item->start != NULL && item->length) {
      return item;
    }
  }
  return NULL;
}

void muliseq_item_show(MultiseqItem * item) {
  int i;
  printf("%lu\t", item->length);
  for (i = 0; i < item->length; i++) {
    printf("%c", item->start[i]);
  }
  printf("\n");
}

void muliseq_destroy(Multiseq * multiseq) {

  if (multiseq) {
    if (multiseq->space) {
      free(multiseq->space->items);
      free(multiseq->space);
    }
    if (multiseq->status) {
      munmap(multiseq->filemap, multiseq->status->st_size);
      free(multiseq->status);
    }

    free(multiseq);
  }

}

