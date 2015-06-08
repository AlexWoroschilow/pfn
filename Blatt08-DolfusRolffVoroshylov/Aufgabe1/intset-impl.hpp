#include <stdio.h>
#include <stdlib.h>

#include <cstring>
#include <algorithm>
#include <limits>
#include <iostream>

#include "intset.hpp"

#define BITS_FOR_SIZE(SIZE)     ((SIZE) * CHAR_BIT)
#define ELEM_TO_SECTION(LOGVAL,X)  ((X) >> (LOGVAL))
#define SECTION_MIN_ELEM(S)       ((S) << logsectionsize)

/* Class constructor, initialize variables with start values */
template<typename Basetype>
IntSet<Basetype>::IntSet(const unsigned long maximum,
                         const unsigned long count) :
  nextfree(0),
  maxelement(maximum),
  currentsectionnum(0),
  nofelements(count),
  previouselem(std::numeric_limits<unsigned long>::max()),
  logsectionsize(BITS_FOR_SIZE(sizeof(*elements))) {

  numofsections = ELEM_TO_SECTION(logsectionsize, maximum) + 1;

  // C++ template fix: avoid malloc, use C++ array constructors
  elements = new Basetype[nofelements];
  sectionstart = new unsigned long[numofsections + 1];

  //set all elements to nofelements
  std::fill(&sectionstart[0], &sectionstart[numofsections + 1], nofelements);
  sectionstart[0] = 0;
}

/* Just a class destructor */
template<typename Basetype>
IntSet<Basetype>::~IntSet() {
  delete[] elements;
  delete[] sectionstart;
}

/* Append new value to set, just a copy-paste from c-file */
template<typename Basetype>
void IntSet<Basetype>::add(unsigned long elem) {
  assert(nextfree < nofelements && elem <= maxelement &&
        (previouselem == std::numeric_limits<unsigned long>::max() ||
         previouselem < elem));

  while (elem >= SECTION_MIN_ELEM(currentsectionnum + 1)) {
    assert(currentsectionnum < numofsections);
    sectionstart[currentsectionnum + 1] = nextfree;
    currentsectionnum++;
  }

  assert(SECTION_MIN_ELEM(currentsectionnum) <= elem &&
         elem < SECTION_MIN_ELEM(currentsectionnum + 1) &&
         ELEM_TO_SECTION(logsectionsize,elem) == currentsectionnum);

  /* C++ template fix: Convert element to given type  */
  elements[nextfree++] = static_cast<Basetype>(elem);
  previouselem = elem;
}

/* Check is a number already in set */
template<typename Basetype>
bool IntSet<Basetype>::is_member(unsigned long check) const {
  if (check <= maxelement) {
    const unsigned long sectionnum = ELEM_TO_SECTION(logsectionsize, check);
    if (sectionstart[sectionnum] < sectionstart[sectionnum + 1]) {
      // Search by sections with standard binary search in O(log(n))
      return std::binary_search(&elements[sectionstart[sectionnum]],
                                &elements[sectionstart[sectionnum + 1]],
                                static_cast<Basetype>(check));
    }
  }
  return false;
}

/* Get a next larger number */
template<typename Basetype>
unsigned long
IntSet<Basetype>::number_next_larger(const unsigned long pos) const {
  const unsigned long sectionnum = ELEM_TO_SECTION(logsectionsize, pos);
  assert(pos <= maxelement);

  if (sectionstart[sectionnum] < sectionstart[sectionnum + 1]) {
    // search with the default implementation for next value larger in O(log(n))
    Basetype* found = std::upper_bound(&elements[sectionstart[sectionnum]],
                                       &elements[sectionstart[sectionnum + 1]],
                                       static_cast<Basetype>(pos));
    // compute index by found because found could start at elements[0]
    assert(*found != pos);
    assert(found != NULL &&
           found >= &elements[sectionstart[sectionnum]] &&
           found >= elements);
    return found - elements;
  }

  return nofelements;
}

/* Function to print a set, just a copy-paste from c-file */
template<typename Basetype>
void IntSet<Basetype>::pretty_print() const {
  unsigned long idx, sectionnum = 0;
  assert(nextfree > 0);

  for (idx = 0; idx < nextfree; idx++) {
    while (idx >= sectionstart[sectionnum + 1]) {
      sectionnum++;
    }
    printf("%lu%s", SECTION_MIN_ELEM(sectionnum) + elements[idx],
           idx < nextfree - 1 ? "&" : "\\\\\n");
  }
  for (idx = 0; idx < nextfree; idx++) {
    printf("%hu%s", elements[idx], idx < nextfree - 1 ? "&" : "\\\\\n");
  }
  sectionnum = 0;
  for (idx = 0; idx < nextfree; idx++) {
    while (idx >= sectionstart[sectionnum + 1]) {
      sectionnum++;
    }
    printf("%lu%s", sectionnum, idx < nextfree - 1 ? "&" : "\\\\\n");
  }
  for (idx = 0; idx <= numofsections; idx++) {
    printf("%lu%s", sectionstart[idx], idx < numofsections ? "&" : "\\\\\n");
  }
}

/* Function to get a size, copy-paste from c-file*/
template<typename Basetype>
size_t IntSet<Basetype>::size(const unsigned long maxelement,
                              const unsigned long nofelements) {
  int logsectionsize = BITS_FOR_SIZE(sizeof(Basetype));
  return sizeof(Basetype) * nofelements +
         sizeof(unsigned long) * (ELEM_TO_SECTION(logsectionsize,
                                                  maxelement) + 1);
}