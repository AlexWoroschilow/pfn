#ifndef SEQUENCE_RANGE_H
#define SEQUENCE_RANGE_H

#include "bestkvals.h"
#include "multiseq.h"

typedef struct SequenceRange SequenceRange;
typedef struct SequenceEntry SequenceEntry;

/**
  Values in the SequenceRange
*/
struct SequenceEntry
{
  unsigned long distance,
                firstId,
                secondId;
};

/**
  Create a new Seqence Range from start to end
  So the Sequence have the size end - start
*/
SequenceRange* createSequenceRange();

/**
  Add a computed distance value to the SequenceRange.
  The x variable is the id of the Sequence in MultiSeq
  and the y variable is the id of the compaired sequence
*/                                   
void addSequenceEntry(SequenceRange* range,
                      unsigned long distance,
                      unsigned long x,
                      unsigned long y);

/**
  Get an entry from the sequence idx must be smaller
  than the size of the SequenceRange
*/
SequenceEntry* getSequenceEntry(const SequenceRange* range,
                                unsigned long idx);
                                
/**
  Returns the size of the SequenceRange
*/                 
unsigned long getSequenceSize(const SequenceRange* range);

/**
  Free the allocated memory of the SequenceRange
*/
void deleteSequenceRange(SequenceRange* seq);
#endif