.PHONY: clean test
CXX=g++
LD=${CXX}
CFLAGS=-g -O3 -Wall -Werror -Wunused-parameter -Wpointer-arith
LDFLAGS=-g -lm

all:hashqual.x

OBJEVALHASHFUNC=hashqual.o tokenizer.o hashqual-functions.o

# generic compilation rule which creates dependency file on the fly
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

hashqual.x:${OBJEVALHASHFUNC}
	${LD} -g -o $@ ${OBJEVALHASHFUNC}

test:hashqual.x
	cat shaks.data | ./hashqual.x | diff - shaks.quality

# read dependencies
-include $(wildcard *.d)

clean:
	${RM} *.[oxd]
