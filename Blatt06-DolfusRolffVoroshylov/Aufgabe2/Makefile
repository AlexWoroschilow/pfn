.PHONY: clean cleanup test
CFLAGS=-g -Wall -Werror -O3

all: test
OBJ=intset.o intset-main.o

intset.x:${OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ}

test: intset.x
	./intset-run.sh

intset.o intset-main.o:intset.h

clean:
	rm -f *.o *.x
	rm -rf *.x.dSYM/
