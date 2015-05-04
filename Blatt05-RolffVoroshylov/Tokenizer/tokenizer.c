#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#include "tokenizer.h"

#define assert_with_message(x, message) \
  if (!(x)) { \
    fprintf(stderr, "%s\n", message); \
    exit(EXIT_FAILURE); \
  }
  
#define calloc_or_exit(x, elems, size) \
  if (!(x = calloc(elems, size))) { \
    fprintf(stderr, "Could not calloc memory of size %lu\n", \
           (unsigned long) (elems) * (size)); \
    exit(EXIT_FAILURE); \
  }

#define realloc_or_exit(x, size) \
  if (!(x = realloc(x, size))) { \
    fprintf(stderr, "Could not realloc memory of size %lu\n", \
           (unsigned long) size); \
    exit(EXIT_FAILURE); \
  }

/* Basic structure of a Token, where the string is the
   current parsed Word and the size the size of the characters in
   string
*/
typedef struct 
{
  char* string;
  unsigned long size;
} Token;

/* Check the input and return true if the symbol is in [a-zA-Z0-9]* */
static bool isValidSymbol(int tok)
{
  return isalnum(tok) || isspace(tok) || tok == '_';
}

/* create a new Token and set everything to zero */
static Token* createToken()
{
  Token* tok = NULL;
  calloc_or_exit(tok, 1, sizeof(*tok));
  return tok;
}

/* Free the string an set the size to 0.
   This does not free the Token itself!
*/
static void freeContent(Token* tok)
{
  if(tok->string)
  {
    free(tok->string);
    tok->string = NULL;
    tok->size = 0;
  }
}

/* Free whole Token
*/
static void freeToken(Token* tok)
{
  if(tok)
  {
    freeContent(tok);
    free(tok);
    tok = NULL;
  }
}

/* Resize the string in the Token by one */
static void resizeToken(Token* tok)
{
  assert(tok != NULL);
  ++tok->size;
  realloc_or_exit(tok->string, tok->size);
}

/* Add a symbol to the string */
static void addSymbol(Token* tok, char symbol)
{
  assert(tok != NULL);
  assert(isValidSymbol(symbol) && !isspace(symbol));
  
  resizeToken(tok);
  /* Because we resized the token by one we can add a new elment to the end */
  tok->string[tok->size  - 1] = symbol;
}

/* Tokenize the input in small substrings and call the tokenhandler function
   foreach parsed Token
*/
void tokenizer(FILE *fp, TokenHandlerFunc tokenhandler, void *data)
{
  assert(fp);
  assert(tokenhandler);

  int symbol;
  Token* token = createToken();
	while ((symbol = fgetc(fp)) != EOF)
	{
    /* Skip Control symbols */
    if(isValidSymbol(symbol))
    {
      /* Check if the Token ends and add the nullbyte character to it
         also reset everything for a new Token 
      */
      if(isspace(symbol) && token->size > 0)
      {
        addSymbol(token, '\0');
        tokenhandler(token->string, data);
        freeContent(token);
      }
      else if (!isspace(symbol))
      {
        /* Because the symbol isnt a space and a valid character 
           we can simply add it to the Token string 
        */
        addSymbol(token, symbol);
      }
    }
    /* End Token if we found an illigal character */
    else if(token->size > 0)
    {
      addSymbol(token, '\0');
      tokenhandler(token->string, data);
      freeContent(token);
    }
  }
  
  /* End the last token if there is one */
  if (token->size > 0)
  {
    addSymbol(token, '\0');
    tokenhandler(token->string, data);
    freeContent(token);
  }

  /* free everything */
	freeToken(token);
}
