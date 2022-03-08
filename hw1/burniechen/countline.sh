#!/bin/bash

count=0
if (($# < 1)); then
	echo "missing file name"

elif (($# > 1)); then
	echo "only one argument is allowed"

else
	if [ -f $1 ]; then
		while read line
		do
			count=$((count+1))
		done < $1

		echo "$count, $1"
	else
		echo "$1 not found"
	fi
fi
