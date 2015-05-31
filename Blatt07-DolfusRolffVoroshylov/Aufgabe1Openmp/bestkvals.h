#ifndef BESTKVALS_H
#define BESTKVALS_H

#include <stdio.h>

typedef struct BestKVals BestKVals;

/* Compare functionpointer type, should return 0 for equal values, x < 0 for a
   "better" b and x > 0 for b better a. (for classical compare: smaller is
   better!)
   */
typedef int (*BestKVals_cmp)(const void *a, const void *b);

/* Return new <Best_k> object. It will use <compare> to compare the elements to
   be stored, and <duplicate> to copy them. <size> is used for allocating
   appropriate memory, and should represent the size for one value.
   Uses simple memcpy for storing of values, therefore it is not possible to
   store values containing pointer to dynamic structures (no nested dynamic
   structures).
   Assumes smaller is better! */
BestKVals*  best_k_vals_new(unsigned long bestnumber,
                            BestKVals_cmp compare,
                            size_t size);

/* frees all memory */
void        best_k_vals_delete(BestKVals *bkv);

/* tests if <invalue> is among the best and, if it is, inserts it.
   Uses memcpy to store what is insize invalue. Invalie therefore can NOT
   contain pointers to non const elements!
   */
void        best_k_vals_insert(BestKVals *bkv, const void *invalue);

/* iterates over the best values; returns one value per call. */
const void* best_k_vals_next(BestKVals *bkv);

/* reset iterator to start */
void        best_k_vals_reset_iter(BestKVals *bkv);

#endif
