#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "intset.hpp"

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

template<typename Basetype>
static void checknotinrange(const IntSet<Basetype> *separator_set,
                            unsigned long left,unsigned long right)
{
  unsigned long j;

  for (j = left; j <= right; j++)
  {
    if (separator_set->is_member(j))
    {
      fprintf(stderr,"j=%lu in [%lu,%lu] not expected\n",j,left,right);
      exit(EXIT_FAILURE);
    }
  }
}

template<typename Basetype>
static void checkseqnum(const IntSet<Basetype> * separator_set,
                        unsigned long seqnum,
                        unsigned long left,unsigned long right)
{
  unsigned long j;

  for (j = left; j <= right; j++)
  {
    unsigned long seqnum2 = separator_set->number_next_larger(j);

    if (seqnum2 != seqnum)
    {
      fprintf(stderr,"range %lu in [%lu,%lu]: seqnum2 = %lu != %lu = seqnum\n",
                 j,left,right,seqnum2,seqnum);
      exit(EXIT_FAILURE);
    }
  }
}

template<typename Basetype>
static void checkconsistency(const unsigned long *numbers,
                             unsigned long nofelements,
                             unsigned long maxvalue,
                             const IntSet<Basetype> *separator_set,
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

    if (!separator_set->is_member(numbers[idx]))
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

template<typename Basetype>
static unsigned long runtrials(const IntSet<Basetype> *separator_set,
                               unsigned long maxvalue,
                               unsigned long trials)
{
  unsigned long idx, countmember = 0;

  for (idx = 0; idx < trials; idx++)
  {
    unsigned long num = drand48() * (maxvalue+1);
    if (separator_set->is_member(num))
    {
      countmember++;
    }
  }
  return countmember;
}

template<typename Basetype>
static void checkall(IntSet<Basetype> *separator_set,
                     const Arguments *arguments,
                     const ArrayUlong *arr)
{
  size_t size = separator_set->size(arguments->maxvalue,
                                    arr->nofelements);
  printf("sizeof(Intset)=%lu (%.2f bits/number)\n",
         size,(double) size/arr->nofelements * CHAR_BIT);
  for (unsigned long idx = 0; idx < arr->nofelements; idx++)
  {
    separator_set->add(arr->numbers[idx]);
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
  // separator_set->pretty_print();
}

static int smallest_representation(unsigned long maxvalue,
                                   unsigned long nofelements)
{
  int best = 8;
  size_t currentsize,
         minsize = IntSet<uint8_t>::size(maxvalue, nofelements);

  currentsize = IntSet<uint16_t>::size(maxvalue, nofelements);
  if (currentsize < minsize)
  {
    minsize = currentsize;
    best = 16;
  }
  currentsize = IntSet<uint32_t>::size(maxvalue, nofelements);
  if (currentsize < minsize)
  {
    best = 32;
  }
  return best;
}

int main(int argc, char *argv[])
{
  ArrayUlong *arr;
  Arguments arguments;
  int best;

  parse_options(&arguments,argc,argv);
  printf("maxvalue=%lu\n",arguments.maxvalue);
  printf("nofelements=%lu\n",arguments.nofelements);
  printf("trials=%lu\n",arguments.trials);
  printf("test_seqnum=%s\n",arguments.test_seqnum ? "true" : "false");
  arr = ordered_set_of_random_numbers(arguments.maxvalue,
                                      arguments.nofelements);
  printf("remaining: %lu\n",arr->nofelements);
  best = smallest_representation(arguments.maxvalue,arr->nofelements);
  if (best == 8)
  {
    IntSet<uint8_t> *separator_set;
    separator_set = new IntSet<uint8_t>(arguments.maxvalue,arr->nofelements);
    checkall(separator_set,&arguments,arr);
    delete separator_set;
  } else
  {
    if (best == 16)
    {
      IntSet<uint16_t> *separator_set;
      separator_set = new IntSet<uint16_t>(arguments.maxvalue,arr->nofelements);
      checkall(separator_set,&arguments,arr);
      delete separator_set;
    } else
    {
      assert(best == 32);
      IntSet<uint32_t> *separator_set;
      separator_set = new IntSet<uint32_t>(arguments.maxvalue,arr->nofelements);
      checkall(separator_set,&arguments,arr);
      delete separator_set;
    }
  }
  free(arr->numbers);
  free(arr);
  return EXIT_SUCCESS;
}
