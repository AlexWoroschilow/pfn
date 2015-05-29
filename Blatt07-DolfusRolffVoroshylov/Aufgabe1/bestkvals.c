#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bestkvals.h"

/*
   This file implements the datatype <BestKVals>.
   The values are stored in a array of size <bestnumber>,
   while references to the matches are stored in double linked list.
   */

typedef struct List_element {
  void                *value;    /* the value stored in the List_element */
  struct List_element *next,     /* next List_element */
                      *previous; /* previous List_element */
} List_element;

struct BestKVals {
  List_element *space,      /* space for listelements */
               *firstelem,  /* first element in sorted double linked list */
               *lastelem,   /* last element in sorted double linked list */
               *next,       /* next element for best_k_vals_next */
               *nextfree;   /* next unused element in space */
  char         *values;     /* space to store values */
  unsigned long bestnumber; /* = k, number of elements allocated in space */
  BestKVals_cmp compare;    /* function pointer used to compare values */
  size_t size;              /* size of one value */
};

BestKVals *best_k_vals_new(unsigned long bestnumber,
                           BestKVals_cmp compare,
                           size_t size)
{
  BestKVals *bkv;

  bkv = (BestKVals *) malloc(sizeof (*bkv));
  if (bkv == NULL) {
    fprintf(stderr, "malloc of bkv failed: not enough space\n");
    exit(EXIT_FAILURE);
  }
  bkv->space = malloc(sizeof (*bkv->space) * bestnumber);
  if (bkv->space == NULL) {
    fprintf(stderr, "malloc of bkv->space failed: not enough space\n");
    exit(EXIT_FAILURE);
  }
  bkv->bestnumber = bestnumber;
  fprintf(stderr, "best: %lu\n", bestnumber);
  bkv->compare = compare;
  bkv->firstelem = bkv->lastelem = bkv->next = NULL;
  bkv->nextfree = bkv->space;
  bkv->size = size;
  bkv->values = malloc(size * bestnumber);
  return bkv;
}

void best_k_vals_insert(BestKVals * bkv, const void *invalue)
{
  List_element *ins_ptr,         /*points to first larger element */
               *new_ptr,         /*points to new storage position */
               *prev_ptr = NULL; /*points to previous position */

  if (bkv->nextfree == bkv->space) { /*list is empty */
    bkv->nextfree->next = NULL;
    bkv->nextfree->previous = NULL;
    bkv->nextfree->value = memcpy(bkv->values, invalue, bkv->size);
    bkv->next = bkv->firstelem = bkv->nextfree;
    bkv->lastelem = bkv->nextfree;
    bkv->nextfree++;
  }
  else {
    /*add value, because k' < k, OR new value is smaller = better then largest */
    assert(bkv->lastelem != NULL);
    if (bkv->nextfree - bkv->space <= bkv->bestnumber - 1 ||
        bkv->compare(invalue, bkv->lastelem->value) < 0) {
      /*add value */
      if (bkv->nextfree - bkv->space <= bkv->bestnumber - 1) {
        new_ptr = bkv->nextfree;
        new_ptr->value =
          bkv->values + (bkv->size * (bkv->nextfree - bkv->space));
        bkv->nextfree++;
      }
      else {                           /*replace largest */
        new_ptr = bkv->lastelem;
        bkv->lastelem = bkv->lastelem->previous;
        bkv->lastelem->next = NULL;
      }
      new_ptr->value = memcpy(new_ptr->value, invalue, bkv->size);
      new_ptr->next = NULL;
      new_ptr->previous = NULL;
      /*find first larger element */
      for (ins_ptr = bkv->firstelem; ins_ptr != NULL; ins_ptr = ins_ptr->next) {
        if (bkv->compare(invalue, ins_ptr->value) < 0) {
          break;
        }
        prev_ptr = ins_ptr;
      }
      if (prev_ptr == NULL) {          /*add new first element */
        new_ptr->next = bkv->firstelem;
        bkv->firstelem->previous = new_ptr;
        bkv->next = bkv->firstelem = new_ptr;
        new_ptr->previous = NULL;
      }
      else {
        if (ins_ptr == NULL) {         /*add new last element */
          new_ptr->previous = bkv->lastelem;
          bkv->lastelem->next = new_ptr;
          bkv->lastelem = new_ptr;
          new_ptr->next = NULL;
        }
        else {                         /*insert between ins und prev ptr */
          new_ptr->previous = prev_ptr;
          prev_ptr->next = new_ptr;
          new_ptr->next = ins_ptr;
          ins_ptr->previous = new_ptr;
        }
      }
    }
  }
}

void best_k_vals_delete(BestKVals * bkv)
{
  if (bkv != NULL) {
    free(bkv->space);
    free(bkv->values);
    free(bkv);
  }
}

const void *best_k_vals_next(BestKVals * bkv)
{
  void *value;

  if (bkv->next == NULL) {
    fprintf(stderr, "%s: cannot access next value\n", __func__);
    exit(EXIT_FAILURE);
  }
  value = bkv->next->value;
  bkv->next = bkv->next->next;
  return value;
}

void best_k_vals_reset_iter(BestKVals *bkv)
{
  bkv->next = bkv->firstelem;
}
