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

  if (argc == 2) {
    fprintf(stdout, "Hello %s\n", ((argv[1] == NULL) ? "World" : argv[1]));
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Usage: ./helloworld.x  or ./helloworld.x name\n");
  return EXIT_FAILURE;
}
