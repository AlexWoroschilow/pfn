/*
 ============================================================================
 Name        : CelsiusToFahrenheit.c
 Author      : Oleksand Voroshylov, Tim Rolff
 Version     :
 Copyright   : 2015
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define IN_RANGE(x,y,z) ( x <= y && y <= z)

int main(int argc, char **argv) {

  float temperatur;

  if (argc == 2) {
    /* DW:
       nicht ganz schön. sscanf(...) == 1 ist besser */
    if (sscanf(argv[1], "%f", &temperatur)) {
      if (IN_RANGE(0.0, temperatur, 100.0)) {
        fprintf(stdout, "Fahrenheit: %3.1f in Celsius: %3.1f\n",
            ((temperatur * 2) + 30), temperatur);
        return EXIT_SUCCESS;
      }

    }
  }

  fprintf(stderr, "Usage: ./CelsiusToFahrenheit.x [0..100]\n");
  return EXIT_FAILURE;
}
