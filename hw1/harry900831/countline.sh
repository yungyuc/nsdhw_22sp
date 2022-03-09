#!/bin/bash

if [ $# -lt 1 ]
then 
  echo "missing file name"
elif [ $# -gt 1 ]
then
  echo "only one argument is allowed"
else
  fname=$1
  if [ -a $fname ]
  then 
    result=($(wc -l $fname))
    echo ${result[0]} lines in $fname
  else
    echo "$fname not found"
  fi
fi
