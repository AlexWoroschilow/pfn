/*
 ============================================================================
 Name        : enumwords.c

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : Parse Tokens from file
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "tokenizer.h"

void printToken(const char * token, __attribute__ ((unused)) void* data)
{
  printf("%s\n", token);
}

int main(int argc, char *argv[])
{
  FILE *input = NULL;
  if (argc != 2)
  {
    fprintf(stderr, "Tokenize the given input file.");
    fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
    return EXIT_FAILURE;
  }

  input = fopen(argv[1], "rb");
  if (!input)
  {
    fprintf(stderr, "Could not open file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  tokenizer(input, &printToken, NULL);

  if (fclose(input))
  {
      fprintf(stderr, "Could not close file: %s\n", argv[1]);
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

