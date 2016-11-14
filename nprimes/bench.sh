#!/bin/sh

CMD=$@

echo "nthreads,time"
for nthreads in $(seq 1 4); do
    echo "$nthreads,$(sh timeit.sh 1 1 $CMD $nthreads)"
done

