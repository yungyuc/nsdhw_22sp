#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo "missing file name"
    exit 0
fi

if [[ $2 -ne 0 ]] ; then
    echo "only one argument is allowed"
    exit 0
fi

if [[ -f $1 ]]; then
    line=$(cat $1 | wc -l)
    echo "${line} lines in $1"
else
    echo "$1 not found"
fi
