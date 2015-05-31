#ifndef UNIT_EDIST_H
#define UNIT_EDIST_H

/* the following function returns the unit edist distance
   for sequence u of length m and sequence v of length n */
unsigned long eval_unit_edist(const unsigned char *u,
                              unsigned long m,
                              const unsigned char *v,
                              unsigned long n);

#endif
