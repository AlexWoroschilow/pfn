#ifndef FASTQ_ASSERT_H
#define FASTQ_ASSERT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/**
 * Check conditions and return
 * a message if fail
 */
#define assert_with_message(condition, message) \
  if (!(condition)) { \
    fprintf(stderr, "%s in file \"%s\" at line %lu\n", \
            message, __FILE__, (unsigned long) (__LINE__ - 1)); \
    exit(EXIT_FAILURE); \
  }

/**
 * Reallocate a memory and display
 * a message if not success
 */
#define realloc_or_exit(pointer, size, message)\
  assert_with_message((pointer = realloc(pointer, size)) != NULL, message);

/* Try to open file and
 assert pointer not equal NULL
 */
#define fopen_or_exit(source, filename, mode) \
    if ((source = fopen(filename, mode)) == NULL) { \
        fprintf(stderr, "Can not open file %s in mode %s\n" \
                        "Error in file %s at line %lu\n", \
                        filename, mode, __FILE__, (unsigned long) __LINE__); \
        exit(EXIT_FAILURE); \
    }

/**
 * Validate FastQentry structure
 */
#define validate_fastqentry(fastqentry)\
  assert_with_message((fastqentry->header != NULL), "Header string should not be empty");\
  assert_with_message((fastqentry->sequence != NULL), "Sequence string should not be empty");\
  assert_with_message((fastqentry->description != NULL), "Description string should not be empty");\
  assert_with_message((fastqentry->quality != NULL), "Quality string should length are different");\
  assert_with_message((fastqentry->parser != NULL), "Parser object should not be empty");\
  assert_with_message((strlen(fastqentry->sequence) == strlen(fastqentry->quality)), "Sequence and Quality length are different");

/* validate fasq-concatenation object */
#define validate_fastqconcat(object)\
  assert_with_message((object->header != NULL), "Header string should not be empty");\
  assert_with_message((object->sequence != NULL), "Sequence string should not be empty");\
  assert_with_message((object->quality != NULL), "Quality string should length are different");\
//  assert_with_message((strlen(object->sequence) == strlen(object->quality)), "Sequence and Quality length are different");

#endif
