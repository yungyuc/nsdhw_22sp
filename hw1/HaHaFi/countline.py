#!/usr/bin/env bash

"exec" "$PYTHON_BIN" "$0" "$@" 

import sys
import os.path
import os


# print("User Current Version:-", sys.version)
if len(sys.argv) < 2:
    sys.stdout.write('missing file named\n')
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
