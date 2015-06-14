CFLAGS=-Wall -Werror -Wunused -Wunused-parameter -g -O3
CXXFLAGS=${CFLAGS} -std=c++11 -g -O3
CC?=gcc
CXX?=g++

all: sort_benchmark-c.x sort_benchmark-cpp.x

%.x: %.c
	${CC} ${CFLAGS} -o $@ $<

%.x: %.cpp
	${CXX} ${CXXFLAGS} -o $@ $<

clean:
	${RM} *.o *.x
