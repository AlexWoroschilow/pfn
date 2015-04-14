//Tim Rolff Oleksand Voroshylov
/*
 ============================================================================
 Name        : subsetsum.c
 Author      : Tim Rolff, Oleksand Voroshylov
 Version     : 1.0
 Copyright   : 2015
 Description : solver for the subsetsum problem
 ============================================================================
 */
#include "subsetsum.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

/*
 * Check conditions and return
 * a message if fail
 */
#define assert_with_message(condition, message) \
  if(!(condition)) \
  { \
    fprintf(stderr, message); \
    exit(EXIT_FAILURE); \
  }

/**
 * Allocate a memory and display
 * a message if not success
 */
#define malloc_or_exit(pointer, size, message)\
  assert_with_message((pointer = malloc(size)) != NULL, message);\


bool subsetsum(unsigned long k,
bool* mark, const unsigned long* arr, unsigned long n) {

  unsigned long i = 0;
  unsigned long sum = 0;
  bool ret = false;

  /*
   Summe berechnen in dem nur elemente addiert werden,
   welche auch makiert wurden.
   */
  for (i = 0; i < n; ++i) {
    if (mark[i]) {
      sum += arr[i];
    }
  }

  /*
   wenn die summe noch kleiner ist, dann die werte von
   dynamic veraendern, welche die gleichen daten wie mark
   enthalten und erneut die funktion aufrufen.
   */
  if (sum < k) {

    bool* dynamic;
    unsigned long dynamic_size = sizeof(*mark) * n;

    malloc_or_exit(dynamic, dynamic_size,
        "Could not allocate enough memory for a new mark pointer\n");

    for (i = 0; i < n; ++i) {
      /*
       speicher von dynamic zurueck setzen und
       den i. eintrag veraendern.
       */
      memcpy(dynamic, mark, dynamic_size);

      /*
       wenn der wert an n - i - 1 noch nicht true ist:
       */
      if (!dynamic[n - i - 1]) {
        /*
         dann die werte modulieren und zwar mit den groe�ten
         werten anfangen.
         Alternativ koennte man auch rueckwerts
         iterieren.
         */
        dynamic[n - i - 1] = true;
        if (subsetsum(k, dynamic, arr, n)) {
          ret = true;
          /*
           wenn eine loesung gefunden wurde,
           dann die loesung an mark zurueck kopieren.
           */
          memcpy(mark, dynamic, dynamic_size);

          /*
           aus der schleife springen.
           */
          break;
        }
      }
    }

    free(dynamic);
  } else if (sum == k) {
    ret = true;
  }

  return ret;
}
