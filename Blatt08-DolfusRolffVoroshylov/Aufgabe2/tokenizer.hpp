#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef void (*TokenHandlerFunc)(const char *,void *);

void tokenizer(TokenHandlerFunc tokenhandler,void *data);

#endif
