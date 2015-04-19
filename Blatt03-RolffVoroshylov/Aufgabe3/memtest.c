/****************************************/
/* Testprogramm zum Modul "memmanage.c" */
/****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memmanage.h"
#include "memmanage-mac.h"

#define BSIZE 3

int main(void)
{
  int *a, *b, i;
  char *c;
  __attribute__((unused)) /*@unused@*/ float *d;
  MMspacetable *spacetable;

  spacetable = mem_man_new(10);

  a = MEM_MAN_ALLOC(spacetable, NULL, int, 1);
  b = MEM_MAN_ALLOC(spacetable, NULL, int, BSIZE);
  c = MEM_MAN_ALLOC(spacetable, NULL, char, 15);

  strcpy(c, "Hello World!\n");
  printf("%s", c);
  for (i = 0; i < BSIZE; i++) {
    b[i] = i * 2 + 1;
  }
  a[0] = b[0] * b[1] * b[2];
  printf("%d*%d*%d=%d\n", b[0], b[1], b[2], a[0]);

  a = MEM_MAN_ALLOC(spacetable, a, int, 2);

  a[1] = a[0] * 2;
  printf("a[0]=%d\na[1]=%d\n", a[0], a[1]);

  printf("Print out internal memory table:\n");
  mem_man_info(spacetable);

  MEM_MAN_DELETE_PTR(spacetable, a);

  /* Versuch der Freigabe eines nicht allokierten Elements:
     Wenn nicht auskommentiert, soll Testprogramm beim
     Aufruf von MEM_MAN_DELETE(d) mit Fehlermeldung abbrechen. */

  /* Alex: hier vermutlich sollte MEM_MAN_DELETE_PTR sein
  d = NULL;
  d = (float *) malloc( sizeof(float)*1 );
  MEM_MAN_DELETE(spacetable, d);
  */

  MEM_MAN_DELETE_PTR(spacetable, b);

  mem_man_check(spacetable);

  mem_man_delete(spacetable);
  return EXIT_SUCCESS;
}
