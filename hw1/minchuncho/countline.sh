#!/bin/bash

if [[ $# < 1 ]] ; then
	echo -e "missing file name"
elif [[ $# > 1 ]] ; then
	echo -e "only one argument is allowed"
else
	filename=$1
	if [[ -f "$filename" ]] ; then
		lines=$(wc -l <$filename | sed -e 's/^[ \t]*//')
		printf "$lines lines in $filename\n"
	else
		echo -e "$filename not found"
	fi
fi