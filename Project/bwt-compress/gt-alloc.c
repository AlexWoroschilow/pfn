/*
  Copyright (c) 2012 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2012 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include "gt-alloc.h"

void gt_free(void *ptr)
{
  if (ptr != NULL)
  {
    free(ptr);
  }
}

void *gt_malloc(size_t size)
{
  void *ptr = malloc(size);

  if (ptr == NULL)
  {
    fprintf(stderr,"%s(%lu) failed\n",__func__,(unsigned long) size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *gt_calloc(size_t count,size_t size)
{
  void *ptr = calloc(count,size);

  if (ptr == NULL)
  {
    fprintf(stderr,"%s(%lu,%lu) failed\n",__func__,(unsigned long) count,
                                                   (unsigned long) size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *gt_realloc(void *ptr,size_t size)
{
  ptr = realloc(ptr,size);

  if (ptr == NULL)
  {
    fprintf(stderr,"%s(%lu) failed\n",__func__,(unsigned long) size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}
