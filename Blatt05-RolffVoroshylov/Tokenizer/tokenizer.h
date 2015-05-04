#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef void (*TokenHandlerFunc)(const char *, void *);
void tokenizer(FILE *fp, TokenHandlerFunc tokenhandler, void *data);

#endif
