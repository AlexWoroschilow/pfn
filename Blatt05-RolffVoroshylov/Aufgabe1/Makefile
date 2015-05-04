.PHONY: clean cleanup test
CFLAGS=-g -Wall -Werror -O3 -Wunused-parameter

all: enumwords.x

OBJ=enumwords.o tokenizer.o

enumwords.x:${OBJ}
	${CC} ${CFLAGS} -o $@ $^

enumwords.o tokenizer.o:tokenizer.h

test: enumwords.x
	@./enumwords.x text.txt | diff test.out -

clean:
	rm -f *.o *.x 
	rm -rf *x.dSYM/
