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

/* DW:
   argc unused, dies ist nicht akzeptiert. Alle Funktionsparameter müssen auch
   verwendet werden! */
int main(int argc, char ** argv) {

  int year;

  /* DW:
     OK, Dieser test Funktioniert, wenn KEIN Parameter Übergeben wurde, aber was
     passiert wenn ich 2 Parameter übergebe? Dann wird der letzte Parameter
     ignoriert und das Programm gibt keine Fehlermeldung aus. Benutzen Sie
     argc!!! */
  if (argv[1] != NULL && sscanf(argv[1], "%d", &year)) {

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
      fprintf(stdout, "Year: %d is a Leap year\n", year);
      return EXIT_SUCCESS;
    }

    fprintf(stdout, "Year: %d is not a Leap year\n", year);
    return EXIT_SUCCESS;
  }

  /* DW:
     infinite??? Nicht wirklich... */
  /* DW:
     Und wieder: für den Programmnamen argv[0] verwenden */
  fprintf(stderr, "Usage: ./LeapYearCalculator.x [0...infinite]\n");
  return EXIT_FAILURE;
}
