#!/bin/bash

if [ "$#" -lt 1 ]
then
    echo "missing file name"
elif [ "$#" -gt 1 ]
then
    echo "only one argument is allowed"
else
    if [ -f "$1" ]
    then
        output=$(wc -l $1)
        #echo $output
        number=$(echo $output | cut -d" " -f 1)
        echo "$number lines in $1"
    else
        echo "$1 not found"
    fi
fi