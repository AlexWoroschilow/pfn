/*
 ============================================================================
 Name        : tokenizer.h

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : tokenize file
 ============================================================================
 */


#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef void (*TokenHandlerFunc)(const char *, void *);
void tokenizer(FILE *fp, TokenHandlerFunc tokenhandler, void *data);

#endif
