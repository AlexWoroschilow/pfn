/*
 ============================================================================
 Name        : revcomp.c

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : calculate a reverse-complement and save to a .rc file
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Check conditions and return
 a message if fail */
#define assert_with_message(condition, message) \
  if(!(condition)) { \
    fprintf(stderr, "%s in file \"%s\" at line %lu\n", \
            message, __FILE__, (unsigned long) (__LINE__ - 1)); \
    exit(EXIT_FAILURE); \
  }

/**
 * Try to open file and
 * assert pointer not equal NULL
 */
#define fopen_or_exit(source, filename, mode, message) \
    source = fopen(filename, mode);\
    assert_with_message(source != NULL, "Can not open .seq file");

/* Allocate a memory and display
 a message and stop the program if the allocation failed
 */
#define malloc_or_exit(pointer, size, message) \
  assert_with_message((pointer = malloc(size)) != NULL, message);

/**
 * Get file size
 * copy-paste from PDF-Document
 */
unsigned long file_size(const char *inputfile) {
  long filesize;
  FILE *fp = fopen(inputfile, "rb");
  assert(fp != NULL);
  fseek(fp, 0, SEEK_END); /* jump to end of file */
  filesize = ftell(fp); /* read current position */
  fclose(fp);
  assert(filesize >= 0);
  return (unsigned long) filesize;
}

/**
 * Get size of first line,
 */
unsigned long line_size(char *filename) {

  char character;

  FILE *f_source = NULL;
  fopen_or_exit(f_source, filename, "rb", "Can not open .seq file");

  unsigned long i;
  for (i = 0; (character = fgetc(f_source)) != EOF; i++) {
    if (character == '\n') {
      /* do not return here,
       * needs to close a file */
      break;
    }
  }
  /* close file, free memory */
  fclose(f_source);

  return i;
}

/**
 * Generate rev.com. file name
 * add a custom suffix at end of file to end
 */
char * revcomp_filename(char *filename, const char * suffix) {

  char * acceptor = NULL;

  unsigned long filname_length = strlen(filename) * sizeof(char)
      + strlen(suffix) + 1;

  malloc_or_exit(acceptor, filname_length,
      "Can not allocate memory for new .rc file");

  strcpy(acceptor, filename);
  strcpy(acceptor + strlen(filename), suffix);

  return acceptor;
}

/**
 * Replace char with given rules
 */
char revcomp_reverce(char source) {
  switch (source) {
  case 'A':
    return 'T';
  case 'C':
    return 'G';
  case 'G':
    return 'C';
  case 'T':
    return 'A';
  }
}

/**
 * Calculate a reverse-complement
 * create a new file and store result
 */
void revcomp_calculate(char *source) {

  const unsigned long filesize = file_size(source);
  /* extract a format from given file,
   * that may be a problem in case that  linesize == filesize
   * but for this task, its is ok */
  const unsigned long linesize = line_size(source);
  /* calculate a new name, add suffix at the end of source file name */
  char * acceptor = revcomp_filename(source, ".rc");

  assert_with_message(filesize > 0, "File should not be empty");
  assert_with_message(linesize > 0, "Line should not be empty");

  FILE *f_source = NULL;
  FILE *f_acceptor = NULL;

  fopen_or_exit(f_source, source, "rb", "Can not open .seq file");
  fopen_or_exit(f_acceptor, acceptor, "wb", "Can not open .rc file");

  /* set pointer to end of file */
  fseek(f_source, 0, SEEK_END);

  unsigned long i;
  unsigned long j;

  for (j = 0, i = 0; i <= filesize; i++) {

    /* calculate current position from end of file*/
    fseek(f_source, -(i), SEEK_END);

    char c;

    /* Not end of file and not end of line */
    if ((c = fgetc(f_source)) != EOF && c != '\n') {
      j++;

      /* write direct to file */
      fputc(revcomp_reverce(c), f_acceptor);

      if (j == linesize) {
        /* write with given format */
        fputc('\n', f_acceptor);
        j = 0;
      }
    }
  }

  fclose(f_source);
  fclose(f_acceptor);

  free(acceptor);
}

/**
 * Main function
 */
int main(int argc, char ** argv) {

  if (argc == 2) {

    char * filename = NULL;

    malloc_or_exit(filename, (strlen(argv[1]) + 1),
        "Can not allocate memory for file name");

    /* file name should have an alphanumeric + point */
    if (sscanf(argv[1], "%[A-z0-9.]", filename)) {
      revcomp_calculate(filename);
    }
    /* not really needs, but we
     * should go the right way */
    free(filename);

    exit(EXIT_SUCCESS);
  }

  fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
  exit(EXIT_FAILURE);
}
