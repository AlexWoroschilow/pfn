 /* Bibliothek mit Funktionen zum Sortieren von Zahlen
    (Source-code file) */
#ifndef SORTING_H
#define SORTING_H

/* Sortiert das Array <values> mit <nofelements> Elementen. Mit Hilfe der
   insertionsort Methode */
void insertionsort(unsigned int *values, unsigned long nofelements);

/* Sortiert das Array <values> mit <nofelements> Elementen. Mit Hilfe der
   countingsort Methode */
void countingsort(unsigned int *values, unsigned long nofelements);
#endif
