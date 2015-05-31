#ifndef MULTISEQ_H
#define MULTISEQ_H

typedef struct Multiseq Multiseq;

typedef struct MultiseqItem MultiseqItem;

Multiseq * muliseq_new(char * filename);

MultiseqItem * muliseq_item_next(Multiseq * multiseq);

void muliseq_item_show(MultiseqItem * item);

void muliseq_destroy(Multiseq * multiseq);

#endif
