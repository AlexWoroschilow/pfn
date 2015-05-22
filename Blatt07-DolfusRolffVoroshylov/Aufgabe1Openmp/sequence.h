#ifndef SEQUENCE_RANGE_H
#define SEQUENCE_RANGE_H

#include "bestkvals.h"
#include "multiseq.h"

typedef struct Sequence Sequence;
typedef struct SequenceEntry SequenceEntry;

typedef unsigned long (*distanceFunction) (const unsigned char*,
                                           unsigned long,
                                           const unsigned char*,
                                           unsigned long);

/**
  Values in the computed sequence
*/
struct SequenceEntry
{
  unsigned long distance,
                firstId,
                secondId;
};

/**
  Create a new Seqence to store SequenceEntrys.
  The SequenceEntrys holding the computed values
  of the distance function.
*/
Sequence* createSequence();

/**
  Add a computed distance value to the Sequence.
  The x variable is the id of the Sequence in MultiSeq
  and the y variable is the id of the compaired sequence
*/                                   
void addSequenceEntry(Sequence* range,
                      unsigned long distance,
                      unsigned long x,
                      unsigned long y);

/**
  Get an entry from the sequence idx must be smaller
  than the size of the Sequence
*/
SequenceEntry* getSequenceEntry(const Sequence* range,
                                unsigned long idx);
                                
/**
  Returns the size of the Sequence
*/                 
unsigned long getSequenceSize(const Sequence* range);

/**
  Free the allocated memory of the Sequence
*/
void deleteSequence(Sequence* seq);
#endif