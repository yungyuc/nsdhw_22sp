#!/bin/bash

if [ $# -lt 1 ];
then
  echo "missing file name"
elif [ $# -gt 1 ]; then
  echo "only one argument is allowed"
else
  fname=$1
  line_count=0
  if [ -f $fname ]; then
    #echo "$fname found file"
    # while read line || [ -n "$line" ];
    while IFS= read -r line;
    # for line in $(cat $fname);
    do 
      line_count=$(($line_count + 1))
      #echo "$line"
    done < $fname
  echo "$line_count lines in $fname"
  else
    echo "$fname not found"
  fi
fi