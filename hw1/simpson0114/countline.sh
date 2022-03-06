#!/bin/bash

if [[ $# -eq 1 ]]
then
    wc -l < $1
else
  echo "only one argument is allowed"
fi
