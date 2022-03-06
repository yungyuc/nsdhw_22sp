#!/bin/bash

if [[ $# -ne 1 ]] ; then
    echo "Only specify one file argument "
    exit 0
fi

if [[ -f $1 ]]; then
    echo $(cat $1 | wc -l)
else
    echo "$1 not found"
fi