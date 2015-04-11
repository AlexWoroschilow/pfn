/*
 ============================================================================
 Name        : LeapYearCalculator.c
 Author      : Oleksand Voroshylov, Tim Rolff
 Version     :
 Copyright   : 2015
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {

  int year;

  if (argv[1] != NULL && sscanf(argv[1], "%d", &year)) {

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
      fprintf(stdout, "Year: %d is a Leap year\n", year);
      return EXIT_SUCCESS;
    }

    fprintf(stdout, "Year: %d is not a Leap year\n", year);
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Usage: ./LeapYearCalculator.x [0...infinite]\n");
  return EXIT_FAILURE;
}
