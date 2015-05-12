/*
 ============================================================================
 Name        : intset.c
 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : a efficient intset implementation
 ============================================================================
 */

#include "intset.h"

#include <assert.h>
#include <inttypes.h>

#include "assertions.h"

/*
Sei s1 der Speicherverbrauch der alten Implementierung mit
s1 = sizeof(IntSetOld) + sizeof(unsigned long) * n;

und sei s2 der Speicherverbrauch dieser neuen Implementierung,
mit
s2 = sizeof(IntSetNew) + sizeof(d value) * n
                       + sizeof(unsigned long) * (max / d + 1)

mit c1 = sizeof(IntSetOld), c2 = sizeof(IntSetNew) folgt
unter der Annahme, dass sizeof(d value) = a
und sizeof(unsigned long) = b

so ist

s1 = c1 + b * l
und
s2 = c2 + a * l + b * (m / d + 1)

so ist s2 < s1 wenn

    s2 < s1
<=> c2 + a * l + b * (m/d + 1) < c2 + b * l
<=> m < d/b * (b * (l - 1) - a * l + c1 - c2)

Somit folgt fuer die gegebene spezifische
Implementierung fuer eine Maschine mit den Datentyp
groessen a = 2 Byte und b = 8 Byte und
den Konstanten c1 = 5 * 8 Byte, c2 = 4 * 8 Byte, dass

    m < d * (3/4 * l - 2)

Da auf Grund der Wahl von a = 2 dass d = 65536 dass

    m < 16384 * (3 * l - 8)

Somit ergibt sich die Gradengleichung

    y = 16384 * (3 * x - 8)

bei der alle Werte unterhalb der Grade weniger Speicher
benoetigen und somit die neue Implementierung besser ist
und alle Werte oberhalb die Alte Implementierung weniger Speicher
benoetigt.
*/


#define DIVISION_SHIFT 16    /* bit size of uint16_t */
#define DIVISION_CONST 65536 /* 2^16 */

#define divide(x) ((x) >> 16)
#define modulo(x) ((x) & (DIVISION_CONST - 1))

#define default_check(set)             \
  assert(set != NULL);                 \
  assert(set->values != NULL);         \
  assert(set->section_starts != NULL);

struct IntSet
{
  uint16_t*      values;
  unsigned long* section_starts;
  unsigned long  current_size,
                 maxvalue,
                 last_element;
};

static bool binarySearch(const IntSet* set, unsigned long elem,
                         unsigned long start, unsigned long end)
{
  default_check(set);
  if (start < set->current_size && end < set->current_size)
  {
    while (start <= end)
    {
      const unsigned long mid = start + ((end - start) / 2);
      if (set->values[mid] == elem)
      {
        return true;
      }
      else if (set->values[mid] < elem)
      {
        start = mid + 1;
        if (start < mid || start >= set->current_size)
        {
          return false;
        }
      }
      else /* set->values[mid] > elem */
      {
        end = mid - 1;
        if (mid < end || end >= set->current_size)
        {
          break;
        }
      }
    }
  }

  return false;
}

IntSet *intset_new(unsigned long maxvalue, unsigned long nofelements)
{
  assert(nofelements <= maxvalue);

  IntSet* set;
  malloc_or_exit(set, sizeof(*set));
  calloc_or_exit(set->values, 1, sizeof(*set->values) * nofelements);
  calloc_or_exit(set->section_starts, sizeof(*set->section_starts),
                 divide(maxvalue) + 1);
  set->maxvalue = maxvalue;
  set->current_size =
  set->last_element = 0;

  return set;
}

size_t intset_size(unsigned long maxvalue, unsigned long nofelements)
{
  assert(nofelements <= maxvalue);

  return sizeof(IntSet) + sizeof(uint16_t) * nofelements
                        + sizeof(unsigned long) * (divide(maxvalue) + 1);
}

void intset_delete(IntSet *intset)
{
  if (intset)
  {
    free(intset->values);
    free(intset->section_starts);
    free(intset);
  }
}

void intset_add(IntSet *intset, unsigned long elem)
{
  assert(elem <= intset->maxvalue);
  default_check(intset);
  assert((intset->current_size > 0 && intset->last_element < elem) ||
          intset->current_size == 0);

  const unsigned long q = divide(elem), r = modulo(elem),
                      old = divide(intset->last_element);
  /* check if a new section begins */
  if (old < q)
  {
    unsigned long i;
    /* if thats the case set unset starts to the current start */
    for (i = old + 1; i <= q; i++)
    {
      intset->section_starts[i] = intset->current_size;
    }
  }

  intset->values[intset->current_size++] = r;
  intset->last_element = elem;
}

bool intset_is_member(const IntSet *intset, unsigned long elem)
{
  default_check(intset);
  if (elem <= intset->last_element)
  {
    const unsigned long q    = divide(elem),
                        r    = modulo(elem),
                        endq = divide(intset->last_element);
    unsigned long start = intset->section_starts[q],
                  end;
    /* check if we reach the end of the sections */
    if (q == endq)
    {
      /* if thats the case we sent the end to the current size */
      end   = intset->current_size - 1;
    }
    else
    {
      /* if thats not the case the end is the start + size of the section */
      end   = intset->section_starts[q + 1] - 1;
    }

    return binarySearch(intset, r, start, end);
  }

  return false;
}

unsigned long intset_number_next_larger(const IntSet *intset,
                                        unsigned long pos)
{
  return 0;
}

void intset_pretty(const IntSet *intset)
{
  default_check(intset);
  unsigned long i, q;
  const unsigned long sections = divide(intset->maxvalue) + 1;

  for (i = q = 0; i < intset->current_size; ++i)
  {
    while (q < sections && i == intset->section_starts[q + 1])
    {
      ++q;
    }

    printf("%lu\n", q * DIVISION_CONST + intset->values[i]);
  }
}
