#include <stdlib.h>
#include "unit_edist.h"

unsigned long eval_unit_edist(const unsigned char *u,
                              unsigned long m,
                              const unsigned char *v,
                              unsigned long n)
{
  unsigned long val, we, nw, *ecol, *ecolptr;
  const unsigned char *uptr, *vptr;

  ecol = (unsigned long *) malloc(sizeof *ecol * (m+1));
  for (*ecol = 0, ecolptr = ecol+1, uptr = u; uptr < u + m; ecolptr++, uptr++)
  {
    *ecolptr = *(ecolptr-1) + 1;
  }
  for (vptr = v; vptr < v + n; vptr++)
  {
    nw = *ecol;
    *ecol = nw + 1;
    for (ecolptr = ecol+1, uptr = u; uptr < u + m; ecolptr++, uptr++)
    {
      we = *ecolptr;
      *ecolptr = *(ecolptr-1) + 1;
      if (*uptr == *vptr)
      {
        val = nw;
      } else
      {
        val = nw + 1;
      }
      if (val < *ecolptr)
      {
        *ecolptr = val;
      }
      if ((val = we + 1) < *ecolptr)
      {
        *ecolptr = val;
      }
      nw = we;
    }
  }
  val = *(ecolptr-1);
  free(ecol);
  return val;
}
