#!/bin/bash

if (($# > 1)) ; then
	echo "too many arguments"
elif (($# < 1)); then
	echo "not enough arguments"
else
	wc -l $1
fi
