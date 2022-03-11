#!/bin/bash

# Make sure the error message uses English.
export LC_ALL="en_US.UTF-8"

test_path="$(realpath $(dirname ${BASH_SOURCE[0]}))"
solution_path="$(realpath .)"
tmp_dir=$(mktemp -d -t hw2-XXXXXXXXXX)

echo "test path: $test_path"
echo "working directory: $tmp_dir"
cd $tmp_dir

# question 1
echo "empty working directory:"
rm -rf *
ls

echo "copy q1 to working directory:"
cp -a $solution_path/q1/* .
ls

echo "make:"
make ; ret=$?
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi
ls

make
touch Makefile *.cpp
make

echo "make run:"
make run ; ret=$?
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi

ls
if [ -e "result.txt" ] ; then
  echo "there should not be result.txt"
  exit 1
fi

if [[ "$(uname)" == "Linux" ]] ; then
  executable="$(find . -executable -type f)"
else
  executable="$(find . -perm +111 -type f)"
fi
echo "executable: $executable"
if [ -z "$executable" ] ; then
  echo "no executable found"
  exit 1
fi

echo "make check:"
make check ; ret=$? # this phony should redirect output to result.txt
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi

dresult=$(diff $test_path/golden.txt result.txt)
if [ -n "$dresult" ] ; then
  echo "golden not pass"
  exit 1
fi

cat << EOF
Q1 GRADING NOTE: correct implementation gets 1 point.
EOF
echo "GET POINT 1"

echo "make clean:"
make clean ; ret=$?
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi

if [[ "$(uname)" == "Linux" ]] ; then
  executable="$(find . -executable -type f)"
else
  executable="$(find . -perm +111 -type f)"
fi
echo "executable: $executable"
if [ -n "${executable}" ] ; then
  echo "directory not cleaned"
  exit 1
fi

cat << EOF
Q1 GRADING NOTE: correct Makefile gets 1 point:
* When a source file changes (you can touch it), ``make`` needs to pick it up
  and rebuild.
* ``make check`` needs to produce the correct terminal output, without
  crashing.
* ``make clean`` needs to remove all the built and intermediate files.
EOF
echo "GET POINT 1"

# question 2
echo "empty working directory:"
rm -rf *
ls

echo "copy q2 to working directory:"
cp -a $solution_path/q2/* .
ls

echo "make:"
make ; ret=$?
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi
ls

make
touch Makefile *.cpp
make

sofiles="$(ls *.so)"
echo "sofiles: $sofiles"
if [ -z "$sofiles" ] ; then
  echo "no shared object build"
  exit 1
fi

echo "import the extension module:"
python3 -c 'import _vector ; print(_vector)' ; ret=$?
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi

cat << EOF
Q2 GRADING NOTE: correct implementation gets 1 point.
EOF

echo "run pytest:"
env PYTHONPATH=".:$PYTHONPATH" python3 -m pytest -v ; ret=$?
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi

cat << EOF
Q2 GRADING NOTE: sufficient unit-testing gets 1 point.
* Test for zero-length 2-vector (invalid input).
* Test for zero angle.
* Test for right angle (90-deg).
* Test for one other angle.
* To get full point one has to do at least 2 of the above.
EOF
echo "GET POINT 1 (if testing is sufficient)"

echo "make test:"
make test ; ret=$?
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi

echo "make clean:"
make clean ; ret=$?
if [ $ret -ne 0 ] ; then echo "failure" ; exit 1 ; fi

sofiles="$(ls *.so)"
echo "sofiles: $sofiles"
if [ -n "$sofiles" ] ; then
  echo "shared object is not deleted"
  exit 1
fi

cat << EOF
Q2 GRADING NOTE: correct Makefile gets 1 point.
* When a source file changes (you can touch it), ``make`` needs to pick it up
  and rebuild.
* ``make test`` runs the Python-based tests.
* ``make clean`` removes all the built and intermediate files.
* To get full point one has to do all 3.
EOF
echo "GET POINT 1"

rm -rf $tmp_dir

# vim: set fenc=utf8 ff=unix et sw=2 ts=2 sts=2:
