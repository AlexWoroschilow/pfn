.PHONY: clean cleanup test
CFLAGS=-g -Wall -Werror -O3 -Wunused-parameter

# comment the following for the space efficient version
# SIMPLE=-simple

OBJ=fastq-compress.o fastq-concat/fastq-concat.o fastq-concat/fastq-parse/fastq-parse.o bwt-compress/bwt-compress.o bwt-compress/gt-alloc.o bwt-compress/sk-sain.o bwt-compress/sktimer.o



fastq-compress.x: ${OBJ}
	${CC} -o $@ ${OBJ}




%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)


clean:
	rm -f *.o *.x 
#
#test: wordstat.x
#	./$< wordstat.c 0 | diff - test0.out
#	./$< wordstat.c 5 | diff - test5.out
