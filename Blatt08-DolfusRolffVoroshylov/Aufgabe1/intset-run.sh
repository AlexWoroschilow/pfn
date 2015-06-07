#!/bin/sh

set -e -x

for maxvalue in 100 1000 2000 50000 100000 1000000 2000000
do
  numofelems=${maxvalue}
  while test ${numofelems} -le 4000000
  do
    valgrind --error-exitcode=1 ./intset.x -s ${maxvalue} ${numofelems}
    numofelems=`expr ${numofelems} \* 2`
  done
done
