.PHONY: clean cleanup test
CC=gcc
LD=${CC}
CFLAGS=-Wall -Werror -Wunused-parameter -ggdb
LDFLAGS=-lm

opt?=no

ifeq (${opt},yes)
	CFLAGS+= -O3
endif

MEMTESTOBJ=memtest.o memmanage.o

memtest.x: ${MEMTESTOBJ}
	${CC} ${LDFLAGS} ${MEMTESTOBJ} -o $@

memmanage.c: memmanage.h

memtest.c: memmanage.h memmanage-mac.h

test: memtest.x
	./memtest.x 2> err | diff memtest.out -
	diff memtest.err err

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x
