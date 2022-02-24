#!/bin/bash

if [[ $# < 1 ]] ; then
    echo -e "missing file name\n"
elif [[ $# > 1 ]] ; then
    echo -e "only one argument is allowed\n"
else
    fname=$1
    if [[ -f "$fname" ]] ; then
        NumsofLines=$(wc -l countline.sh | tr -dc '0-9')
        echo -e "$NumsofLines lines in $fname\n"
    else
        echo -e "$fname not found\n"
    fi
fi