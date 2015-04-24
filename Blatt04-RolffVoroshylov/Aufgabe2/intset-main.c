#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "intset.h"

static void usage(const char *progname)
{
  const char *optionsmsg =
    "  -s\ttest sequence numbers\n"
    "  -t\trun trials\n"
    "  -h\tshow this usage message\n";
  fprintf(stderr, "Usage: %s [OPTION] maxvalue nofelements\n%s",
          progname,
          optionsmsg);
}

typedef struct
{
  unsigned long maxvalue,
                nofelements,
                trials;
  bool test_seqnum;
} Arguments;

static void parse_options(Arguments *arguments,int argc,char *const argv[])
{
  int opt;
  long readlong;

  arguments->trials = 0;
  arguments->test_seqnum = false;
  while ((opt = getopt(argc, argv, "st:")) != -1)
  {
    switch ((char) opt)
    {
      case 's':
        arguments->test_seqnum = true;
        break;
      case 't':
        if (sscanf(optarg,"%ld",&readlong) != 1 || readlong <= 0)
        {
          usage(argv[0]);
          exit(EXIT_FAILURE);
        }
        arguments->trials = (unsigned long) readlong;
        break;
      case 'h':
        usage(argv[0]);
        exit(EXIT_SUCCESS);
      default:
        assert((char) opt == '?');
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (optind + 1 >= argc)
  {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  if (sscanf(argv[optind],"%ld",&readlong) != 1 || readlong <= 0)
  {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  arguments->maxvalue = (unsigned long) readlong;
  if (sscanf(argv[optind+1],"%ld",&readlong) != 1 || readlong <= 0)
  {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  arguments->nofelements = (unsigned long) readlong;
}

typedef struct
{
  unsigned long nofelements, *numbers;
} ArrayUlong;

static int compareUlong(const void *aptr,const void *bptr)
{
  unsigned long a = *((unsigned long *) aptr),
                b = *((unsigned long *) bptr);

  return (a < b) ? -1 : ((a > b) ? 1 : 0);
}

static void remove_duplicates(ArrayUlong *arr)
{
  unsigned long *rptr;

  assert(arr->nofelements > 0);
  if (arr->nofelements > 1UL)
  {
    unsigned long *wptr = arr->numbers, *rptr;

    for (rptr = arr->numbers + 1; rptr < arr->numbers + arr->nofelements;
         rptr++)
    {
      assert(*wptr <= *rptr);

      if (*wptr < *rptr)
      {
        wptr++;
        *wptr = *rptr;
      }
    }
    arr->nofelements = (unsigned long) (wptr - arr->numbers + 1);
  }
  for (rptr = arr->numbers + 1; rptr < arr->numbers + arr->nofelements; rptr++)
  {
    assert(*(rptr-1) < *rptr);
  }
  arr->numbers = (unsigned long *) realloc(arr->numbers,arr->nofelements *
                                                        sizeof *(arr->numbers));
  assert(arr->numbers != NULL);
}

static ArrayUlong *ordered_set_of_random_numbers(unsigned long maxvalue,
                                                 unsigned long nofelements)
{
  unsigned long idx;
  ArrayUlong *arr = (ArrayUlong *) malloc(sizeof *arr);

  assert(arr != NULL);
  arr->numbers = (unsigned long *) malloc(nofelements * sizeof *arr->numbers);
  assert(arr->numbers != NULL);
  srand48(366292341);
  for (idx = 0; idx < nofelements; idx++)
  {
    arr->numbers[idx] = drand48() * (maxvalue+1);
  }
  arr->nofelements = nofelements;
  qsort((void *) arr->numbers,(size_t) arr->nofelements,sizeof *(arr->numbers),
        compareUlong);
  remove_duplicates(arr);
  return arr;
}

static void checknotinrange(const IntSet *separator_set,
                            unsigned long left,unsigned long right)
{
  unsigned long j;

  for (j = left; j <= right; j++)
  {
    if (intset_is_member(separator_set,j))
    {
      fprintf(stderr,"j=%lu in [%lu,%lu] not expected\n",j,left,right);
      exit(EXIT_FAILURE);
    }
  }
}

static void checkseqnum(const IntSet *separator_set,unsigned long seqnum,
                        unsigned long left,unsigned long right)
{
  unsigned long j;

  for (j = left; j <= right; j++)
  {
    unsigned long seqnum2 = intset_number_next_larger(separator_set,j);

    if (seqnum2 != seqnum)
    {
      fprintf(stderr,"range %lu in [%lu,%lu]: seqnum2 = %lu != %lu = seqnum\n",
                 j,left,right,seqnum2,seqnum);
      exit(EXIT_FAILURE);
    }
  }
}

static void checkconsistency(const unsigned long *numbers,
                             unsigned long nofelements,
                             unsigned long maxvalue,
                             const IntSet *separator_set,
                             bool test_seqnum)
{
  unsigned long idx;

  if (numbers[0] > 0)
  {
    checknotinrange(separator_set,0,numbers[0]-1);
    if (test_seqnum)
    {
      checkseqnum(separator_set,0,0,numbers[0]-1);
    }
  }
  for (idx = 0; idx < nofelements; idx++)
  {
    unsigned long left, right;

    if (!intset_is_member(separator_set,numbers[idx]))
    {
      fprintf(stderr,"%lu in intset expected\n",numbers[idx]);
      exit(EXIT_FAILURE);
    }
    left = numbers[idx] + 1;
    assert(idx >= nofelements -1 || numbers[idx+1] > 0);
    right = idx < nofelements - 1 ? numbers[idx+1] - 1 : maxvalue;
    checknotinrange(separator_set,left,right);
    if (test_seqnum)
    {
      checkseqnum(separator_set,idx+1,left,right);
    }
  }
  printf("%s okay\n",__func__);
}

static unsigned long runtrials(const IntSet *separator_set,
                               unsigned long maxvalue,
                               unsigned long trials)
{
  unsigned long idx, countmember = 0;

  for (idx = 0; idx < trials; idx++)
  {
    unsigned long num = drand48() * (maxvalue+1);
    if (intset_is_member(separator_set,num))
    {
      countmember++;
    }
  }
  return countmember;
}

static void checkall(IntSet *separator_set,
                     const Arguments *arguments,
                     const ArrayUlong *arr)
{
  unsigned long idx;
  size_t size = intset_size(arguments->maxvalue,arr->nofelements);

  printf("sizeof(Intset)=%zu (%.2f bits/number)\n",
         size,(double) size/arr->nofelements * CHAR_BIT);
  for (idx = 0; idx < arr->nofelements; idx++)
  {
    intset_add(separator_set,arr->numbers[idx]);
  }
  if (arguments->trials == 0)
  {
    checkconsistency(arr->numbers,arr->nofelements,arguments->maxvalue,
                     separator_set,arguments->test_seqnum);
  } else
  {
    unsigned long countmember
      = runtrials(separator_set,arguments->maxvalue,arguments->trials);
    printf("run %lu trials: %lu members found\n",arguments->trials,
                                                 countmember);
  }
  /*intset_pretty(separator_set);*/
}

int main(int argc, char *argv[])
{
  ArrayUlong *arr;
  IntSet *separator_set;
  Arguments arguments;

  parse_options(&arguments,argc,argv);
  printf("maxvalue=%lu\n",arguments.maxvalue);
  printf("nofelements=%lu\n",arguments.nofelements);
  printf("trials=%lu\n",arguments.trials);
  printf("test_seqnum=%s\n",arguments.test_seqnum ? "true" : "false");
  arr = ordered_set_of_random_numbers(arguments.maxvalue,
                                      arguments.nofelements);
  printf("remaining: %lu\n",arr->nofelements);
  separator_set = intset_new(arguments.maxvalue,arr->nofelements);
  checkall(separator_set,&arguments,arr);
  intset_delete(separator_set);
  free(arr->numbers);
  free(arr);
  return EXIT_SUCCESS;
}
