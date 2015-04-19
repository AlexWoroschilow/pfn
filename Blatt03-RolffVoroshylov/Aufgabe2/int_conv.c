#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "int_conv.h"

int get_int(const char *const number)
{
  long tmp;
  tmp = get_long(number);
  if (tmp > INT_MAX || tmp < INT_MIN) {
    fprintf(stderr, "%ld is to large for an integer\n", tmp);
    exit(EXIT_FAILURE);
  }
  return (int) tmp;
}

long get_long(const char *const number)
{
  char *end;
  long result;

  errno = 0;
  result = strtol(number, &end, 0);

  if (end == number) {
    fprintf(stderr, "%s: not a decimal number\n", number);
    exit(EXIT_FAILURE);
  }
  if ('\0' != *end) {
    fprintf(stderr, "%s: extra characters at end of input: %s\n",
            number, end);
    exit(EXIT_FAILURE);
  }
  if (((LONG_MIN == result || LONG_MAX == result) && ERANGE == errno) ||
      (errno != 0 && result == 0)) {
    fprintf(stderr, "%s out of range of type long?\n", number);
    perror("strtol");
    exit(EXIT_FAILURE);
  }
  return result;
}
