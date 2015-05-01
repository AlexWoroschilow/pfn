.PHONY: all clean example

CC=gcc
CFLAGS=-g -Wall -Werror -O3 -Wunused-parameter -Wpointer-arith

all: simul_evolution.x

%.x: %.c
	${CC} ${CFLAGS} -o $@ $<

test: test1 test2 test3 \
      test4 test5 test6 \
      test7 test8 test9

test1: simul_evolution.x
	./simul_evolution.x 1000 0.0 1000 1.0 10000 | tee test1.tmp
	grep -q fixed:B test1.tmp
	rm test1.tmp

test2: simul_evolution.x
	./simul_evolution.x 1000 1.0 1000 0.0 10000 | tee test2.tmp
	grep -q fixed:A test2.tmp
	rm test2.tmp

test3: simul_evolution.x
	./simul_evolution.x 1000 0.0 1000 0.0 10000 | tee test3.tmp
	grep -q "simulation stopped" test3.tmp
	rm test3.tmp

test4: simul_evolution.x
	./simul_evolution.x 1000 0.5 1000 0.5 10000

test5: simul_evolution.x
	./simul_evolution.x 1000 0.6 1000 0.5 10000

test6: simul_evolution.x
	./simul_evolution.x 1000 1.0 1000 1.0 10000

test7: simul_evolution.x
	./simul_evolution.x 2000 0.51 100000 0.5 10000

test8: simul_evolution.x
	./simul_evolution.x 2000 0.5 100000 0.51 10000

test9: simul_evolution.x
	./simul_evolution.x 2000 0.5 100000 0.51 10000 results.test9.dat

clean:
	${RM} -f *.x *~ *.dat *.tmp *.pdf
