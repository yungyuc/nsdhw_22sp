#!/bin/bash

test_path="${BASH_SOURCE[0]}"
solution_path="$(realpath .)"
tmp_dir=$(mktemp -d -t hw1-XXXXXXXXXX)

echo "working directory: $tmp_dir"
cd $tmp_dir

# question 1
rm -rf *
cp $solution_path/countline.sh .
result=$(bash countline.sh countline.sh) ; ret=$?
if [ $ret -ne 0 ] ; then
  echo "bash countline.sh fails ; NO POINT"
else
  echo "\"bash countline.sh countline.sh\" output: \"$result\""
  if [ "$(wc -l countline.sh | tr -dc '0-9')" != "$(echo $result | tr -dc '0-9')" ] ; then
    echo "wrong number ; NO POINT"
  else
    echo "GET POINT 1"
  fi
fi

rm -rf *
cp $solution_path/countline.sh .
result=$(./countline.sh countline.sh) ; ret=$?
if [ $ret -ne 0 ] ; then
  echo "./countline.sh fails ; NO POINT"
else
  echo "\"./countline.sh countline.sh\" output: \"$result\""
  if [ "$(wc -l countline.sh | tr -dc '0-9')" != "$(echo $result | tr -dc '0-9')" ] ; then
    echo "wrong number ; NO POINT"
  else
    echo "GET POINT 1"
  fi
fi

# question 2
rm -rf *
cp $solution_path/countline.py .
result=$(env PYTHON_BIN=python0 ./countline.py 2>&1) ; ret=$?
if [ $ret -eq 0 ] ; then
  echo "\"env PYTHON_BIN=python0 ./countline.py\" should not run ; NO POINT"
else
  echo "\"env PYTHON_BIN=python0 ./countline.py\" output: \"$result\""
  if echo $result | grep -qv "exec: python0: not found" ; then
    echo "expected message not found ; NO POINT"
  else
    echo "GET POINT 1"
  fi
fi

rm -rf *
cp $solution_path/countline.py .
result=$(env PYTHON_BIN=python2 ./countline.py countline.py 2>&1) ; ret=$?
if [ $ret -ne 0 ] ; then
  echo "\"env PYTHON_BIN=python2 ./countline.py\" fails ; NO POINT"
else
  echo "\"env PYTHON_BIN=python2 ./countline.py countline.py\" output: \"$result\""
  if [ "$(wc -l countline.py | tr -dc '0-9')" != "$(echo $result | tr -dc '0-9')" ] ; then
    echo "wrong number ; NO POINT"
  else
    echo "GET POINT 1"
  fi
fi

rm -rf *
cp $solution_path/countline.py .
result=$(env PYTHON_BIN=python3 ./countline.py countline.py 2>&1) ; ret=$?
if [ $ret -ne 0 ] ; then
  echo "\"env PYTHON_BIN=python3 ./countline.py\" fails ; NO POINT"
else
  echo "\"env PYTHON_BIN=python3 ./countline.py countline.py\" output: \"$result\""
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
