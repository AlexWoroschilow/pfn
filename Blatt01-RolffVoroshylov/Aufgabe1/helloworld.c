/*
 ============================================================================
 Name        : helloworld.c
 Author      : Oleksand Voroshylov, Tim Rolff
 Version     :
 Copyright   : 2015
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  /* DW:
     Dies ist auf mehrere arten falsch:
     wenn argc == 2 ist, dann ist argv[1] NIEMALS NULL
     wenn argc == 1 ist, dann führt schon der test ob argv[1] == NULL ist zu
     einem Crash des Programms. Die gesamte Logik hier ist also verkehrt. */
  /* if (argc == 2) {
    fprintf(stdout, "Hello %s\n", ((argv[1] == NULL) ? "World" : argv[1]));
    return EXIT_SUCCESS;
  } */
  /* DW:
     so ist es korrekt: */
  if (argc <= 2) {
    fprintf(stdout, "Hello %s\n", (argc == 1 ? "World" : argv[1]));
    return EXIT_SUCCESS;
  }

  /* DW:
     die Fehlermeldung ist FALSCH. Ihr Makefile generiert ein Program mit dem
     name helloworld OHNE .x - Das haben Sie nun davon, dass Sie Ihre makefiles
     nicht selber schreiben und abhängig von Eclipse sind...
     Aber grundsätzlich gilt: für USAGE-Meldungen verwendet man argv[0], niemals
     einen fest im code vergebenen Namen. */
  fprintf(stderr, "Usage: ./helloworld.x  or ./helloworld.x name\n");
  return EXIT_FAILURE;
}
