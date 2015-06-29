/*
  Copyright (c) 2012 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2012 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef SK_SAIN_H
#define SK_SAIN_H

#include <stdbool.h>
#include "sktimer.h"
#include "gt-defs.h"

Uint *gt_sain_plain_sortsuffixes(bool silent,
                                 const GtUchar *plainseq,
                                 unsigned long len,
                                 unsigned long numofchars,
                                 bool intermediatecheck,
                                 GtSKtimer *sktimer);

/* Given a <sequence> of length <len> containing <numofchars> different
   characters, the following function returns the suffix array SA of length
   <len>+1 containing the start positions of all suffixes of the sequence
   in lexicographic order. During the sorting, each suffix is considered
   as if it would contain a sentinel character $ which does not occur in
   <sequence> and which is larger than any other character.
   The user is responsible to delete the space allocated for SA.
*/

Uint *gt_sain_sorted_suffixes_new(const GtUchar *sequence,
                                  unsigned long len,
                                  unsigned long numofchars);

#endif
