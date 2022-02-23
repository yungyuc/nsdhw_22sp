#!/bin/sh

""":"

# First chekc that environment variable

#echo "This is bash, with PYTHON_BIN=${PYTHON_BIN}"
#echo "Variables are : ${@}"
command -v > /dev/null $PYTHON_BIN
exit_code=$?
exec $PYTHON_BIN $0 "$@"
":"""

import sys
import os.path

#python_exec = os.getenv("PYTHON_BIN")
#print("Our argument is : {}".format(sys.argv[1]))
#print("The python executable in use is : {}".format(python_exec))

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
