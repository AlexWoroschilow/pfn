/*
 ============================================================================
 Name        : tokenizer.c

 Author      : Tim Rollf, Oleksand Voroshylov
 Version     :
 Copyright   : 2015
 Description : Tokenize file
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#include "tokenizer.h"
#include "macros.h"

typedef struct
{
  char* string;
  unsigned long size,
                allocated_size;
} Token;

static bool isValidSymbol(int tok)
{
  return isalnum(tok) || isspace(tok) || tok == '_';
}

static Token* createToken()
{
  Token* tok = NULL;
  calloc_or_exit(tok, 1, sizeof(*tok),
                 "Could not create a Token");
  return tok;
}

static void freeContent(Token* tok)
{
  tok->size = 0;
}

static void freeToken(Token* tok)
{
  if (tok)
  {
    free(tok->string);
    free(tok);
  }
}

static void resizeToken(Token* tok)
{
  assert(tok != NULL);
  tok->allocated_size = tok->allocated_size * 1.2 + 256;
  realloc_or_exit(tok->string, tok->allocated_size,
                  "Could not resize the Token");
}

static void addSymbol(Token* tok, char symbol)
{
  assert(tok != NULL);
  if (tok->allocated_size <= tok->size + 1)
  {
    resizeToken(tok);
  }

  tok->string[tok->size++] = symbol;
}

void tokenizer(FILE *fp, TokenHandlerFunc tokenhandler, void *data)
{
  int symbol;
  Token* token = NULL;
  assert(fp);
  assert(tokenhandler);

  token = createToken();
  while ((symbol = fgetc(fp)) != EOF)
  {
    if (isValidSymbol(symbol))
    {
      if (isspace(symbol) && token->size > 0)
      {
        addSymbol(token, '\0');
        tokenhandler(token->string, data);
        freeContent(token);
      }
      else if (!isspace(symbol))
      {
        addSymbol(token, symbol);
      }
    }
    else if (token->size > 0)
    {
      addSymbol(token, '\0');
      tokenhandler(token->string, data);
      freeContent(token);
    }
  }
  if (token->size > 0)
  {
    addSymbol(token, '\0');
    tokenhandler(token->string, data);
    freeContent(token);
  }

  freeToken(token);
}
