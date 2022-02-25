#!/bin/bash

if [ $# -lt 1 ]; then
    echo "missing file name"
elif [ $# -gt 1 ]; then
    echo "only one argument is allowed"
else
    FILE=$1
    if [ -f "$FILE" ]; then
        linecount=`wc -l $FILE | awk '{ print $1 }'`
        echo "$linecount lines in $FILE"
    else
        echo "$FILE not found"
    fi
fi