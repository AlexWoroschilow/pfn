#!/bin/sh

set -e -x

export VALGRIND_OPTS="--quiet --tool=memcheck"

TMPFILE=`mktemp TMP.XXXXXX` || exit 1
for filename in `ls fastq-files`
do
  exec `valgrind ./fastq-compress.x fastq-files/${filename} > ${TMPFILE}` 
  cmp -s fastq-files/${filename} ${TMPFILE}
done
rm -f ${TMPFILE}
