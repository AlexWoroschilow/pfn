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
   a message if fail
 */
#define assert_with_message(condition, message) \
  if(!(condition)) { \
    fprintf(stderr, "%s in file \"%s\" at line %lu\n", \
            message, __FILE__, (unsigned long) __LINE__); \
    exit(EXIT_FAILURE); \
  }

/* Try to open file and
   assert pointer not equal NULL
 */
#define fopen_or_exit(source, filename, mode) \
    if((source = fopen(filename, mode)) == NULL) { \
        fprintf(stderr, "Can not open file %s in mode %s\n" \
                        "Error in file %s at line %lu\n", \
                        filename, mode, __FILE__, (unsigned long) __LINE__); \
        exit(EXIT_FAILURE); \
    }

/* Allocate a memory and display
   a message and stop the program if the allocation failed
 */
#define malloc_or_exit(pointer, size, message) \
  assert_with_message((pointer = malloc(size)) != NULL, message);

/* Reallocate a memory and display
   a message and stop the program if the allocation failed
 */
#define realloc_or_exit(pointer, size, message) \
  assert_with_message((pointer = realloc(pointer, size)) != NULL, message);

/* get the minimum from x, y */
#define min(x, y) (x < y) ? (x) : (y)

/* Alternativ: get <sys/sysinfo.h> informations for the ram size */
#define BUFFERSIZE 65536UL

/* Get file size
   copy-paste from PDF-Document
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

/* Get size of first line */
unsigned long line_size(char *filename) {
  int character;
  unsigned long i;
  FILE *f_source = NULL;

  fopen_or_exit(f_source, filename, "rb");
  character = fgetc(f_source);
  for(i = 0; character != EOF && character != '\n'; ++i) {
    character = fgetc(f_source);
  }

  /* close file, free memory */
  fclose(f_source);
  return i;
}

/* Append the suffix to the filename and return a
   char pointer to a new string with the concatenated filename.
 */
char * revcomp_filename(char *filename, const char * suffix) {
  char * acceptor = NULL;
  malloc_or_exit(acceptor,
                (strlen(suffix) + strlen(filename) + 1) * sizeof(char),
                "Could not allocate enough memory for the output filename");
  strcpy(acceptor, filename);
  acceptor = strcat(acceptor, suffix);
  assert_with_message(acceptor != NULL,
                      "Could append the suffix to the output filename");
  return acceptor;
}

/* Replace char with given rules
   Returns 0 at failure.
 */
char revcomp_reverse(char source) {
  switch (source) {
    case 'A': return 'T';
    case 'C': return 'G';
    case 'G': return 'C';
    case 'T': return 'A';
  }

  /* At failure return 0 */
  return 0;
}

/* Calculate a reverse-complement
   create a new file and store result
 */
void revcomp_calculate(char *source) {
  unsigned long j, k,
                buffersize,
                to_read, filesize, linesize;
  char * acceptor = NULL, *buffer = NULL;
  FILE * f_source = NULL, * f_acceptor = NULL;

  to_read = filesize = file_size(source);
  linesize = line_size(source);
  assert_with_message(filesize > 0, "File should not be empty");
  assert_with_message(linesize > 0, "Line should not be empty");

  acceptor = revcomp_filename(source, ".rc");
  /* set the buffersize maximal to the filesize if BUFFERSIZE bigger than
     filesize */
  buffersize = min(filesize, BUFFERSIZE);
  malloc_or_exit(buffer, buffersize * sizeof(char),
                 "Could not allocate the buffer");

  fopen_or_exit(f_source, source, "rb");
  fopen_or_exit(f_acceptor, acceptor, "wb");

  j = 0;
  while (to_read > 0) {
    /* size to read (rest of file of buffersize), rest > 0 */
    const unsigned long rest = min(to_read, buffersize);
    to_read -= rest;
    /* seek from start to the rest */
    fseek(f_source, to_read, SEEK_SET);
    /* fill the buffer */
    fread(buffer, sizeof(*buffer), rest, f_source);
    /* read the buffer */
    for (k = 0; k < rest; ++k) {
      /* get the current base */
      const char base = buffer[rest - k - 1];
      /* skip control symbols */
      if (base != '\n' && base != ' ' && base != EOF) {
        /* build the reverse of base */
        const char reverse = revcomp_reverse(base);
        /* check for error */
        if (reverse == 0) {
          printf("Could not found the reverse of the base %c (ASCII %lu)\n",
                 base, base);
          exit(EXIT_FAILURE);
        } else {
          /* write direct to the output file */
          fputc(reverse, f_acceptor);

          if (++j == linesize) {
            /* write a linebreak */
            fputc('\n', f_acceptor);
            j = 0;
          }
        }
      }
    }
  }

  fclose(f_source);
  fclose(f_acceptor);

  free(acceptor);
  free(buffer);
}

/* Main function */
int main(int argc, char* argv[]) {
  if (argc == 2) {
    /* all symbols can be a file name */
    revcomp_calculate(argv[1]);
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
  return EXIT_SUCCESS;
}
