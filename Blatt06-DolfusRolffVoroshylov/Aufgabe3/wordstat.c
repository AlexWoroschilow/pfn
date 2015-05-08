#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "tokenizer.h"
#include "bintree.h"

typedef struct
{
  unsigned long count;
  char *word;
} Wordinfo;

static int cmp_wordinfo(const void *a,const void *b)
{
  const Wordinfo *wia = (const Wordinfo *) a;
  const Wordinfo *wib = (const Wordinfo *) b;

  return strcmp(wia->word,wib->word);
}

static void combine_nodeinfo(void *node_value,
                             __attribute__ ((unused)) /*@unused@*/
                             const void *new_value)
{
  Wordinfo *word_node_value = node_value;
  word_node_value->count++;
}

static void delete_wordinfo(void *a)
{
  Wordinfo *wi = (Wordinfo *) a;

  if (wi != NULL)
  {
    free(wi->word);
    free(wi);
  }
}

static void insert_wordinfo(const char *s,void *data)
{
  GenBinTree *gbt = (GenBinTree *) data;
  Wordinfo *wi = malloc(sizeof *wi);

  assert(wi != NULL);
  wi->count = 1UL;
  wi->word = strdup(s);
  assert(gbt != NULL);
  if (!gbt_add(gbt,wi))
  {
    free(wi->word);
    free(wi);
  }
}

void showfrequentwords(const void *void_value,void *data)
{
  const Wordinfo *wi = (const Wordinfo *) void_value;
  unsigned long minfreq = *((unsigned long *) data);

  if (wi->count >= minfreq)
  {
    printf("%s\t%lu\n",wi->word,wi->count);
  }
}
   
int main(int argc, char *argv[])
{
  FILE *fp;
  GenBinTree *gbt;
  long readlong;
  unsigned long minfreq;

  if (argc != 3 || sscanf(argv[2],"%ld",&readlong) != 1 || readlong < 0)
  {
    fprintf(stderr,"Usage: %s <inputfile> <minfreq>\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  fp = fopen(argv[1],"rb");
  if (fp == NULL)
  {
    fprintf(stderr,"%s: cannot open file %s: %s\n",
            argv[0],argv[1],strerror(errno));
    exit(EXIT_FAILURE);
  }
  gbt = gbt_new(cmp_wordinfo,combine_nodeinfo,delete_wordinfo);
  tokenizer(fp,insert_wordinfo,(void *) gbt);
  minfreq = (unsigned long) readlong;
  gbt_enumvalues(gbt,showfrequentwords,&minfreq);
  gbt_delete(gbt);
  (void) fclose(fp);
  exit(EXIT_SUCCESS);
}
