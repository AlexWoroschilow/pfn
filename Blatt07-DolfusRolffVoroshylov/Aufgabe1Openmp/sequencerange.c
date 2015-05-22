#include "sequencerange.h"

#include <stdlib.h>
#include "macros.h"

#define validateSequence(sequence)                  \
  assert_with_message(sequence != NULL,             \
                      "Sequence should be NULL");   \
  assert_with_message(sequence->entries != NULL,     \
                      "Entries should not be NULL");

struct SequenceRange
{
  SequenceEntry* entries;
  unsigned long currentSize,
                allocatedSize;
};

SequenceRange* createSequenceRange()
{
  SequenceRange* range;
  malloc_or_exit(range, sizeof(*range),
                 "Could not allocate memory for a new SequenceRange");
  
  range->currentSize = 0;
  range->allocatedSize = 1024;
  malloc_or_exit(range->entries,
                 sizeof(*range->entries) * range->allocatedSize,
                 "Could not allocate memory for the entries");
  return range;
}
                            
void addSequenceEntry(SequenceRange* range,
                      unsigned long distance,
                      unsigned long x,
                      unsigned long y)
{
  validateSequence(range);
  if (range->currentSize + 1 >= range->allocatedSize)
  {
    range->allocatedSize = range->allocatedSize * 1.2 + 512;
    realloc_or_exit(range->entries, 
                    sizeof(*range->entries) * range->allocatedSize,
                    "Could not reallocate memory for entries");
  }
  range->entries[range->currentSize].distance = distance;
  range->entries[range->currentSize].firstId = x;
  range->entries[range->currentSize].secondId = y;
  range->currentSize++;
}

SequenceEntry* getSequenceEntry(const SequenceRange* range,
                                unsigned long idx)
{
  validateSequence(range);
  assert_with_message(idx < getSequenceSize(range),
                      "The index must be smaller than the sequence size");

  return &range->entries[idx];
}
              
unsigned long getSequenceSize(const SequenceRange* range)
{
  validateSequence(range);
  return range->currentSize;
}

void deleteSequenceRange(SequenceRange* seq)
{
  if (seq)
  {
    free(seq->entries);
    free(seq);
  }
}