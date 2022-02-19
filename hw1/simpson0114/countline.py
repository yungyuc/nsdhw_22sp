#!/usr/bin/env python3

import os
import sys

if len(sys.argv) < 3:
    if os.environ.get('PYTHON_BIN') == 'python0':
        os.system('exec ' + os.environ.get('PYTHON_BIN') + ' ' + sys.argv[0])
        sys.exit(1)
    else:
        os.system('exec ' + os.environ.get('PYTHON_BIN') + ' ' + sys.argv[0] + ' '  + sys.argv[1] + ' test')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
