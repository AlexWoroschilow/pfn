#ifndef BWT_COMPRESS_H
#define BWT_COMPRESS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "gt-defs.h"
#include "gt-alloc.h"

/* The following function returns the BWT for a <sequence> of length
 <seqlength>.
 The suffix array <sa> for <sequence> is provided as the second
 argument. The returned pointer references a memory area storing the
 BWT of length <seqlength>+1. <longest> is the address to
 the row of the longest suffix, i.e.\ the index <i> satisfying sa[i]=0.
 For this index, bwt[i] is undefined, but should be set to some arbitrary
 value which should never be accessed. */

GtUchar *bwt_encode(unsigned long *longest, const Uint *sa,
    const GtUchar *sequence, unsigned long seqlength) {

  unsigned long i;
  GtUchar * bwt = NULL;

  bwt = gt_malloc((size_t) (seqlength + 1) * sizeof *bwt);

  for (i = 0; i <= seqlength; i++) {
    if (sa[i] > 0) {
      bwt[i] = sequence[sa[i] - 1];
    } else {
      bwt[i] = 0;
      *longest = i;
    }
  }

  return bwt;
}

/* The following function takes the BWT for a sequence of length <seqlength>
 and decodes the original sequence. To do this, additionally the
 index <longest> must be provided as well as the alphabet size <numofchars>
 which is usually set to UCHAR_MAX+1. */

GtUchar *bwt_decode(unsigned long seqlength, const GtUchar *bwt,
    unsigned long longest, unsigned long numofchars) {

  unsigned long i, j;
  unsigned long * occ = NULL;
  unsigned long * rang = NULL;
  unsigned long * count = NULL;
  unsigned long * lf = NULL;
  unsigned long partialsum;
  GtUchar * sequence = NULL;

  occ = gt_calloc((size_t) numofchars, sizeof *occ);
  count = gt_calloc((size_t) numofchars, sizeof *count);
  lf = gt_malloc((size_t) (seqlength + 1) * sizeof *lf);
  rang = gt_malloc((size_t) (seqlength + 1) * sizeof *rang);
  sequence = gt_malloc((size_t) (seqlength + 1) * sizeof *sequence);

  for (i = 0; i < seqlength + 1; i++) {
    if (i != longest) {
      const unsigned long a = (const unsigned long) bwt[i];
      occ[a] = occ[a] + 1;
      rang[i] = occ[a];
    }
  }

  for (i = 0, partialsum = 0; i < numofchars; i++) {
    count[i] = partialsum;
    partialsum += occ[i];
  }

  for (i = 0; i < seqlength + 1; i++) {
    if (i != longest) {
      const unsigned long a = (const unsigned long) bwt[i];
      lf[i] = count[a] + rang[i] - 1;
    }
  }

  i = seqlength;
  for (j = seqlength - 1; true; j--) {
    sequence[j] = bwt[i];
    i = lf[i];
    if (j == 0) {
      break;
    }
  }

  free(occ);
  free(count);
  free(lf);
  free(rang);

  return sequence;
}

/* The following function checks that bwt_encode and bwt_decode work
 correctly for the given <sequence> of length <seqlength> over an
 alphabet of size <numofchars> and the corresponding suffix array
 <sa>. The check is performed by first constructing
 the BWT using bwt_encode. The BWT is the decoded and stored in
 another memory area. Finally it is checked that the decoded sequence
 and the original sequence are identical. If any difference occurs
 the function reports this and exits with an exit code different
 from 0. */

void bwt_check(const Uint *sa, const GtUchar *sequence, unsigned long seqlength,
    unsigned long numofchars) {

  GtUchar * bwt;
  GtUchar * bwt_seq;

  unsigned long sequence_longest = 0;

  bwt = bwt_encode(&sequence_longest, sa, sequence, seqlength);
  bwt_seq = bwt_decode(seqlength, bwt, sequence_longest, numofchars);

  if (memcmp((const void *) sequence, (const void *) bwt_seq, seqlength) != 0) {
    fprintf(stderr, "Sequences are different\n");
    fprintf(stderr, "seq1: %s\nseq2: %s\n", bwt_seq, sequence);
    exit(EXIT_FAILURE);
  }

  free(bwt);
  free(bwt_seq);
}
/* The following function computes the distribution of the length of the
 runs in the bwt for the given <sequence> of length <seqlength>.
 The suffix array for <sequence> is referred to by <suftab>. If
 <silent> is true, then no output is shown. */

//void bwt_runlengthdist(bool silent, const Uint *suftab, const GtUchar *sequence,
//    unsigned long seqlength) {
//
//}
/**
 * Calculate alphabet from
 * a given string
 */
GtUchar * mtf_alphabet(const GtUchar *sequence, unsigned long seqlength,
    unsigned long numofchars, unsigned long *alphabetlength) {
  unsigned long i, j;
  unsigned long * cache = NULL;
  GtUchar * alphabet = NULL;

  cache = gt_calloc((size_t) numofchars, sizeof *cache);
  for (i = 0; i < seqlength; i++) {
    unsigned long c = sequence[i];
    cache[c]++;
  }

  *alphabetlength = 0;
  alphabet = gt_calloc((size_t) numofchars, sizeof *alphabet);
  for (i = 0, j = 0; i < numofchars; i++) {
    if (cache[i] > 0) {
      alphabet[j++] = i;
      (*alphabetlength)++;
    }
  }

  alphabet = gt_realloc(alphabet,
      (size_t) ((*alphabetlength) * sizeof(*alphabet)));

  free(cache);
  return alphabet;
}

/**
 * Display an alphabet
 */
void mtf_alphabet_print(const GtUchar *a, unsigned long numofchars) {
  unsigned long i;
  for (i = 0; i < numofchars; i++) {
    printf("index: %lu, char: %c, numofchars: %lu\n", i, a[i], numofchars);
  }
}

/**
 * Get char position in
 * current alphabet
 */
GtUchar mtf_alphabet_position(const GtUchar *a, unsigned long numofchars,
    const char c) {
  GtUchar i;
  for (i = 0; i < numofchars; i++) {
    if ((const char) a[i] == c) {
      return i;
    }
  }
  return numofchars;
}

/**
 * Switch char position
 * in alphabet
 */
void mtf_alphabet_move_to_front(GtUchar *a, unsigned long numofchars,
    unsigned long k) {
  if (k) {
    assert(numofchars > k);
    const GtUchar cache = a[k];
    memmove(&a[1], &a[0], k * sizeof(*a));
    a[0] = cache;
  }
}

/* The following function computes the Move-to-front encoding of the Bwt for
 the given <sequence> of length <seqlength>.
 The suffix array for <sequence> is referred to by <suftab> and
 <numofchars> is the number of symbols. The pointer <longest> refers
 to a variable in which the index <idx> satisfying SUF[idx] = 0 is stored.
 The method works by computing the bwt character by character directly
 applying the MTF to the character. That is, the Bwt is not stored. */

GtUchar *mtf_encode(GtUchar * a, unsigned long *longest, const Uint *suftab,
    const GtUchar *sequence, unsigned long seqlength, unsigned long numofchars) {

  unsigned long i;

  GtUchar * bwt = NULL;
  GtUchar * mft = NULL;

  mft = gt_malloc((size_t) (seqlength + 1) * sizeof(*mft));

  bwt = bwt_encode(longest, suftab, sequence, seqlength);
  for (i = 0; i <= seqlength; i++) {
    if (i != *longest) {
      GtUchar c = bwt[i];
      GtUchar x = mtf_alphabet_position(a, numofchars, c);
      mtf_alphabet_move_to_front(a, numofchars, x);

      mft[i] = (GtUchar) x;
    }
  }

  free(bwt);

  return mft;
}
/* The following function decodes the bwt from the MTF for a sequence of
 length <seqlength> over an alphabet of <numofchars> symbols. <longest>
 satisfying SUF[longest] = 0. The method works in-place that is the
 input (the MTF) is stored in the same memory area <codespace> as the
 output (the Bwt). */

GtUchar *mtf_decode(GtUchar * a, GtUchar *codespace, unsigned long longest,
    unsigned long seqlength, unsigned long numofchars) {

  unsigned long i;

  for (i = 0; i <= seqlength; i++) {
    if (i != longest) {
      unsigned long x = codespace[i];
      const GtUchar c = (const GtUchar) a[x];
      mtf_alphabet_move_to_front(a, numofchars, x);
      codespace[i] = c;
    }
  }

  return bwt_decode(seqlength, codespace, longest, numofchars);
}
/* The following function checks that bwt_encode/mtf_encode and
 bwt_decode/mtf_decode work
 correctly for the given <sequence> of length <seqlength> over an
 alphabet of size <numofchars> and the corresponding suffix array
 <suftab>. If any difference occurs, the function reports this and
 exits with an exit code different from 0. */
void bwt_mtf_show(const GtUchar *mtf, unsigned long seqlength,
    unsigned long longest) {

  unsigned long i;

  printf("\n");
  for (i = 0; i <= seqlength; i++) {
    if (i != longest) {
      printf("%lu", (unsigned long) mtf[i]);
    }
  }
  printf("\n");
}

void bwt_mtf_check(bool silent, bool verbose, const Uint *suftab,
    const GtUchar *sequence, unsigned long seqlength, unsigned long numofchars) {

  GtUchar * alphabet_1 = NULL;
  GtUchar * alphabet_2 = NULL;

  GtUchar * bwt_mtf = NULL;
  GtUchar * bwt_mtf_decoded = NULL;

  unsigned long i;
  unsigned long alphabet_length;
  unsigned long longest = 0;

  alphabet_length = numofchars;

  alphabet_1 = mtf_alphabet(sequence, seqlength, numofchars, &alphabet_length);
  alphabet_2 = gt_malloc((size_t) numofchars * sizeof(*alphabet_2));

  assert(memcpy(
      (void *)alphabet_2,
      (const void *)alphabet_1,
      alphabet_length)  != NULL);

  bwt_mtf = mtf_encode(alphabet_1, &longest, suftab,
      sequence, seqlength, alphabet_length);

  bwt_mtf_decoded = mtf_decode(alphabet_2, bwt_mtf,
      longest, seqlength, numofchars);

  if (memcmp(
      (const void *) sequence,
      (const void *) bwt_mtf_decoded,
      seqlength) != 0) {

    if (verbose && !silent) {
      fprintf(stderr, "Sequences are different\n");
      for (i = 0; i <= seqlength; i++) {
        if (i != longest) {
          fprintf(stderr, "%c", (char) bwt_mtf[i]);
        }
      }
      fprintf(stderr, "\n");
    }
  }

  free(alphabet_1);
  free(alphabet_2);
  free(bwt_mtf);
  free(bwt_mtf_decoded);
}
#endif
