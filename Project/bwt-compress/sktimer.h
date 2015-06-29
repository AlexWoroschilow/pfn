#ifndef SKTIMER_H
#define SKTIMER_H

typedef struct GtSKtimer GtSKtimer;

GtSKtimer *gt_SKtimer_new(void);

void gt_SKtimer_start(GtSKtimer *sktimer);

double gt_SKtimer_elapsed(GtSKtimer *sktimer);

double gt_SKtimer_total(GtSKtimer *sktimer);

void gt_SKtimer_delete(GtSKtimer *sktimer);

#endif
