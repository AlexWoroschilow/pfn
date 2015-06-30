#ifndef BWT_COMPRESS_H
#define BWT_COMPRESS_H

#include <stdbool.h>
#include "gt-defs.h"

/* The following function returns the BWT for a <sequence> of length
   <seqlength>.
   The suffix array <sa> for <sequence> is provided as the second
   argument. The returned pointer references a memory area storing the
   BWT of length <seqlength>+1. <longest> is the address to
   the row of the longest suffix, i.e.\ the index <i> satisfying sa[i]=0.
   For this index, bwt[i] is undefined, but should be set to some arbitrary
   value which should never be accessed. */

GtUchar *bwt_encode(unsigned long *longest,const Uint *sa,
                    const GtUchar *sequence,unsigned long seqlength);

/* The following function takes the BWT for a sequence of length <seqlength>
   and decodes the original sequence. To do this, additionally the
   index <longest> must be provided as well as the alphabet size <numofchars>
   which is usually set to UCHAR_MAX+1. */

GtUchar *bwt_decode(unsigned long seqlength,const GtUchar *bwt,
                    unsigned long longest,unsigned long numofchars);

/* The following function checks that bwt_encode and bwt_decode work
   correctly for the given <sequence> of length <seqlength> over an
   alphabet of size <numofchars> and the corresponding suffix array
   <sa>. The check is performed by first constructing
   the BWT using bwt_encode. The BWT is the decoded and stored in
   another memory area. Finally it is checked that the decoded sequence
   and the original sequence are identical. If any difference occurs
   the function reports this and exits with an exit code different
   from 0. */

void bwt_check(const Uint *sa,const GtUchar *sequence,
               unsigned long seqlength,unsigned long numofchars);

/* The following function computes the distribution of the length of the
   runs in the bwt for the given <sequence> of length <seqlength>.
   The suffix array for <sequence> is referred to by <suftab>. If
   <silent> is true, then no output is shown. */

void bwt_runlengthdist(bool silent,const Uint *suftab,const GtUchar *sequence,
                       unsigned long seqlength);

/* The following function computes the Move-to-front encoding of the Bwt for
   the given <sequence> of length <seqlength>.
   The suffix array for <sequence> is referred to by <suftab> and
   <numofchars> is the number of symbols. The pointer <longest> refers
   to a variable in which the index <idx> satisfying SUF[idx] = 0 is stored.
   The method works by computing the bwt character by character directly
   applying the MTF to the character. That is, the Bwt is not stored. */

GtUchar *mtf_encode(GtUchar * a, unsigned long *longest,const Uint *suftab,
                    const GtUchar *sequence,unsigned long seqlength,
                    unsigned long numofchars);

/* The following function decodes the bwt from the MTF for a sequence of
   length <seqlength> over an alphabet of <numofchars> symbols. <longest>
   satisfying SUF[longest] = 0. The method works in-place that is the
   input (the MTF) is stored in the same memory area <codespace> as the
   output (the Bwt). */

void mtf_decode(GtUchar * a, GtUchar *codespace,
                unsigned long longest,
                unsigned long seqlength,
                unsigned long numofchars);

/* The following function checks that bwt_encode/mtf_encode and
   bwt_decode/mtf_decode work
   correctly for the given <sequence> of length <seqlength> over an
   alphabet of size <numofchars> and the corresponding suffix array
   <suftab>. If any difference occurs, the function reports this and
   exits with an exit code different from 0. */

void bwt_mtf_check(bool silent,bool verbose,const Uint *suftab,
                   const GtUchar *sequence,unsigned long seqlength,
                   unsigned long numofchars);

#endif
