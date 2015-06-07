#include <stdio.h>
#include <stdlib.h>
#include "intset.hpp"

#define BITS_FOR_SIZE(SIZE)     ((SIZE) * CHAR_BIT)
#define ELEM2SECTION(LOGVAL,X)  ((X) >> (LOGVAL))
#define SECTIONMINELEM(S)       ((S) << this->logsectionsize)

/* Class constructor, initialize variables with start values
 * C++ fix: use variable "this"*/
template<typename Basetype>
IntSet<Basetype>::IntSet(unsigned long maxelement, unsigned long nofelements) {

  this->nextfree = 0;
  this->logsectionsize = BITS_FOR_SIZE(sizeof(*this->elements));
  this->numofsections = ELEM2SECTION(this->logsectionsize,maxelement) + 1;
  this->maxelement = maxelement;
  this->currentsectionnum = 0;
  this->nofelements = nofelements;
  this->previouselem = ULONG_MAX;

  /* C++ template fix: avoid malloc, use C++ array constructors  */
  this->elements = new Basetype[this->nofelements];
  this->sectionstart = new unsigned long[this->numofsections + 1];

  this->sectionstart[0] = 0;
  for (unsigned long idx = 1; idx <= this->numofsections; idx++) {
    this->sectionstart[idx] = this->nofelements;
  }
}

/* Just a class destructor */
template<typename Basetype>
IntSet<Basetype>::~IntSet() {
  delete[] this->elements;
  delete[] this->sectionstart;
}

/* Append new value to set, just a copy-paste from c-file
 * C++ fix: use variable "this" */
template<typename Basetype>
void IntSet<Basetype>::add(unsigned long elem) {
  assert(
      this->nextfree < this->nofelements && elem <= this->maxelement
          && (this->previouselem == ULONG_MAX || this->previouselem < elem));

  while (elem >= SECTIONMINELEM(this->currentsectionnum + 1)) {
    assert(this->currentsectionnum < this->numofsections);
    this->sectionstart[this->currentsectionnum + 1] = this->nextfree;
    this->currentsectionnum++;
  }

  assert(
      SECTIONMINELEM(this->currentsectionnum) <= elem
          && elem < SECTIONMINELEM(this->currentsectionnum + 1) &&
          ELEM2SECTION(this->logsectionsize,elem) == this->currentsectionnum);

  /* C++ template fix: Convert element to given type  */
  this->elements[this->nextfree++] = (Basetype) elem;
  this->previouselem = elem;
}

/* Check is a number already in set
 * C++ fix: use variable "this", use Basetype */
template<typename Basetype>
bool IntSet<Basetype>::is_member(unsigned long check) const {

  const Basetype *midptr, *leftptr, *rightptr, *elem;

  /* needs to work with unsigned long
   * to find a position range */
  if (check <= this->maxelement) {

    const unsigned long sectionnum = ELEM2SECTION(this->logsectionsize, check);

    if (this->sectionstart[sectionnum] < this->sectionstart[sectionnum + 1]) {
      /* needs to work with Basetype to find
       * element in array */
      elem = (const Basetype*) &check;
      leftptr = (this->elements + this->sectionstart[sectionnum]);
      rightptr = (this->elements + this->sectionstart[sectionnum + 1] - 1);

      /* just a copy-paste binary search */
      while (leftptr <= rightptr) {
        midptr = leftptr + (((unsigned long) (rightptr - leftptr)) >> 1);
        if (*elem < *midptr) {
          rightptr = midptr - 1;
        } else {
          if (*elem > *midptr) {
            leftptr = midptr + 1;
          } else {
            return true;
          }
        }
      }
    }
  }
  return false;
}

/* Get a next larger number
 * C++ fix: use variable "this", use Basetype */
template<typename Basetype>
unsigned long IntSet<Basetype>::number_next_larger(unsigned long pos) const {

  unsigned long sectionnum, result;
  const Basetype *midptr, *leftptr, *rightptr, *found, *leftorig, *elem;

  /* Needs to work with unsigned long here to
   * find a correct element position range*/
  sectionnum = ELEM2SECTION(this->logsectionsize, pos);
  result = this->sectionstart[sectionnum];

  assert(pos <= this->maxelement);

  if (pos > this->previouselem) {
    return this->nofelements;
  }

  if (this->sectionstart[sectionnum] < this->sectionstart[sectionnum + 1]) {

    found = NULL;
    /* Needs to work with Basetype
     * to find an element in array*/
    elem = (const Basetype*) &pos;
    leftptr = this->elements + this->sectionstart[sectionnum];
    rightptr = this->elements + this->sectionstart[sectionnum + 1] - 1;
    leftorig = leftptr;

    assert(leftptr <= rightptr);

    if (*elem < *leftptr) {
      return result;
    }

    if (*elem > *rightptr) {
      return result + 1UL + (unsigned long) (rightptr - leftptr);
    }

    assert(*elem > *leftptr && *elem < *rightptr);

    while (leftptr <= rightptr) {
      midptr = leftptr + ((unsigned long) (rightptr - leftptr) >> 1);
      if (*elem < *midptr) {
        rightptr = midptr - 1;
      } else {
        found = midptr;
        if (*elem > *midptr) {
          leftptr = midptr + 1;
        } else
          break;
      }
    }
    /* not allowed by exercise! */
    assert(*found != *elem);
    assert(found != NULL && found >= leftorig);
    return result + 1UL + (unsigned long) (found - leftorig);
  }

  return result;

}

/* Function to print a set, just a copy-paste from c-file,
 * C++ fix: use variable "this" */
template<typename Basetype>
void IntSet<Basetype>::pretty_print(void) const {
  unsigned long idx, sectionnum = 0;

  assert(this->nextfree > 0);
  for (idx = 0; idx < this->nextfree; idx++) {
    while (idx >= this->sectionstart[sectionnum + 1]) {
      sectionnum++;
    }
    printf("%lu%s", SECTIONMINELEM(sectionnum) + this->elements[idx],
        idx < this->nextfree - 1 ? "&" : "\\\\\n");
  }
  for (idx = 0; idx < this->nextfree; idx++) {
    printf("%hu%s", this->elements[idx],
        idx < this->nextfree - 1 ? "&" : "\\\\\n");
  }
  sectionnum = 0;
  for (idx = 0; idx < this->nextfree; idx++) {
    while (idx >= this->sectionstart[sectionnum + 1]) {
      sectionnum++;
    }
    printf("%lu%s", sectionnum, idx < this->nextfree - 1 ? "&" : "\\\\\n");
  }
  for (idx = 0; idx <= this->numofsections; idx++) {
    printf("%lu%s", this->sectionstart[idx],
        idx < this->numofsections ? "&" : "\\\\\n");
  }
}

/* Function to get a size, copy-paste from c-file*/
template<typename Basetype>
size_t IntSet<Basetype>::size(unsigned long maxelement,
    unsigned long nofelements) {
  int logsectionsize = BITS_FOR_SIZE(sizeof(Basetype));
  return sizeof(Basetype) * nofelements
      + sizeof(unsigned long) * (ELEM2SECTION(logsectionsize,maxelement) + 1);
}

