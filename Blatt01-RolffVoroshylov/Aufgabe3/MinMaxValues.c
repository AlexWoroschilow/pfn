/*
 ============================================================================
 Name        : MinMaxValues.c
 Author      : Oleksand Voroshylov, Tim Rolff
 Version     :
 Copyright   : 2015
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

int main(void) {

  // Eine Möglichkeit wäre dann
  // diese Werte einfach berechnen wie hier
  long maxui = (unsigned int) (-1);
  /* DW:
     Sie versuchen hier den maximalwert für unsigned long in einem long zu
     Speichern - wo ist da der Denkfehler? */
  long maxul = (unsigned long) (-1);
  long minsi = -(maxui / 2 + 1);
  long maxsi = (maxui / 2);
  long minsl = -(maxul / 2 + 1);
  long maxsl = (maxul / 2);

  // Eine andere Möglichkeit ist eine
  // Konstante aus limits.h zu verwenden
  fprintf(stdout, "Unsigned char \tmin: %d, max:%d, Bytes:%lu\n", 0,
  UCHAR_MAX, sizeof(char));

  fprintf(stdout, "Unsigned short \tmin: %d, max:%d, Bytes:%lu\n", 0,
  USHRT_MAX, sizeof(short));

  fprintf(stdout, "Unsigned int \tmin: %d, max:%ld, Bytes:%lu\n", 0, maxui,
      sizeof(int));

  fprintf(stdout, "Unsigned long \tmin: %d, max:%ld, Bytes:%lu\n", 0, maxul,
      sizeof(long));

  /* DW:
     Falsche Format-strings verwendet. */
  fprintf(stdout, "Signed char \tmin: %d, max:%d, Bytes:%lu\n",
  CHAR_MIN, CHAR_MAX, sizeof(char));

  fprintf(stdout, "Signed short \tmin: %d, max:%"/*l*/"d, Bytes:%lu\n",
  SHRT_MIN, SHRT_MAX, sizeof(short));

  fprintf(stdout, "Signed int \tmin: %ld, max:%ld, Bytes:%ld\n", minsi, maxsi,
      sizeof(int));

  fprintf(stdout, "Signed long \tmin: %ld, max:%ld, Bytes:%lu\n", minsl, maxsl,
      sizeof(long));

  return EXIT_SUCCESS;
}
