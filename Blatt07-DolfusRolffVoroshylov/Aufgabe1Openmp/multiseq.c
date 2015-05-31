#include <stdio.h>
#include <stdlib.h>

#include "multiseq.h"
#include "macros.h"

#define validateSequence(sequence)                      \
  assert_with_message(sequence != NULL,                 \
                      "Seq should be NULL");            \
  assert_with_message(sequence->length != NULL,         \
                      "The length should not be NULL"); \
  assert_with_message(sequence->sequences != NULL,      \
                      "Sequences should not be NULL");

struct MultiSeq
{
  char** sequences;
  unsigned long* length;
  unsigned long currentSize,
                allocatedSize;
};

MultiSeq* createMutliSeq()
{
  MultiSeq* seq;  
  malloc_or_exit(seq, sizeof(*seq),
                 "Could not allocate memory for MultiSeq");
  seq->currentSize = 0;
  seq->allocatedSize = 1024;
  malloc_or_exit(seq->sequences,
                 sizeof(*seq->sequences) * seq->allocatedSize,
                 "Could not allocate memory for the sequences");
  malloc_or_exit(seq->length,
                 sizeof(*seq->length) * seq->allocatedSize,
                 "Could not allocate memory for the length");
  return seq;
}

void addSequence(MultiSeq* seq,
                 char* sequence,
                 unsigned long sequenceLength)
{
  validateSequence(seq);
  if (seq->currentSize + 1 >= seq->allocatedSize)
  {
    seq->allocatedSize = seq->allocatedSize * 1.2 + 512;
    realloc_or_exit(seq->sequences,
                    sizeof(*seq->sequences) * seq->allocatedSize,
                    "Could not resize the sequences");
    realloc_or_exit(seq->length,
                    sizeof(*seq->length) * seq->allocatedSize,
                    "Could not resize the lengths");
  }

  seq->sequences[seq->currentSize] = sequence;
  seq->length[seq->currentSize] = sequenceLength;
  seq->currentSize++;
}

const char* getSequence(const MultiSeq* seq,
                        unsigned long idx,
                        unsigned long* sequenceLength)
{
  validateSequence(seq);
  assert_with_message(idx < seq->currentSize, 
                      "The given index must be smaller than "
                      "the count of the stored sequences");
  if (sequenceLength)
  {
    *sequenceLength = seq->length[idx];
  }
  
  return seq->sequences[idx];
}

unsigned long getSequencesCount(const MultiSeq* seq)
{
  validateSequence(seq);
  return seq->currentSize;
}

void deleteMutliSeq(MultiSeq* seq)
{
  if (seq)
  {
    free(seq->sequences);
    free(seq->length);
    free(seq);
  }
}