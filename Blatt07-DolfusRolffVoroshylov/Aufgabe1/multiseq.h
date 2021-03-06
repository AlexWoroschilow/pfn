#ifndef MULTISEQ_H
#define MULTISEQ_H

typedef unsigned long (*EvalMultiseq)(const unsigned char *u, unsigned long m,
    const unsigned char *v, unsigned long n);

typedef struct MultiseqItemSpace MultiseqItemSpace;
typedef struct MultiseqPaarSpace MultiseqPaarSpace;

typedef struct MultiseqItem {
  char * start;
  unsigned long length;
} MultiseqItem;

typedef struct MultiseqPaar {
  unsigned long id1, id2, distance;
} MultiseqPaar;

typedef struct Multiseq {
  char * filemap;
  const char * filename;
  struct stat * status;
  MultiseqItemSpace * items;
  MultiseqPaarSpace * paars;
} Multiseq;

typedef struct MultiseqTread {
  unsigned long id, i, j;
  pthread_t identifier;
  EvalMultiseq compare;
  Multiseq * multiseq;
} MultiseqTread;

typedef struct MultiseqTreadSpace {
  unsigned long count, current;
  MultiseqTread * treads;
} MultiseqTreadSpace;

Multiseq * muliseq_new(const char * filename);

unsigned long muliseq_items(const Multiseq * multiseq);
unsigned long muliseq_paars(const Multiseq * multiseq);

const MultiseqItem * muliseq_item(const Multiseq * multiseq,
                                  const unsigned long i);
const MultiseqPaar * muliseq_paar(const Multiseq * multiseq,
                                  const unsigned long i);

const MultiseqItem * muliseq_fill_item(Multiseq * multiseq,
                                       char * stream);
const MultiseqPaar * muliseq_fill_paar(Multiseq * multiseq,
                                       const unsigned long distance,
                                       const unsigned long id1,
                                       const unsigned long id2);

void muliseq_delete(Multiseq * multiseq);

MultiseqTreadSpace * multiseq_threads(const unsigned long t);
MultiseqTread * multiseq_thread_next(MultiseqTreadSpace * space);
void multiseq_threads_delete(MultiseqTreadSpace * space);

#endif
