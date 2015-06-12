.PHONY: all clean cleanup test
CXX=clang++
CXXFLAGS=-Wall -Wunused-parameter -Werror -g -O3

all: wortketten.x

%.x: %.o
	${CXX} ${CXXFLAGS} -o $@ $^

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x

test: wortketten.x 5BuchstabenWoerter.txt Wortpaare.txt
	@./wortketten.x 5BuchstabenWoerter.txt Wortpaare.txt | diff result.txt -

5BuchstabenWoerter.txt:
	@grep -P -e '^[^ÄÖÜßäöüéê]{5}$$' /usr/share/dict/ngerman > $@

Wortpaare.txt: 5BuchstabenWoerter.txt
	@shuf $< | \
	awk '{if (NR%2==1) printf "%s\t",$$0; else print $0}' | \
	head -n 100 > $@
