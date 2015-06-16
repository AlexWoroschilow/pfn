#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include <sys/time.h>

#define NUMVALUES (1 << 20)
/* 128 */
#define NUMSORTS (1<<7)


/* Das c Programm laeuft um den Faktor 10 schneller als das c++ Programm
 */

static int timeval_subtract(struct timeval *result,
                            struct timeval *x,
                            struct timeval *y)
{
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;
  return x->tv_sec < y->tv_sec;
}

static int timeval_add(struct timeval *result,
                       const struct timeval *x,
                       const struct timeval *y)
{
  result->tv_sec = x->tv_sec + y->tv_sec;
  result->tv_usec = x->tv_usec + y->tv_usec;
  while (result->tv_usec > 1000000) {
    result->tv_usec -= 1000000;
    result->tv_sec++;
  }
  return 0;
}

int main(void)
{
  unsigned long sortnum;
  struct timeval start, end, diff, tmp, runtime = {0,0};
  std::vector<int> values(NUMVALUES);

  for (sortnum = 0; sortnum < NUMSORTS; ++sortnum) {
    std::generate_n(values.begin(), NUMSORTS, std::rand);
    (void) gettimeofday(&start, NULL);

    std::sort(values.begin(), values.end(), std::less<int>());


    (void) gettimeofday(&end, NULL);
    (void) timeval_subtract(&diff, &end, &start);
    tmp = runtime;
    (void) timeval_add(&runtime, &tmp, &diff);
  }

  std::cout << "Runtime (hh:mm:ss:us)\t"
			<< std::setfill ('0') << std::setw (2)
            << runtime.tv_sec / 60LU / 60LU
            
            << ":" << std::setfill ('0') << std::setw (2)
            << (runtime.tv_sec / 60LU) % 60LU
            
            << ":" << std::setfill ('0') << std::setw (2)
            << runtime.tv_sec % 60LU
            
            << ":" << runtime.tv_usec
            << std::endl;
  return 0;
}
