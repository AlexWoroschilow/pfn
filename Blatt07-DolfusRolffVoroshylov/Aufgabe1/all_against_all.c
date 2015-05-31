#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "multiseq.h"

int main() {

  char * filename = "sw175.fna";

  Multiseq * muliseq;
  MultiseqItem * item;

  muliseq = muliseq_new(filename);
  while ((item = muliseq_item_next(muliseq)) != NULL) {
    muliseq_item_show(item);
  }

//  printf("\n-----------------------\n");
//
//  while ((item = muliseq_item_next(muliseq)) != NULL) {
//    muliseq_item_show(item);
//  }

  muliseq_destroy(muliseq);

  exit(EXIT_SUCCESS);
}
