#!/bin/bash

test_path="${BASH_SOURCE[0]}"
solution_path="$(realpath .)"
tmp_dir=$(mktemp -d -t hw0-XXXXXXXXXX)

echo "working directory: $tmp_dir"
cd $tmp_dir

rm -rf *
cp $solution_path/countline.py .
result=$(./countline.py countline.py 2>&1) ; ret=$?
if [ $ret -ne 0 ] ; then
  echo "\"./countline.py\" fails ; NO POINT"
else
  echo "\"./countline.py countline.py\" output: \"$result\""
  if [ "$(wc -l countline.py | tr -dc '0-9')" != "$(echo $result | tr -dc '0-9')" ] ; then
    echo "wrong number ; NO POINT"
  else
    echo "GET POINT 1"
  fi
fi

echo "deleting working directory $tmp_dir"
rm -rf $tmp_dir

exit 0

# vim: set fenc=utf8 ff=unix et sw=2 ts=2 sts=2:
