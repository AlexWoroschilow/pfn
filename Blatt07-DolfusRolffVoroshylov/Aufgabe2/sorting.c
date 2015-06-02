#include "sorting.h"

#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <limits.h>

#include "macros.h"

/* Gibt die groesste und kleinste zufallszahl im angegeben array <values>
   der groesse <nofelements> zurueck.
   Hierbei duerfen weder <min> noch <max> null pointer sein.
*/
static void getMinMaxValue(const unsigned int* values,
                           const unsigned long n,
                           unsigned int* min,
                           unsigned int* max)
{
  unsigned long i;
  *min = UINT_MAX;
  *max = 0;

  assert_with_message(min != NULL, "min can not be NULL");
  assert_with_message(max != NULL, "max can not be NULL");
  for (i = 0; i < n; ++i)
  {
    if (*max < values[i])
    {
      *max = values[i];
    }
    if (*min > values[i])
    {
      *min = values[i];
    }
  }
}

/* Sortiert das Array <values> mit <nofelements> Elementen. Mit Hilfe der
   insertionsort Methode */
void insertionsort(unsigned int *values, unsigned long nofelements){
  unsigned long index, h_index; //Hilfsindizes
  assert_with_message(values != NULL,
                      "The given array to sort can not be NULL!")  
  /*läuft von links nach rechts alle Werte im Array durch,
    Index fängt bei 1 an, da das 0-te Element schon als sortiert gilt*/
  for (index = 1; index < nofelements; index ++) {
    /*die aktuell zu sortierende Zahl wird zwischengespeichert*/
    const unsigned int wert = values[index];
    /*nun schaut man, ob von der aktuellen Position aus,
      links im Array noch ein kleinerer Wert existiert */
    for (h_index = index; values[h_index-1] > wert && h_index > 0; h_index--){
      values[h_index] = values[h_index-1];
    }
    
    values[h_index] = wert;
  }
}


/* Sortiert das Array <values> mit <nofelements> Elementen. Mit Hilfe der
   countingsort Methode */
void countingsort(unsigned int *values, unsigned long nofelements){
  unsigned long i, range;
  unsigned int min, max;
  unsigned int* h_array, *h_values;
  
  /*Suche nach dem größten Wert im Array*/
  getMinMaxValue(values, nofelements, &min, &max);
  range = max - min + 1;
  if (range <= 1 || max < min)
    return; /* ein array mit einem element ist bereits sortiert */
  
  /*Hilfsarray der Größe maxsortvalue*/
  calloc_or_exit(h_array, range, sizeof(*h_array),
                 "Could not allocate enough space for the counting array");
  malloc_or_exit(h_values, sizeof(*h_values) * nofelements,
                 "Could not allocate enough space for the output array");
  /*Zähle wie häufig jeder Wert in values vorkommt,
    in h_array[i] steht nun wie häufig der Wert i in values vorkommt*/
  for (i = 0; i < nofelements; i++){
    h_array[values[i] - min]++;
  }
  
  /* anzahl der elemente unter i bestimmen */
  for (i = 1; i < range; ++i) {
    h_array[i] += h_array[i - 1];
  }

  /* elemente einsortieren */
  for (i = 0; i < nofelements; ++i)
  {
    /* Countingsort beginnt bei 1 */
    h_values[h_array[values[i] - min] - 1] = values[i];
    --h_array[values[i] - min];
  }
  /*kopiere die Werte aus h_values in values */
  memcpy(values, h_values, sizeof(*values) * nofelements);
  
  free(h_array);
  free(h_values);
}
