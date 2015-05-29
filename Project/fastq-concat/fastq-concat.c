#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "fastq-assert.h"
#include "fastq-parse/fastq-parse.h"

/* The following type is used to store the concatenation of
 a set of sequences stored in Fastq-format. Actually,
 three concatenations stored, namely
 - the concatenation of the nucleotide sequences,
 - the concatenation of the quality values and
 - the concatenation of the header lines.
 The first two concatenations are of the same length and does not include
 newlines. The concatenation of the header lines contains newlines and
 is \0-terminated. */

#define fastq_concat_string_copy(destination, source)\
    if (destination == NULL) {\
      destination = (unsigned char*)strdup(source);\
    } else {\
      realloc_or_exit(destination,\
          sizeof(destination) * (strlen((const char *)destination) + strlen(source)),\
          "Can not allocate memory for generation space");\
      strcat((char *)destination, source);\
    }

/* Store debug information here */
typedef struct FastqConcatDebug {
  unsigned long vector_length;
  unsigned long * vector_header;
  unsigned long * vector_sequence;
} FastqConcatDebug;

/* Main concat structure */
typedef struct FastqConcat {
  FastqConcatDebug * debug;
  unsigned char * header;
  unsigned char * sequence;
  unsigned char * quality;
} FastqConcat;

/* Initialize debug object */
FastqConcatDebug *fastq_concat_debug_new(const unsigned long length) {

  FastqConcatDebug * debug = NULL;
  realloc_or_exit(debug, sizeof(*debug),
      "Can not allocation memory for sequence-concatenation debug object");

  debug->vector_length = length;

  debug->vector_header = NULL;
  realloc_or_exit(debug->vector_header,
      sizeof(*debug->vector_header) * debug->vector_length,
      "Can not allocation memory for sequence-concatenation debug object");

  debug->vector_sequence = NULL;
  realloc_or_exit(debug->vector_sequence,
      sizeof(*debug->vector_sequence) * debug->vector_length,
      "Can not allocation memory for sequence-concatenation debug object");

  return debug;
}

/* resize a buffer object */
void fastq_concat_debug_increase(FastqConcatDebug *debug) {

  const unsigned long vector_length = debug->vector_length
      + (debug->vector_length * 0.2);

  const unsigned long vector_size = sizeof(*debug->vector_header)
      * vector_length;

  realloc_or_exit(debug->vector_header, vector_size,
      "Can not allocation memory for sequence-concatenation debug object");
  realloc_or_exit(debug->vector_sequence, vector_size,
      "Can not allocation memory for sequence-concatenation debug object");

  debug->vector_length = vector_length;
}

/**
 * Store current log position
 */
void fastq_concat_debug_position(FastqConcatDebug *debug, unsigned long index,
    unsigned long length_header, unsigned long length_sequence) {

  unsigned long previous_header = 0, previous_sequence = 0;
  if (index) {
    previous_header = debug->vector_header[index - 1];
    previous_sequence = debug->vector_sequence[index - 1];
  }

  if (index >= debug->vector_length) {
    fastq_concat_debug_increase(debug);
  }

  debug->vector_header[index] = length_header + previous_header;
  debug->vector_sequence[index] = length_sequence + previous_sequence;
}

char* append_char(char* s, char c) {
  int len = strlen(s);
  realloc_or_exit(s, sizeof(*s) * (len + 1),
      "Can not allocation memory for sequence-concatenation debug object");
  s[len] = c;
  s[len + 1] = '\0';
  return s;
}

/* This is the constructor to deliver a sequence concatenation for
 the given <inputfilename>. Additionally, the name of the program
 which calls the function must be supplied. */

FastqConcat *fastq_concat_new(const char *progname, const char *inputfilename) {

  FastqConcat * sq = NULL;

  realloc_or_exit(sq, sizeof(*sq), "Can not allocation memory");

  sq->header = NULL;
  sq->quality = NULL;
  sq->sequence = NULL;
  sq->debug = fastq_concat_debug_new(10UL);

  FastQentry *fastqentry = fastqentry_new(progname, inputfilename);

  unsigned long i = 0;

  while (fastqentry_next(fastqentry)) {

    const char * header = fastqentry_headerline(fastqentry);
    const char * sequence = fastqentry_sequenceline(fastqentry);
    const char * quality = fastqentry_qualityline(fastqentry);

    /* header string should have a new lines */
//    append_char((char*) header, '\n');

    fastq_concat_string_copy(sq->header, header);
    fastq_concat_string_copy(sq->sequence, sequence);
    fastq_concat_string_copy(sq->quality, quality);

    fastq_concat_debug_position(sq->debug, i++, strlen(header),
        strlen(sequence));

    fastqentry_clear(fastqentry);
  }

  fastqentry_delete(fastqentry);

  return sq;
}

/* This is the destructor for a sequence concatenation. */

void fastq_concat_delete(FastqConcat *sq) {
  if (sq) {
    if (sq->debug) {
      free(sq->debug->vector_header);
      free(sq->debug->vector_sequence);
      free(sq->debug);
    }
    free(sq->header);
    free(sq->quality);
    free(sq->sequence);
    free(sq);
  }
}

/* Deliver the total length of the nucleotide sequences in the concatenation.
 This is the same as the total length of the quality sequences. */

unsigned long fastq_concat_totallength(const FastqConcat *sq) {
  validate_fastqconcat(sq);
  return strlen((const char *) sq->sequence);
}

/* Deliver the concatenation of the nucleotide sequences. The user can modify
 the content of the sequence but is not responsible to free its memory. */

unsigned char *fastq_concat_seq(const FastqConcat *sq) {
  validate_fastqconcat(sq);
  return sq->sequence;
}

/* Deliver the concatenation of the quality sequences. The user can modify
 the content of the sequence but is not responsible to free its memory. */

unsigned char *fastq_concat_qual(const FastqConcat *sq) {
  validate_fastqconcat(sq);
  return sq->quality;
}

/* Deliver the concatenation of the header lines. The user can modify
 the content of the sequence but is not responsible to free its memory. */

unsigned char *fastq_concat_header(const FastqConcat *sq) {
  validate_fastqconcat(sq);
  return sq->header;
}

/* Output the sequences (to stdout) in the same format as the input.
 This is used for testing purposes. */

void fastq_concat_show(const FastqConcat *sq) {
  validate_fastqconcat(sq);

  unsigned long i;
  unsigned long previews = 0;
  unsigned long previewh = 0;

  for (i = 0; i < sq->debug->vector_length; i++) {

    const unsigned long lengthh = sq->debug->vector_header[i] - previewh;
    const unsigned long lengths = sq->debug->vector_sequence[i] - previews;

    const char * starth = (const char*) &sq->header[previewh];
    const char * header = strndup(starth, lengthh);

    const char * startq = (const char*) &sq->quality[previews];
    const char * quality = strndup(startq, lengths);

    const char * starts = (const char*) &sq->sequence[previews];
    const char * sequence = strndup(starts, lengths);

    printf("%s\n%s\n+\n%s\n", header, sequence, quality);

    previewh = sq->debug->vector_header[i];
    previews = sq->debug->vector_sequence[i];

    if ((!previewh || !previews)
        || (previewh >= strlen((const char*) sq->header))
        || (previews >= strlen((const char*) sq->sequence))) {
      break;
    }
  }
}

/* Output the distribution of the occurrences of the symbols in the
 the nucleotide and the quality sequences. */

void fastq_concat_dist_show(const FastqConcat *sq) {
  validate_fastqconcat(sq);
}

