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

void process_entry(const char * header, const char * sequence,
    const char *quality, unsigned long length) {

  printf("Header: \t%s\nSequence: \t%s\nQuality: \t%s\nLength: \t%lu\n", header,
      sequence, quality, length);
}

int main(int argc, char * argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FastqConcat * sq = fastq_concat_new(argv[0], argv[1]);
  fastq_concat_show(sq);

  fastq_concat_delete(sq);

  exit(EXIT_SUCCESS);
}
