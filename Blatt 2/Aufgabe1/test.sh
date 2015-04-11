#!/bin/sh

for num in `seq 100 1000`
do
  ./subsetsum-test.x $num
done
