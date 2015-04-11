.PHONY: all
CFLAGS=-Wall -Werror -g

opt?=no
ifeq (${opt}, yes)
  CFLAGS+= -O3
endif

all: queuetest.x

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $^

queuetest.x: queue.o queuetest.o
	${CC} ${CFLAGS} -o $@ $^

# vim:ft=make
