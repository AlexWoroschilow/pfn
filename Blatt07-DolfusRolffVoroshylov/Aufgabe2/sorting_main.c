#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#include "int_conv.h"
#include "sorting.h"
#include "macros.h"

#define MAXARRAYELEMENTS 1000000
#define MAXSORTVALUE 1000U
#define DECENTILSIZE 10
#define MINDECENTILSIZE 10
#define MINVALUESIZE 8
#define DECENTILNAME "Dezentil"
#define VALUENAME "Werte"

#define printSeperator() \
  printf(" "); \
  for (i = 0; i <= dezentilSize + 1; ++i) { printf("-"); } \
  printf("+"); \
  for (i = 0; i <= valuesSize + 1; ++i) { printf("-"); } \
  printf("+\n");

  
/*
Die Worst-Case Laufzeit von Insertionsort ist O(n^2),
mit n Anzahl der Elemente im array, da
for i = 0 to n
n mal aufgerufen wird, und innerhalb dieser schleife im
Worst-Case n mal die Elemente verschoben werden.
Somit ergibt sich die Laufzeit von O(n * (n - 1)) = O(n^2 - n) = O(n^2)

Für countingsort ist die Laufzeit bei O(n + k),
mit n Anzahl der Elemente im array und k der Wertebereich der Elemente,
da hier einmal eine for i = 0 to n schleife und einmal eine for i = 0 to k
schleife durchlaufen werden
und somit die Laufzeit das maxiumum von n bzw. k ist.

Makefile:
Ja das Linkerflag -lm war gesetzt
*/
static void printDezentilverteilung(const unsigned int* values,
                                    const unsigned int count,
                                    const unsigned int min,
                                    const unsigned int max) {
  double range;
  unsigned int dezentilSize,
               valuesSize;
  unsigned long i,
                dezentil[DECENTILSIZE];
  const unsigned int nameTagSize = strlen(DECENTILNAME),
                     valueTagSize = strlen("#" VALUENAME);
  assert_with_message(values != NULL, "Values can not be null");
  printf("Dezentilverteilung:\n\n");

  /* berechnen der wertebereiche und der maximalen größe */
  valuesSize = 0;
  range = ((double)(max + 1)) / DECENTILSIZE;
  memset(dezentil, 0, sizeof(*dezentil) * DECENTILSIZE);
  for (i = 0; i < count; ++i) {
    const unsigned long idx = (unsigned long) (values[i] / range);
    dezentil[idx]++;
    if (valuesSize < log10(dezentil[idx])) {
      valuesSize = log10(dezentil[idx]);
    }
  }

  /* ausgabe der dezentile */
  dezentilSize = maximum(nameTagSize, (unsigned int)
                        (log10(range * (DECENTILSIZE - 1)) +
                         log10(range)) + 2);
  valuesSize = maximum(valueTagSize, valuesSize);

  printf(" %*s%s |%*s#%s |\n",
         (dezentilSize - nameTagSize) + 1, " ", DECENTILNAME,
         (valuesSize - valueTagSize) + 1, " ", VALUENAME);
  printSeperator();

  for (i = 0; i < DECENTILSIZE; ++i)
  {
    unsigned int size1, size2,
                 log10_9, log10_10;
    log10_9 = (unsigned int)(log10(range * i));
    log10_10 = (unsigned int)(log10(range * (i + 1)));

    size1 = dezentilSize - (log10_9 + log10_10) - 1;
    size2 = valuesSize - ((unsigned int) log10(dezentil[i]));
    printf("%*s%u-%u |%*s%u |\n",
           size1, " ", (unsigned int) (range * i),
                       (unsigned int) (range * (i + 1)),
           size2, " ", (unsigned int) dezentil[i]);
  }
}


int main (int argc, char *argv[]) {
  unsigned int *c_values,
               *i_values;
  unsigned long i;    /* Zählervariable */
  unsigned long nofelements;

  if (argc != 2 ) {
    printf("Bitte geben sie nur genau eine Zahl ein!\n");
    return EXIT_SUCCESS;
  }

  nofelements = get_long(argv[1]);
  if (nofelements <= 0 ) {
    printf("Die Anzahl an Zufallszahlen muss größer 0 sein\n");
    return EXIT_SUCCESS;
  }
  if (nofelements > MAXARRAYELEMENTS) {
    printf("Die Anzahl an Zufallszahlen soll kleiner als %d\n",
           MAXARRAYELEMENTS);
    return EXIT_SUCCESS;
  }

  malloc_or_exit(c_values, sizeof(*c_values) * nofelements,
                 "Could not allocate space for the random values");
  malloc_or_exit(i_values, sizeof(*i_values) * nofelements,
                 "Could not allocate space for the random values");

  printf("Erzeugung der Zufallszahlen..\n");
  srand48(42349421);
  for(i = 0; i < nofelements; i++) {
    i_values[i] = (unsigned int) (drand48() * (MAXSORTVALUE+1));
  }
  printf("Kopieren der Zufallszahlen..\n");
  memcpy(c_values, i_values, sizeof(*i_values) * nofelements);

  printf("Sortieren der Zufallszahlen mit Insertion Sort..\n");
  insertionsort(i_values, nofelements);
  printf("Sortieren der Zufallszahlen mit Counting Sort..\n");
  countingsort(c_values, nofelements);
  printf("Vergleich der Arrays..\n");
  if (memcmp(i_values, c_values, sizeof(*i_values) * nofelements) != 0) {
    printf("Arrays nicht identisch..\n");
    return EXIT_FAILURE;
  }

  printf("Arrays identisch..\n");
  printf("Größte erzeugte Zufallszahl: %d\n", i_values[nofelements-1]);
  printf("Kleinste erzeugte Zufallszahl: %d\n", i_values[0]);
  printDezentilverteilung(i_values, nofelements,
                          i_values[0], i_values[nofelements-1]);
  free(c_values);
  free(i_values);
  return EXIT_SUCCESS;
}

