#!/bin/bash

# Check for right amount of arguments

if [ $# -lt 1 ] ; then
    echo "missing file name\n"

elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed\n"
else
    fname=$1
    # Check for filename
    if [ -f $fname ]; then 
        lines=$(wc -l < $fname | xargs)
        echo "$lines lines in $fname"
    else
        echo "$fname not found"

    fi 
fi


