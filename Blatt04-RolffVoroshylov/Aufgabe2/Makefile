.PHONY: clean cleanup test
CFLAGS=-g -Wall -Werror -O3

all: test
OBJ=intset.o intset-main.o

intset.x:${OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ}

intset.o intset-main.o: intset.h

intset.o: intset-simple.c
	${CC} ${CFLAGS} -c -o $@ $<

test: intset.x
	./intset-run.sh

clean:
	rm -f *.o *.x
	rm -rf intset.x.dSYM/
