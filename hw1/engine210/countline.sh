#!/bin/bash

if [[ "$#" -lt 1 ]]; then
	echo 'missing file name'
elif [[ "$#" -gt 1 ]]; then
	echo 'only one argument is allowed'
else
	if [[ -f $1 ]]; then
		echo $(wc -l countline.sh | awk '{print $1}') lines in $1
	fi
fi
