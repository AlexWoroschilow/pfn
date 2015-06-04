#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <cassert>
#include <iostream>
#include "tokenizer.hpp"

#define CHECKSPACE\
        if (nextfree >= allocated)\
        {\
          allocated += 100UL;\
          buffer = (char *) realloc(buffer,sizeof *buffer * allocated);\
          assert(buffer != NULL);\
        }

void tokenizer(TokenHandlerFunc tokenhandler,void *data)
{
  bool inword = false;
  unsigned long nextfree = 0, allocated = 0;
  char *buffer = NULL;

  while (std::cin.good())  // loop while extraction from file is possible
  {
    unsigned char cc = std::cin.get();   // get character from file

    if (isalpha(cc) || isdigit(cc) || cc == (int) '_')
    {
      CHECKSPACE;
      buffer[nextfree++] = (char) cc;
      inword = true;
    } else
    {
      if (inword)
      {
        CHECKSPACE;
        buffer[nextfree] = '\0';
        tokenhandler(buffer,data);
        nextfree = 0;
        inword = false;
      }
    }
  }
  if (inword)
  {
    CHECKSPACE;
    buffer[nextfree] = '\0';
    tokenhandler(buffer,data);
  }
  free(buffer);
}
