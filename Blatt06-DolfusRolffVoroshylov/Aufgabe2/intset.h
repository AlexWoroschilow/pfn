#ifndef INTSET_H
#define INTSET_H

#include <stdbool.h>
#include <stdlib.h>

/* The <IntSet> class, used to store sorted integer sets. */
typedef struct IntSet IntSet;

/* Return a new <IntSet> Object with space allocated for <nofelements> elements
   [0,<maxvalue>] */
IntSet *intset_new(unsigned long maxvalue, unsigned long nofelements);

/* Returns the size needed to store <nofelements> integers smaller equal
   <maxvalue> */
size_t intset_size(unsigned long maxvalue, unsigned long nofelements);

/* Free the memory of <intset> */
void intset_delete(IntSet *intset);

/* Add <elem> to <intset>. Fails if <elem> is larger <maxvalue> or <intset>
   already contains <nofelements> elements. */
void intset_add(IntSet *intset, unsigned long elem);

/* True if <elem> is stored in <intset> */
bool intset_is_member(const IntSet *intset, unsigned long elem);

/* Return the index/number (0 based) of the smallest element that is larger then
   <value> from <intset> or <nofelements> if there is no such element.
   Fails if <pos> is member of <intset> */
/* Returns 0 for most exercises, Implementation of this method will be done on a
   separate exercise! */
unsigned long intset_number_next_larger(const IntSet *intset,
                                        unsigned long pos);

/* Print out the <intset> to stdout */
void intset_pretty(const IntSet *intset);

#endif
