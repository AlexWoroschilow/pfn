.PHONY: clean cleanup test
CFLAGS=-g -Wall -Werror -O3 -Wunused-parameter

# comment the following for the space efficient version
# SIMPLE=-simple

OBJ=wordstat.o bintree.o tokenizer.o

wordstat.o bintree.o: bintree.h

wordstat.x: ${OBJ}
	${CC} -o $@ ${OBJ}

clean:
	rm -f *.o *.x 
	rm -rf wordstat.x.dSYM/

test: wordstat.x
	./$< wordstat.c 0 | diff - test0.out
	./$< wordstat.c 5 | diff - test5.out
