.PHONY: test clean
# choose one:
#CC=clang
CC=gcc
CFLAGS=-Wall -Werror -g -Wunused
LDFLAGS=-lm

ifdef opt
CFLAGS+= -O3
endif

sorting.x: sorting.o sorting_main.o int_conv.o
	${CC} ${LDFLAGS} $^ -o $@

clean:
	${RM} *.o *.x

test: sorting.x
	./sorting.x 1000
	./sorting.x 10000
	./sorting.x 100000
	./sorting.x 1000000
