#!/bin/sh

REPEAT=$1
shift
NUMBER=$1
shift

CMD=$@

for ((i=0; i < $NUMBER; ++i)); do
    for ((j=0; j < $REPEAT; ++j)); do
        VALUES="$(/usr/bin/time $CMD 2>&1 > /dev/null | awk  '{print $1}')\n$VALUES"
    done

    MIN="$(echo $VALUES | st --min 2> /dev/null)\n$MIN" 
    VALUES=""
done

echo $MIN | st --median 2> /dev/null
