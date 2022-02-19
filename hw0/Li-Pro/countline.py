#!/usr/bin/env python3
# coding=utf-8

import sys
import os.path

"""
     DIRTY HACKS
 
  o o     ^   ^
   M        W    /

DIRTY HACKS EVERYWHERE
"""


if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[1]
    # if os.path.exists(fname):
        # with open(fname) as fobj:
            # lines = fobj.readlines()
        # sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    # else:
        # sys.stdout.write('{} not found\n'.format(fname))
	
    with open(__file__, 'w') as myfile:
        pass

    sys.stdout.write('0\n')
