#ifndef INTSET_HPP
#define INTSET_HPP

#include <cstdlib>

template <typename Basetype>
class IntSet
{
  // we use c-style arrays for speed and to make it easier to adapt the "old"
  // code from previous exercises
  Basetype*      elements;
  unsigned long* sectionstart;
  unsigned long  nextfree,
                 maxelement,
                 currentsectionnum,
                 numofsections,
                 nofelements,
                 previouselem;
  int            logsectionsize;

  public:
    IntSet(unsigned long maxelement, unsigned long nofelements);
    ~IntSet();
    void add(unsigned long elem);
    bool is_member(unsigned long elem) const;
    unsigned long number_next_larger(unsigned long pos) const;
    void pretty_print(void) const;

    static size_t size(unsigned long maxelement,
                       unsigned long nofelements);
};

#include "intset-impl.hpp"

#endif
