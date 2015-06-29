#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
/* OS X does not have clock_gettime. Implement the function
   as suggest in http://stackoverflow.com/questions/5167269/
                        clock-gettime-alternative-in-mac-os-x */

#include "sktimer.h"

#ifdef __MACH__
#define CLOCK_MONOTONIC 0

static int clock_gettime(__attribute__ ((unused)) /*@unused@*/
                         int clk_id, struct timespec *tp)
{
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  tp->tv_sec = mts.tv_sec;
  tp->tv_nsec = mts.tv_nsec;
  return 0;
}
#endif

struct GtSKtimer
{
  struct timespec starttime, currenttime;
  bool has_been_started;
};

GtSKtimer *gt_SKtimer_new(void)
{
  GtSKtimer *sktimer = malloc(sizeof *sktimer);
  sktimer->has_been_started = false;
  return sktimer;
}

void gt_SKtimer_start(GtSKtimer *sktimer)
{
  (void) clock_gettime(CLOCK_MONOTONIC,&sktimer->currenttime);
  sktimer->starttime = sktimer->currenttime;
  sktimer->has_been_started = true;
}

#define GTSKTIMER_DIVISIOR 1000000000.0

double gt_SKtimer_elapsed(GtSKtimer *sktimer)
{
  struct timespec previoustime;
  assert(sktimer->has_been_started);

  previoustime = sktimer->currenttime;
  (void) clock_gettime(CLOCK_MONOTONIC,&sktimer->currenttime);
  return sktimer->currenttime.tv_sec - previoustime.tv_sec +
         (sktimer->currenttime.tv_nsec - previoustime.tv_nsec)/
         GTSKTIMER_DIVISIOR;
}

double gt_SKtimer_total(GtSKtimer *sktimer)
{
  assert(sktimer->has_been_started);

  (void) clock_gettime(CLOCK_MONOTONIC,&sktimer->currenttime);
  return sktimer->currenttime.tv_sec - sktimer->starttime.tv_sec +
         (sktimer->currenttime.tv_nsec - sktimer->starttime.tv_nsec)/
         GTSKTIMER_DIVISIOR;
}

void gt_SKtimer_delete(GtSKtimer *sktimer)
{
  free(sktimer);
}
