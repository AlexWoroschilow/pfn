#Tim Rolff, Oleksand Voroshylov

.PHONY: all
CFLAGS=-Wall -Werror -Wunused -g

opt?=no
ifeq (${opt}, yes)
  CFLAGS+= -O3
endif

all: queuetest.x

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $^

queuetest.x: queue.o queuetest.o
	${CC} ${CFLAGS} -o $@ $^

clean:
	${RM} -f *.x
	${RM} -f *.o

# vim:ft=make
