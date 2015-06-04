.PHONY: clean cleanup test
CPPFLAGS=-g -Wall -Werror -O3

default: intset.x

OBJ=intset-main.o

intset.x: ${OBJ}
	${CXX} ${CPPFLAGS} -o $@ ${OBJ}

test: intset.x
	./intset-run.sh

intset-main.o: intset.hpp intset-impl.hpp

clean:
	rm -f *.o *.x
	rm -rf intset.x.dSYM/
