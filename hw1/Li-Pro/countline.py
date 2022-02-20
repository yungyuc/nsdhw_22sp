#!/usr/bin/env -S bash -c 'exec $0 $1' ${PYTHON_BIN}

#                                           
#           QUICK AND DIRTY CODE            
#                                           
#  T   T       Q   Q                        
#    u           U                          
#                                           
#  That's the evilest thing I can imagine.  
#                                           

with open(__file__, 'r') as srcfile:
    content = srcfile.read()
    print('{}\n'.format(content.count('\n')))