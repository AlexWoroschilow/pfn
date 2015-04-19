.PHONY: all clean
CC=gcc
CFLAGS=-Wall -Werror -Wunused-parameter -ggdb

all: macro_tests.x

%.x: %.c
	${CC} $< -o $@

clean:
	${RM} *.x

test: macro_tests.x
	./macro_tests.x
