.PHONY:clean test cleanup
CFLAGS=-g -O3 -Wall -Werror -Wunused-parameter -Wpointer-arith 
LDFLAGS=-lm -pthread
CC?=clang

SEQFILE=sw175.fna

OBJ=all_against_all.o multiseq.o unit_edist.o bestkvals.o

all:all_against_all.x

# generic compilation rule which creates dependency file on the fly
%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

all_against_all.x:${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS} 

test: test_expected_results test_multithreaded

test_expected_results:	all_against_all.x ${SEQFILE}
	./all_against_all.x 50 1 ${SEQFILE} | diff - expected.results

test_multithreaded:	all_against_all.x ${SEQFILE}
	./all_against_all.x 50 4 ${SEQFILE} | grep -v '^#' > result.4
	./all_against_all.x 50 1 ${SEQFILE} | grep -v '^#' > result.1
	diff result.1 result.4
	rm -f result.1 result.4

# read dependencies
-include $(wildcard *.d)

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x result* *.d

