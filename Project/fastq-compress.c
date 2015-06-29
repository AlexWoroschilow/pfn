/*
 ============================================================================
 Name        : fastq-compress.c
 Author      : Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "fastq-concat/fastq-parse/fastq-parse.h"
#include "fastq-concat/fastq-concat.h"
#include "bwt-compress/gt-alloc.h"
#include "bwt-compress/sk-sain.h"
#include "bwt-compress/sktimer.h"
#include "bwt-compress/bwt-compress.h"

void process_entry(const char * header, const char * sequence,
    const char *quality, unsigned long length) {
  printf("Header: \t%s\nSequence: \t%s\nQuality: \t%s\nLength: \t%lu\n", header,
      sequence, quality, length);
}

int main(int argc, char * argv[]) {

  unsigned long numofchars = UCHAR_MAX + 1;

  size_t sequence_len;
  unsigned char * sequence;
  Uint * sequence_sa;

  size_t quality_len;
  unsigned char * quality;
  Uint * quality_sa;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FastqConcat * sq = fastq_concat_new(argv[0], argv[1]);
//  fastq_concat_show((const FastqConcat *) sq);

  sequence = fastq_concat_seq(sq);
  sequence_len = strlen((const char *) sequence);

  quality = fastq_concat_qual(sq);
  quality_len = strlen((const char *) sequence);

  sequence_sa = gt_sain_sorted_suffixes_new((const GtUchar *) sequence,
      sequence_len, numofchars);

  /* check sequence with bwt encode/decode */
  bwt_check((const Uint *) sequence_sa, (const GtUchar *) sequence,
      sequence_len, numofchars);

  bwt_mtf_check(false, true, sequence_sa, (const GtUchar *) sequence,
      sequence_len, numofchars);

  quality_sa = gt_sain_sorted_suffixes_new((const GtUchar *) quality,
      quality_len, numofchars);

//  /* check quality with bwt encode/decode */
//  bwt_check((const Uint *) quality_sa, (const GtUchar *) quality, quality_len,
//      numofchars);

//  bwt_mtf_check(false, true, quality_sa, (const GtUchar *) quality, quality_len,
//      numofchars);

  free(quality_sa);
  free(sequence_sa);

//  fastq_concat_show(sq);
  fastq_concat_delete(sq);

  exit(EXIT_SUCCESS);
}
