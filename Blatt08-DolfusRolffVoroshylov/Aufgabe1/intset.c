#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <assert.h>
#include <stdbool.h>
#include "intset.h"

struct IntSet
{
  uint16_t *elements;
  int logsectionsize;
  unsigned long nextfree,
                maxelement,
                currentsectionnum,
                numofsections,
                nofelements,
                previouselem,
                *sectionstart;
};

#define BITS_FOR_SIZE(SIZE)     ((SIZE) * CHAR_BIT)
#define ELEM2SECTION(LOGVAL,X)  ((X) >> (LOGVAL))
#define SECTIONMINELEM(S)       ((S) << intset->logsectionsize)

IntSet *intset_new(unsigned long maxelement,unsigned long nofelements)
{
  IntSet *intset = malloc(sizeof *intset);
  unsigned long idx;

  assert(intset != NULL);
  intset->elements = malloc(sizeof *intset->elements * nofelements);
  assert(intset->elements != NULL);
  intset->nextfree = 0;
  intset->logsectionsize = BITS_FOR_SIZE(sizeof(uint16_t));
  intset->numofsections = ELEM2SECTION(intset->logsectionsize,maxelement) + 1;
  intset->sectionstart = malloc(sizeof *intset->sectionstart *
                                (intset->numofsections+1));
  assert(intset->sectionstart != NULL);
  intset->sectionstart[0] = 0;
  for (idx = 1; idx <= intset->numofsections; idx++)
  {
    intset->sectionstart[idx] = nofelements;
  }
  intset->maxelement = maxelement;
  intset->currentsectionnum = 0;
  intset->nofelements = nofelements;
  intset->previouselem = ULONG_MAX;
  return intset;
}

size_t intset_size(unsigned long maxelement,unsigned long nofelements)
{
  int logsectionsize = BITS_FOR_SIZE(sizeof (uint16_t));
  return sizeof (uint16_t) * nofelements +
         sizeof (unsigned long) * (ELEM2SECTION(logsectionsize,maxelement) + 1);
}

void intset_delete(IntSet *intset)
{
  if (intset != NULL)
  {
    free(intset->elements);
    free(intset->sectionstart);
    free(intset);
  }
}

void intset_add(IntSet *intset,unsigned long elem)
{
  assert(intset->nextfree < intset->nofelements &&
         elem <= intset->maxelement &&
         (intset->previouselem == ULONG_MAX || intset->previouselem < elem));
  while (elem >= SECTIONMINELEM(intset->currentsectionnum+1))
  {
    assert(intset->currentsectionnum < intset->numofsections);
    intset->sectionstart[intset->currentsectionnum+1] = intset->nextfree;
    intset->currentsectionnum++;
  }
  assert(SECTIONMINELEM(intset->currentsectionnum) <= elem &&
         elem < SECTIONMINELEM(intset->currentsectionnum+1) &&
         ELEM2SECTION(intset->logsectionsize,elem) ==
           intset->currentsectionnum);
  intset->elements[intset->nextfree++] = (uint16_t) elem;
  intset->previouselem = elem;
}

static bool binarysearch_is_member(const uint16_t *leftptr,
                                   const uint16_t *rightptr,
                                   uint16_t elem)
{
  const uint16_t *midptr;

  while (leftptr <= rightptr)
  {
    midptr = leftptr + (((unsigned long) (rightptr-leftptr)) >> 1);
    if (elem < *midptr)
    {
      rightptr = midptr-1;
    } else
    {
      if (elem > *midptr)
      {
        leftptr = midptr + 1;
      } else
      {
        return true;
      }
    }
  }
  return false;
}

bool intset_is_member(const IntSet *intset,unsigned long elem)
{
  if (elem <= intset->maxelement)
  {
    const unsigned long sectionnum = ELEM2SECTION(intset->logsectionsize,elem);

    if (intset->sectionstart[sectionnum] < intset->sectionstart[sectionnum+1])
    {
      return binarysearch_is_member(intset->elements +
                                    intset->sectionstart[sectionnum],
                                    intset->elements +
                                    intset->sectionstart[sectionnum+1] - 1,
                                    (uint16_t) elem);
    }
  }
  return false;
}

void intset_pretty(const IntSet *intset)
{
  unsigned long idx, sectionnum = 0;

  assert(intset->nextfree > 0);
  for (idx = 0; idx < intset->nextfree; idx++)
  {
    while (idx >= intset->sectionstart[sectionnum+1])
    {
      sectionnum++;
    }
    printf("%lu%s",SECTIONMINELEM(sectionnum) + intset->elements[idx],
                   idx < intset->nextfree - 1 ? "&" : "\\\\\n");
  }
  for (idx = 0; idx < intset->nextfree; idx++)
  {
    printf("%hu%s",intset->elements[idx],
                   idx < intset->nextfree - 1 ? "&" : "\\\\\n");
  }
  sectionnum = 0;
  for (idx = 0; idx < intset->nextfree; idx++)
  {
    while (idx >= intset->sectionstart[sectionnum+1])
    {
      sectionnum++;
    }
    printf("%lu%s",sectionnum,
                   idx < intset->nextfree - 1 ? "&" : "\\\\\n");
  }
  for (idx = 0; idx <= intset->numofsections; idx++)
  {
    printf("%lu%s",intset->sectionstart[idx],
                   idx < intset->numofsections ? "&" : "\\\\\n");
  }
}

static unsigned long binarysearch_smallest_g(const uint16_t *leftptr,
                                             const uint16_t *rightptr,
                                             uint16_t pos)
{
  const uint16_t *midptr, *found = NULL, *leftorig = leftptr;

  assert(leftptr <= rightptr);
  if (pos < *leftptr)
  {
    return 0;
  }
  if (pos > *rightptr)
  {
    return 1UL + (unsigned long) (rightptr - leftptr);
  }
  assert(pos > *leftptr && pos < *rightptr);
  while (leftptr <= rightptr)
  {
    midptr = leftptr + ((unsigned long) (rightptr - leftptr) >> 1);
    if (pos < *midptr)
    {
      rightptr = midptr - 1;
    } else
    {
      found = midptr;
      if (pos > *midptr)
      {
        leftptr = midptr + 1;
      }
      else
        break;
    }
  }
  /* not allowed by exercise! */
  assert(*found != pos);
  assert(found != NULL && found >= leftorig);
  return 1UL + (unsigned long) (found - leftorig);
}

unsigned long intset_number_next_larger(const IntSet *intset,unsigned long pos)
{
  unsigned long sectionnum = ELEM2SECTION(intset->logsectionsize,pos),
                result = intset->sectionstart[sectionnum];

  assert(pos <= intset->maxelement);

  if (pos > intset->previouselem)
    return intset->nofelements;

  if (intset->sectionstart[sectionnum] < intset->sectionstart[sectionnum+1])
  {
    result += binarysearch_smallest_g(
                     intset->elements + intset->sectionstart[sectionnum],
                     intset->elements + intset->sectionstart[sectionnum+1] - 1,
                     (uint16_t) pos);
  }
  return result;
}

