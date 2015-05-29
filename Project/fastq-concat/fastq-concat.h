#ifndef SEQUENCE_CONCAT_H
#define SEQUENCE_CONCAT_H

/* The following type is used to store the concatenation of
   a set of sequences stored in Fastq-froamt. Actually,
   three concatenations stored, namely
   - the concatenation of the nucleotide sequences,
   - the concatenation of the quality values and
   - the concatenation of the header lines.
   The first two concatenations are of the same length and does not include
   newlines. The concatenation of the header lines contains newlines and
   is \0-terminated. */

typedef struct FastqConcat FastqConcat;

/* This is the constructor to deliver a sequence concatenation for
   the given <inputfilename>. Additionally, the name of the program
   which calls the function must be supplied. */

FastqConcat *fastq_concat_new(const char *progname,
                                   const char *inputfilename);

/* This is the destructor for a sequence concatenation. */

void fastq_concat_delete(FastqConcat *sq);

/* Deliver the total length of the nucleotide sequences in the concatenation.
   This is the same as the total length of the quality sequences. */

unsigned long fastq_concat_totallength(const FastqConcat *sq);

/* Deliver the concatenation of the nucleotide sequences. The user can modify
   the content of the sequence but is not responsible to free its memory. */

unsigned char *fastq_concat_seq(const FastqConcat *sq);

/* Deliver the concatenation of the quality sequences. The user can modify
   the content of the sequence but is not responsible to free its memory. */

unsigned char *fastq_concat_qual(const FastqConcat *sq);

/* Deliver the concatenation of the header lines. The user can modify
   the content of the sequence but is not responsible to free its memory. */

unsigned char *fastq_concat_header(const FastqConcat *sq);

/* Output the sequences (to stdout) in the same format as the input.
   This is used for testing purposes. */

void fastq_concat_show(const FastqConcat *sq);

/* Output the distribution of the occurrences of the symbols in the
   the nucleotide and the quality sequences. */

void fastq_concat_dist_show(const FastqConcat *sq);

#endif
