#!/bin/bash

if [ $# -lt 1 ]; then
    echo "missing file name"
elif [ $# -gt 1 ]; then
    echo "only one argument is allowed"
elif [ -e $1 ]; then
    count=`(cat $1 | wc -l) | xargs`
    echo "$count lines in $1"
else
    echo "$1 not found"
fi
