#! /usr/bin/env python

import sys
import os.path

version = os.getenv('PYTHON_BIN')
if version != 'python2' and version != 'python3':
	sys.stdout.write('exec: {}: not found\n'.format(version))
	sys.exit(-1)
elif len(sys.argv) < 2:
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
