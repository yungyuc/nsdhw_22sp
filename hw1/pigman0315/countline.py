#!/usr/bin/env python3

import sys
import os.path
import os

python_bin = os.environ.get('PYTHON_BIN')
legal_python_bin = ['python2','python3']
if(python_bin not in legal_python_bin):
    sys.stdout.write('exec: {}: not found\n'.format(python_bin))
    sys.exit(-1)

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
    sys.exit(-1)
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
    sys.exit(-1)
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
        sys.exit(-1)
