/*
 ============================================================================
 Name        : intset-simple.c
 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : a simple intset implementation
 ============================================================================
 */


/*
Zusatzfrage 1:
Da die Operation des herraussuchens aus der Menge der
Seperatorzeichen eine vergleichsweise teure Operation
ist, sollte das lambda so gewaehlt werden, dass es der
seltensten Base entspricht.
Dies hat den Grund, dass wenn man die heaufigste Base
weahlen wuerde, beim lesen dieser Base geschaut
werden muss, ob es sich um ein Seperatorzeichen handelt.
Wenn jetzt es jetzt Beispielweise eine Verteilung von
(60%, 20%, 15%, 5%) existiert so wuerde bei 60% aller Faelle
eine binaere suche durchgefuehrt werden mussen um zu
pruefen, ob es sich um ein Seperatorzeichen handel,
obwohl eigentlich nur bei besserer wahl 5% aller Basen
geprueft werden muessten.
*/

/*
Zusatzfrage 2:
Wenn die Sequenzen die gleiche laenge haben, dann laesst
sich die Summe zur Berechnung von
p_j mit n = n_j vereinfachen zu

p_j = j + sum n_j from 0 to j
    = j + sum n from 0 to j
Dies ist das gleiche wie
    = j + (j + 1) * n

Somit ist die Menge der Seperatorzeichen
S = {n, 2*n+1, 3*n+2, 4*n+3, 5*n+4, ...}

Setzt man l = j + 1 so folgt
      j + (j + 1) * n
 <=>  (l-1) + l*n
    = l*(n + 1) - 1

Somit befindet sich jedes Seperatorzeichen an
der l*(n + 1) - 1 Stelle
Wenn nun ein Element gesucht werden soll,

  x = l*(n + 1) - 1 gesetzt werden und es folgt

  l = (x + 1) / (n + 1)

Somit ergibt sich, dass das gegebene Element genau dann
ein Seperatorzeichen ist, wenn l eine natuerliche Zahl ist.
Dies kann mittels der Operation

          /
          \
          / (x + 1) modulo (n + 1) - 1 == 0 fuer l ist Seperatorzeichen
  f_l(x) =\
          /                             sonst l ist kein Seperatorzeichen
          \

geprueft werden.
Somit kann die funktion wie folgt implementiert werden

bool intset_is_member(const IntSet *intset, unsigned long elem)
{
  bool result = false;
  if ((elem + 1) % (intset->sequenzsize + 1) == 0)
  {
    result = true;
  }

  return result;
}

Hierbei muesste dem Intset aber noch die groesse der Sequenz uebergeben
werden.

Bsp:
    1    2    3    4   Nummer des Seperatorzeichens

ABCD$ABCD$ABCD$ABCD$   Sequenz
    ^    ^    ^    ^
    4    9    14   19  Position der Bits

n = 4
S = {4, 9, 14, 19}

(4 + 1)  % (4 + 1) = 5  % 5 = 0
(9 + 1)  % (4 + 1) = 10 % 5 = 0
(14 + 1) % (4 + 1) = 15 % 5 = 0
(19 + 1) % (4 + 1) = 20 % 5 = 0
*/

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "intset.h"

#define save_malloc(x, size) \
  if ( !(x = malloc(size)) ) { \
    fprintf(stderr, "Not enough memory for an new object of size %lu\n", \
            (unsigned long) size); \
    exit(EXIT_FAILURE); \
  }

struct IntSet
{
  unsigned long* elements; /* pointer to all elements */
  unsigned long maxvalue, /* the maximal value of an element */
                count, /* the maximal count of elements */
                current; /* the current count of elements */
};

IntSet *intset_new(unsigned long maxvalue, unsigned long nofelements)
{
  IntSet* set = NULL;
  assert(nofelements <= maxvalue);
  save_malloc(set, sizeof(*set));
  save_malloc(set->elements, sizeof(*set->elements) * nofelements);

  set->maxvalue = maxvalue;
  set->count = nofelements;
  set->current = 0;

  return set;
}

size_t intset_size(unsigned long maxvalue, unsigned long nofelements)
{
  assert(nofelements <= maxvalue);

  /* maxvalue is not needed here because we need it only
     to check if there are doublicates in the intset. */
  return sizeof(nofelements) * nofelements + sizeof(IntSet);
}

void intset_delete(IntSet *intset)
{
  if (intset)
  {
    free(intset->elements);
    intset->elements = NULL;
    free(intset);
    intset = NULL;
  }
}

void intset_add(IntSet *intset, unsigned long elem)
{
  assert(intset != NULL);
  assert(intset->elements != NULL);
  assert(elem <= intset->maxvalue);
  assert(intset->current < intset->count);

  if (intset->current > 0)
  {
    /* because its ensured, that all elements before
       inset->current are sorted and we
       add the element to the end of the set,
       we only need to check
       if the new element is bigger than the biggest element
       of the previous elements.
       The first element is everytime the biggest one,
       so we dont need to check it.
    */
    assert(intset->elements[intset->current - 1] < elem);
  }

  intset->elements[intset->current++] = elem;
}

/*
  Returns true if it found the given element in the intset.
  The search use binary search to ensure an fast as posible search.
*/
bool intset_is_member(const IntSet *intset, unsigned long elem)
{
  /* We found the element in O(log_2(k - 1)) because we use binary search! */
  bool found = false;
  assert(intset != NULL && intset->elements != NULL);

  /* small precheck to skip binary search if posible */
  if (elem <= intset->maxvalue)
  {
    /* binary search */
    unsigned long i, j;
    i = 0;
    j = intset->current;
    while (i <= j)
    {
      const unsigned long mid = i + ((j - i) / 2);
      if (intset->elements[mid] == elem)
      {
        found = true;
        break;
      }
      else if (intset->elements[mid] < elem)
      {
        i = mid + 1;
        /* if i is smaller than mid we get an overflow,
           and we didnt find the element. */
        if (i < mid || i >= intset->current)
        {
          break;
        }
      }
      else /* intset->elements[mid] > elem */
      {
        j = mid - 1;

        /* mid should be bigger if you subtract 1
           so we get an underflow and could find the element */
        if (mid < j || j >= intset->current)
        {
          break;
        }
      }
    }
  }

  return found;
}

/*
  returns 0
*/
unsigned long intset_number_next_larger(const IntSet *intset,
                                        unsigned long value)
{
  return 0;
}

/*
Prints the intset to the stdout
*/
void intset_pretty(const IntSet *intset)
{
  unsigned long i;
  assert(intset != NULL && intset->elements != NULL);
  if (intset->current > 0)
  {
    for (i = 0; i < intset->current - 1; ++i)
    {
      printf("%lu ", intset->elements[i]);
    }
    printf("%lu\n", intset->elements[i]);
  }
}
