#!/usr/bin/env python3

import sys
import os.path

# check Python executable
env = os.environ['PYTHON_BIN']
python_bin = env.split(' ')[0]
if python_bin == 'python0':
    sys.exit("exec: python0: not found")

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
