#ifndef SUBSETSUM_H
#define SUBSETSUM_H
#include <stdbool.h>
/* Returns true if the ordered set of numbers <arr> of size <n> contains a
   subset with the sum of its members equal to <k>, returns false if no such
   subset exists. Elements in <mark> will be set to true for all members of
   <arr> that are part of said subset. */
bool subsetsum(unsigned long k,
               bool *mark,
               const unsigned long *arr,
               unsigned long n);
#endif
