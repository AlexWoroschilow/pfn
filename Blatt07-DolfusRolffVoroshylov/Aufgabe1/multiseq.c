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
#include <pthread.h>

#include "macros.h"
#include "multiseq.h"

#define open_or_exit(filename, file, status) \
    file = open(filename, O_RDONLY);\
    assert_with_message(!fstat(file, status), "Can not get file status");

#define mmap_or_exit(filemap, file, status) \
    filemap = mmap(0, status->st_size, PROT_READ, MAP_SHARED, file, 0);\
    assert_with_message((filemap != MAP_FAILED), "Can not map a file");

struct MultiseqPaarSpace {
  unsigned long size;
  unsigned long count;
  MultiseqPaar * elements;
};

struct MultiseqItemSpace {
  unsigned long size;
  unsigned long count;
  MultiseqItem * elements;
};

static void muliseq_resize_paar(MultiseqPaarSpace * space, unsigned long size) {
  unsigned long i;
  realloc_or_exit(space->elements, sizeof(*space->elements) * size,
      "Can not allocate memory");

  for (i = space->count; i < size; i++) {
    space->elements[i].distance = 0;
    space->elements[i].id1 = 0;
    space->elements[i].id2 = 0;
  }

  space->size = size;
}

const MultiseqPaar * muliseq_fill_paar(Multiseq * multiseq,
                                       const unsigned long distance,
                                       const unsigned long id1,
                                       const unsigned long id2) {
  MultiseqPaar * element;
  MultiseqPaarSpace * space;
  assert_with_message(multiseq != NULL, "Multiseq can not be NULL");

  space = multiseq->paars;
  if (space->size <= space->count) {
    muliseq_resize_paar(space, space->size + space->size * 0.2);
  }

  element = &space->elements[space->count++];
  element->distance = distance;
  element->id1 = id1;
  element->id2 = id2;

  return (const MultiseqPaar *) element;
}

static MultiseqPaarSpace * muliseq_init_paars() {
  MultiseqPaarSpace * space = NULL;
  realloc_or_exit(space, sizeof(*space), "Can not allocate memory");

  space->size = 10UL;
  space->count = 0UL;
  space->elements = NULL;

  realloc_or_exit(space->elements, sizeof(*space->elements) * space->size,
      "Can not allocate memory");
  return space;
}

static void muliseq_resize_items(MultiseqItemSpace * space,
                                 const unsigned long size) {
  unsigned long i;
  assert_with_message(space != NULL, "space can not be NULL");
  realloc_or_exit(space->elements, sizeof(*space->elements) * size,
      "Can not allocate memory");

  for (i = space->size; i < size; i++) {
    space->elements[i].start = NULL;
    space->elements[i].length = 0UL;
  }

  space->size = size;
}

const MultiseqItem * muliseq_fill_item(Multiseq * multiseq,
                                       char * stream) {
  unsigned long i;
  MultiseqItem * item;
  MultiseqItemSpace * space;
  assert_with_message(multiseq != NULL, "Multiseq can not be NULL");

  space = multiseq->items;
  if (space->size <= space->count) {
    muliseq_resize_items(space, space->size + space->size * 0.2);
  }

  item = &space->elements[space->count++];
  for (i = 0; i < strlen(stream); i++) {
    if (stream[i] == '\n') {
      break;
    }
  }

  item->start = stream;
  item->length = i;
  return (const MultiseqItem *) item;
}

static MultiseqItemSpace * muliseq_init_items() {
  MultiseqItemSpace * space = NULL;
  realloc_or_exit(space, sizeof(*space), "Can not allocate memory");

  space->size = 10UL;
  space->count = 0UL;
  space->elements = NULL;
  realloc_or_exit(space->elements, sizeof(*space->elements) * space->size,
      "Can not allocate memory");

  return space;
}

static bool muliseq_item_start(char * stream) {
  return (stream[0] == '\n' && stream[1] != '>');
}

Multiseq * muliseq_new(const char * filename) {
  unsigned long file, i;

  Multiseq * multiseq = NULL;
  assert_with_message(filename != NULL, "filename can not be NULL");
  realloc_or_exit(multiseq, sizeof(*multiseq), "Can not allocate memory");

  multiseq->filename = filename;
  multiseq->items = muliseq_init_items();
  multiseq->paars = muliseq_init_paars();

  multiseq->status = NULL;
  realloc_or_exit(multiseq->status, sizeof(*multiseq->status),
      "Can not allocate memory");

  open_or_exit(multiseq->filename, file, multiseq->status);
  mmap_or_exit(multiseq->filemap, file, multiseq->status);

  for (i = 0; (i + 1) < multiseq->status->st_size; i++) {
    if (muliseq_item_start(&multiseq->filemap[i])) {
      const MultiseqItem * item = muliseq_fill_item(multiseq,
          &multiseq->filemap[i + 1]);
      i += item->length;
    }
  }

  /* we do know now a real table space, reduce memory usage to this value*/
  muliseq_resize_items(multiseq->items, multiseq->items->count);
  /* we do know now a count of sequences, that means that we do know
   * a all - to - all table size too, resize this table to use
   * with multiple threads */
  muliseq_resize_paar(multiseq->paars,
      multiseq->items->count * multiseq->items->count);

  return multiseq;
}

unsigned long muliseq_items(const Multiseq * multiseq) {
  assert_with_message(multiseq != NULL, "Multiseq can not be NULL");
  MultiseqItemSpace * space = multiseq->items;
  return space->count;
}

unsigned long muliseq_paars(const Multiseq * multiseq) {
  assert_with_message(multiseq != NULL, "Multiseq can not be NULL");
  MultiseqPaarSpace * space = multiseq->paars;
  return space->count;
}

const MultiseqItem * muliseq_item(const Multiseq * multiseq,
                                  const unsigned long i) {
  assert_with_message(multiseq != NULL, "Multiseq can not be NULL");
  MultiseqItemSpace * space = multiseq->items;
  assert_with_message((space->count > i), "Not existed element requested");
  return (const MultiseqItem *) &space->elements[i];
}

const MultiseqPaar * muliseq_paar(const Multiseq * multiseq,
                                  const unsigned long i) {
  assert_with_message(multiseq != NULL, "Multiseq can not be NULL");
  MultiseqPaarSpace * space = multiseq->paars;
  assert_with_message((space->count > i), "Not existed element requested");
  return (const MultiseqPaar *) &space->elements[i];
}

void muliseq_delete(Multiseq * multiseq) {
  if (multiseq) {
    if (multiseq->items) {
      free(multiseq->items->elements);
      free(multiseq->items);
    }
    if (multiseq->paars) {
      free(multiseq->paars->elements);
      free(multiseq->paars);
    }
    if (multiseq->status) {
      munmap(multiseq->filemap, multiseq->status->st_size);
      free(multiseq->status);
    }
    free(multiseq);
  }
}

MultiseqTreadSpace * multiseq_threads(const unsigned long t) {
  MultiseqTreadSpace * space = NULL;
  realloc_or_exit(space, sizeof(*space) * t, "Can not allocate memory");

  space->count = t;
  space->current = 0UL;
  space->treads = NULL;
  realloc_or_exit(space->treads, sizeof(*space->treads) * space->count,
      "Can not allocate memory");

  return space;
}

MultiseqTread * multiseq_thread_next(MultiseqTreadSpace * space) {
  assert_with_message(space != NULL, "space can not be NULL");
  if (space->current < space->count) {
    MultiseqTread * element = &space->treads[space->current];
    element->id = space->current++;
    return element;
  }
  space->current = 0UL;
  return NULL;
}

void multiseq_threads_delete(MultiseqTreadSpace * space) {
  if (space) {
    free(space->treads);
    free(space);
  }
}

