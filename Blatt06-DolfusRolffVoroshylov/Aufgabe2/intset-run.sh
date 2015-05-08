#!/bin/sh

set -e -x

for maxvalue in 100 1000 2000 50000 100000 1000000 2000000
do
  numofelems=100
  while test ${numofelems} -le ${maxvalue}
  do
    ./intset.x ${maxvalue} ${numofelems}
    numofelems=`expr ${numofelems} \* 2`
  done
done
