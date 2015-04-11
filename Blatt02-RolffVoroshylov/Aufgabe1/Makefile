.PHONY: all clean cleanup test

CFLAGS+=-ggdb -Wall -Werror -Wunused-parameter -Wunused-variable

all: subsetsum-test.x test

subsetsum-test.x: subsetsum.o subsetsum-test.o int_conv.o
	${CC} -o $@ $^

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x

test: subsetsum-test.x
	@./test.sh | diff test.out -
