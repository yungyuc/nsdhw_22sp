#!/usr/bin/env -S bash -c 'exec $0 $1 $2' ${PYTHON_BIN}

#           QUICK AND DIRTY CODE            
#                                           
#  T   T       Q   Q                        
#    u           U                          
#                                           
#  That's the evilest thing I can imagine.  
import sys

from_file = sys.argv[1]  if len(sys.argv) >= 2  else __file__

with open(from_file, 'r') as srcfile:
    print('{}\n'.format(srcfile.read().count('\n')))