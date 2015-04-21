/*
 ============================================================================
 Name        : enumkmers.c

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : Enumerate k-mers
 ============================================================================
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "int_conv.h"

#define assert_with_message(x, message) \
  if(!(x)) { \
    fprintf(stderr, "%s", message); \
    exit(EXIT_FAILURE); \
  }

#define save_malloc(x,size) \
  assert_with_message((x = malloc(size)) != NULL, \
                      "Could not allocate enough memory\n");

void print_usage(char* progname) {
  printf("Usage:\n");
  printf("%s [Alphabet] [Number]\n", progname);
  printf("Alphabet: A string which represet the alphabet\n"
      "Number: The count of the outputed chars in the string "
      "(must be bigger than zero!)\n");
  printf("\nExample: %s ABC 2\n", progname);
  printf("Creates the output AA AB AC BA BB BC CA CB CC\n");
}

/*
 maximumReached:
 checks if the program circles through
 all posibilitys.

 array: The input array
 strsize: the size of the Alphabet

 Description:
 it only checks array[0] == strsize because if the
 input string length =: N

 and we are in the last iteration,
 so the array is represented by:
 [N - 1, N - 1, ..., N - 1]
 in the next circle we get
 [N, 0, 0, ..., 0]

 which is not an element of the string. So we reached the end.
 */
bool maximumReached(const unsigned long* array, const long strsize) {
  return array[0] == strsize;
}

/*
 printArray:
 prints out the alphabet by the given indices of the array

 string: the alphabet which should be shown
 array: the indices which letter should be shown
 k: the length of the array
 */
void printArray(const char* string, const unsigned long* array, const long k) {
  unsigned long i;
  for (i = 0; i < k; ++i) {
    printf("%c", string[array[i]]);
  }
  printf("\n");
}

/*
 increment:
 increments the array and does some range checks.

 array: the indices which should be incremented.
 strsize: the size of the input alphabet
 k: the size of the array
 */
void increment(unsigned long* array, const long strsize, const long k) {
  unsigned long i;

  array[k - 1]++;
  for (i = k - 1; i > 0; --i) {
    if (array[i] >= strsize) {
      array[i - 1]++;
      array[i] = 0;
    }
  }
}

/*
 permutateAlphabet:
 build all permutations out of an given alphabet.

 string: the alphabet over which should be circled
 strsize: the size of the alphabet
 k: the size of the array which used for the sub alphabet's
 */
void permutateAlphabet(const char* string, const long strsize, const long k) {
  unsigned long i;
  unsigned long* array;

  save_malloc(array, sizeof(unsigned long) * k);
  for (i = 0; i < k; ++i) {
    array[i] = 0;
  }

  while (!maximumReached(array, strsize)) {
    printArray(string, array, k);
    increment(array, strsize, k);
  }

  free(array);
}

int main(int argc, char* argv[]) {

  if (argc == 3) {

    char* string;
    long k;

    string = argv[1];
    k = get_long(argv[2]);

    if (k > 0) {
      unsigned long strsize = strlen(string);
      permutateAlphabet(string, strsize, k);
      exit(EXIT_SUCCESS);
    }
  }

  print_usage(argv[0]);
  exit(EXIT_FAILURE);
}
