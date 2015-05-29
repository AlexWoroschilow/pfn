#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "../fastq-assert.h"

/**
 * Duplicate string, or just a copy string
 * resize if string size not equals
 */
#define string_copy(destination, source)\
    if (destination == NULL) {\
      destination = strdup(source);\
    } else {\
      if (strlen(source) > strlen(destination)) {\
        realloc_or_exit(destination,\
            sizeof(destination) * strlen(source),\
            "Can not allocate memory for generation space");\
        strcpy(destination, source);\
      }\
    }\

/**
 * Class to represent line parser properties
 * like max length, current line, file
 * and current read buffer other
 */
typedef struct FastQentryLineParser {
  unsigned long line;
  unsigned long length;
  FILE * file;
  char * buffer;
} FastQentryLineParser;

/* class to represent a single <FastQentry> */
typedef struct FastQentry {
  unsigned long line;
  char * header;
  char * sequence;
  char * description;
  char * quality;
  FastQentryLineParser * parser;
} FastQentry;

/* create a <FastQentry} object for <filename>. To generate an appropriate
 error message, the name of the program must be provided as first argument */
FastQentry *fastqentry_new(__attribute__((unused))const char *progname, const char *filename) {

  FastQentry * fastqentry = NULL;

  realloc_or_exit(fastqentry, sizeof(*fastqentry),
      "Can not allocate memory for generation space");

  fastqentry->line = 0UL;
  fastqentry->header = NULL;
  fastqentry->quality = NULL;
  fastqentry->sequence = NULL;
  fastqentry->description = NULL;

  fastqentry->parser = NULL;
  realloc_or_exit(fastqentry->parser, sizeof(*fastqentry->parser),
      "Can not allocate memory for generation space");

  fastqentry->parser->file = NULL;
  fopen_or_exit(fastqentry->parser->file, filename, "r");

  fastqentry->parser->buffer = NULL;
  fastqentry->parser->length = 50UL;
  fastqentry->parser->line = fastqentry->line;

  return fastqentry;
}

/**
 * Custom line parser,
 * return a line without \n or a NULL
 * use a dynamic buffer size
 */
char * fastqentry_parse_line(FastQentryLineParser *fastqentryparser) {

  char character;
  unsigned long i = 0;

  if (fastqentryparser->buffer == NULL) {
    realloc_or_exit(fastqentryparser->buffer,
        sizeof(fastqentryparser->buffer) * fastqentryparser->length,
        "Can not allocate memory for generation space");
  }

  while ((character = fgetc(fastqentryparser->file)) != EOF) {

    if (fastqentryparser->length <= i) {
      fastqentryparser->length *= 2;

      realloc_or_exit(fastqentryparser->buffer,
          sizeof(fastqentryparser->buffer) * fastqentryparser->length,
          "Can not allocate memory for generation space");
    }

    if (character == '\n') {
      fastqentryparser->line++;
      fastqentryparser->buffer[i] = '\0';
      return fastqentryparser->buffer;
    }

    fastqentryparser->buffer[i++] = character;
  }

  return NULL;
}

/* Ask for next <FastQentry>. Returns <false>, if there is no more
 <FastQentry>. Return <true> if there is one which is referred to
 by <fastqentry>. */
bool fastqentry_next(FastQentry *fastqentry) {

  char * buffer;

  if ((buffer = fastqentry_parse_line(fastqentry->parser))) {
    string_copy(fastqentry->header, buffer);
    fastqentry->line = fastqentry->parser->line;
    if ((buffer = fastqentry_parse_line(fastqentry->parser))) {
      string_copy(fastqentry->sequence, buffer);
      /* ignore this line 3 with + something unknown*/
      if ((buffer = fastqentry_parse_line(fastqentry->parser))) {
        string_copy(fastqentry->description, buffer);
        /* line 4 is a quality string*/
        if ((buffer = fastqentry_parse_line(fastqentry->parser))) {
          string_copy(fastqentry->quality, buffer);

          validate_fastqentry(fastqentry);
          return true;
        }
      }
    }
  }
  return false;
}

/* TODO: clear strings without free function, */
/* clear the contents of a <FastQentry>. */
void fastqentry_clear(FastQentry *fastqentry) {
  validate_fastqentry(fastqentry);
  free(fastqentry->header);
  fastqentry->header = NULL;
  free(fastqentry->quality);
  fastqentry->quality = NULL;
  free(fastqentry->sequence);
  fastqentry->sequence = NULL;
  free(fastqentry->description);
  fastqentry->description = NULL;
}

/* delete <FastQentry>-object. */
void fastqentry_delete(FastQentry *fastqentry) {
  if (fastqentry) {
    if (fastqentry->parser) {
      fclose(fastqentry->parser->file);
      free(fastqentry->parser->buffer);
    }
    free(fastqentry->parser);
    free(fastqentry->header);
    free(fastqentry->quality);
    free(fastqentry->sequence);
    free(fastqentry->description);
    free(fastqentry);
  }
}

/* show <FastQentry>-object. This is used mainly for testing. */
void fastqentry_show(const FastQentry *fastqentry) {
  validate_fastqentry(fastqentry);
  printf("%s\n%s\n%s\n%s\n", fastqentry->header, fastqentry->sequence,
      fastqentry->description, fastqentry->quality);
}

/* return header line from given <FastQentry>-object.*/
const char *fastqentry_headerline(const FastQentry *fastqentry) {
  validate_fastqentry(fastqentry);
  return fastqentry->header;
}

/* return sequence line from given <FastQentry>-object.*/
const char *fastqentry_sequenceline(const FastQentry *fastqentry) {
  validate_fastqentry(fastqentry);
  return fastqentry->sequence;
}

/* return quality value line from given <FastQentry>-object.*/
const char *fastqentry_qualityline(const FastQentry *fastqentry) {
  validate_fastqentry(fastqentry);
  return fastqentry->quality;
}

/* deliver length of line of sequenceline and qualityline of
 <FastQentry>-object.*/
unsigned long fastqentry_linelength(const FastQentry *fastqentry) {
  validate_fastqentry(fastqentry);
  return strlen(fastqentry->sequence);
}

/* deliver the line number of the input file at which the current
 <FastQentry> starts. Can be used to generate meaningful error messages. */
unsigned long fastqentry_linenum(const FastQentry *fastqentry) {
  validate_fastqentry(fastqentry);
  return fastqentry->line;
}
