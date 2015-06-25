GCC=gcc
LD=${GCC}
CFLAGS= -I/usr/include/python3.4m/ -g -O3 -Wall -Werror -Wunused-parameter -Wpointer-arith -fpic
LDFLAGS=-g -shared

all: matrixmultiply.so

OBJS=matrixmultiply.o

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

matrixmultiply.so:${OBJS}
	${LD} $(LDFLAGS) -o $@ ${OBJS}

test: matrixmultiply.so
	@python3 matrixmultiplication.py

clean:
	${RM} *.so *.o