#!/bin/bash

if [ "$#" -lt 1 ]
then
	echo "missing file name\n"
	exit 1
elif [ "$#" -gt 1 ]
then
	echo "only one argument is allowed\n"
	exit 1
else
	echo $(wc --line < $1)
fi

