.PHONY: all clean test
CC=gcc
CFLAGS=-Wall -Werror -Wunused-parameter -ggdb -lm -std=c89

all: enumkmers.x

# if int_conv is used, uncomment this line and have int_conv.[ch] in this
# directory
#
enumkmers.x: int_conv.o

%.x: %.o
	${CC} -o $@ $^

clean:
	${RM} *.x *.o

test: enumkmers.x
	./test.sh | diff test.out -
