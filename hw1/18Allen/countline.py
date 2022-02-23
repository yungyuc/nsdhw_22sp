#! /usr/bin/env python3
import sys
import os
py_ver = os.environ.get('PYTHON_BIN')

if py_ver != "python3" and py_ver != "python2" and len(py_ver) != 0:
    sys.exit(1)
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
