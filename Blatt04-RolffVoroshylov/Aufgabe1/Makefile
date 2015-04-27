.PHONY: all clean
CC=gcc
CFLAGS=-Wall -Werror -Wunused-parameter -ggdb

all: revcomp.x

%.x: %.c
	${CC} $< -o $@

clean:
	${RM} -f *.x ecoli.seq.rc

test: revcomp.x
	valgrind ./revcomp.x ecoli.seq
	diff ecoli.seq.rc expected_output
