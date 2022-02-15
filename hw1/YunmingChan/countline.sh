#!/bin/bash

if [ "$#" -lt "1" ]; then
	echo missing file name
elif [ "$#" -gt "1" ]; then
	echo only one argument is allowed
else
	fname="$1"
	if [ -e "$fname" ]; then
		echo $(cat "$fname" | wc -l) "lines in $fname"
	else
		echo "$fname not found"
	fi
fi

