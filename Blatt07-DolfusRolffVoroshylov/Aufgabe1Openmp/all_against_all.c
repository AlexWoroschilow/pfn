#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>

#include "sequence.h"
#include "unit_edist.h"
#include "bestkvals.h"
#include "multiseq.h"
#include "macros.h"

/* Each Thread process 16 Sequences at once */
#define SUBPROBLEMSIZE 16

static void printError(const char* progName)
{
  fprintf(stderr, "Program usage %s <k> <t> <filename>\n"
                  "Where k  maximal best sequence pair count\n"
                  "      t  the number of thread to use\n"
                  "filename The filename to read from (in fasta format)\n\n"
                  "Example: %s 10 2 sw175.fna\n", progName, progName);
}

static unsigned long getFileSize(FILE* file)
{
  unsigned long size;
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

static char* readFasta(const char* filename, MultiSeq* seq)
{
  char* line, *buffer;
  FILE* file;
  size_t lineSize, read;
  unsigned long fileSize, currentPos;

  lineSize = currentPos = 0;
  line = NULL;
  file = NULL;

  fopen_or_exit(file, filename, "rb");
  fileSize = getFileSize(file);
  malloc_or_exit(buffer, sizeof(*buffer) * fileSize,
                 "Could not allocate enough memory for Buffer");

  while ((read = getline(&line, &lineSize, file)) != -1)
  {
    memcpy(&buffer[currentPos], line, sizeof(*line) * read);
    currentPos += read;

    if (line != NULL && line[0] == '>')
    {
      read = getline(&line, &lineSize, file);
      assert_with_message(read != -1,
                          "There must be a Sequence after a header!");
      memcpy(&buffer[currentPos], line, sizeof(*line) * read);
      addSequence(seq, &buffer[currentPos], read);
      currentPos += read;
    }
  }

  free(line);
  fclose(file);
  
  return buffer;
}

static int compareValue(const void *x, const void *y)
{
  const SequenceEntry* a = (const SequenceEntry*) x;
  const SequenceEntry* b = (const SequenceEntry*) y;

  if (a->distance > b->distance)
  {
    return 1;
  }
  else if (b->distance > a->distance)
  {
    return -1;
  }
  else if (a->firstId > b->firstId)
  {
    return 1;
  }
  else if (b->firstId > a->firstId)
  {
    return -1;
  }
  else if (a->secondId > b->secondId)
  {
    return 1;
  }
  else if(b->secondId > a->secondId)
  {
    return -1;
  }

  return 0;
}

static void eval_seqrange(const MultiSeq* seq,
                          Sequence* range,
                          distanceFunction distance,
                          const unsigned long start,
                          const unsigned long end)
{
  assert_with_message(start <= end, "Start muss be smaller than the end");
  unsigned long i, j;
  const unsigned long n = getSequencesCount(seq);
  for (i = start; i <= end; ++i)
  {
    unsigned long currentSize;
    const char* currentSequence = getSequence(seq, i, &currentSize);
    for (j = i + 1; j < n; ++j)
    {
      unsigned long otherSize;
      const char* otherSequence = getSequence(seq, j, &otherSize);
      const unsigned long val = distance((unsigned char*)currentSequence,
                                         currentSize,
                                         (unsigned char*)otherSequence,
                                         otherSize);
      addSequenceEntry(range, val, i, j);
    }
  }
}

static void printKVals(const unsigned long threads,
                       const unsigned long k,
                       Sequence** ranges)
{
  unsigned long i, j;
  BestKVals* kvals = best_k_vals_new(k, compareValue, sizeof(SequenceEntry));
  for (i = 0; i < threads; ++i)
  {
    const unsigned long size = getSequenceSize(ranges[i]);
    for (j = 0; j < size; ++j)
    {
      const SequenceEntry* entry = getSequenceEntry(ranges[i], j);
      best_k_vals_insert(kvals, entry);
    }
  }

  best_k_vals_reset_iter(kvals);
  for(i = 0; i < k; ++i)
  {
    const SequenceEntry* entry = (SequenceEntry*)best_k_vals_next(kvals);
    printf("%lu\t%lu\t%lu\n", entry->firstId,
                              entry->secondId,
                              entry->distance);
  }

  best_k_vals_delete(kvals);
}

static void process(const unsigned long threads,
                    const unsigned long k,
                    MultiSeq* seq,
                    distanceFunction distance)
{
  double range;
  Sequence** ranges;
  unsigned long i, threadCount, count;

  count = getSequencesCount(seq);
  threadCount = maximum(((double) count) / SUBPROBLEMSIZE, 1);
  range = ((double) count) / threadCount;

  malloc_or_exit(ranges, sizeof(*ranges) * threadCount,
                 "Could not allocate memory for sequence ranges");
  for (i = 0; i < threadCount; ++i)
  {
    ranges[i] = createSequence();
  }

  /* dont chance thread count at runtime */
  omp_set_dynamic(false);
  omp_set_num_threads(threads);

  /* Run parallel seqrange */
  #pragma omp parallel for private(i)                \
                           firstprivate(ranges, seq) \
                           schedule(static)
  for (i = 0; i < threadCount; ++i)
  {
    const unsigned long start = i * range;
    const unsigned long end   = (i + 1) * range - 1;
    /* DEBUG */
    /*printf("Process from %lu to %lu in thread %d\n",
           start, end, omp_get_thread_num());*/
    eval_seqrange(seq, ranges[i], distance, start, end);
  }

  /* print out computed values */
  printKVals(threadCount, k, ranges);

  for (i = 0; i < threadCount; ++i)
  {
    deleteSequence(ranges[i]);
  }

  free(ranges);
}

int main(int argc, char* argv[])
{
  unsigned long k, t;
  if (argc == 4 &&
      sscanf(argv[1], "%lu", &k) == 1 &&
      sscanf(argv[2], "%lu", &t) == 1 &&
      t >= 1 && k >= 1) {
    char* buffer;
    MultiSeq* seq;
    seq = createMutliSeq();
    buffer = readFasta(argv[3], seq);

    process(t, k, seq, eval_unit_edist);

    deleteMutliSeq(seq);
    free (buffer);
  }
  else
  {
    printError(argv[0]);
  }

  return EXIT_SUCCESS;
}