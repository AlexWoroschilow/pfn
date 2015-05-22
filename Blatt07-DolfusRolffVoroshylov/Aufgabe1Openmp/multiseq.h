#ifndef MULTISEQ_H
#define MULTISEQ_H

typedef struct MultiSeq MultiSeq;

/**
  Creates a new Sequence Store to store
  seqences with different length init
*/
MultiSeq* createMutliSeq();

/**
  Add a new sequence to the sequence store.
  The sequence in the argument must have the same or a longer
  livetime as the store.
*/
void addSequence(MultiSeq* seq,
                 char* sequence,
                 unsigned long sequenceLength);

/**
  Return a sequence from the store with a given Index.
  The Index must be smaller than the size of the stored sequeneces
  sequenceLength is the length of the sequence, but it can be NULL
*/
const char* getSequence(const MultiSeq* seq,
                        unsigned long idx,
                        unsigned long* sequenceLength);

/**
  Returns the count of the seqences in the store
*/
unsigned long getSequencesCount(const MultiSeq* seq);
                        
/**
  Delete the whole store and frees the memory
*/
void deleteMutliSeq(MultiSeq* seq);

#endif