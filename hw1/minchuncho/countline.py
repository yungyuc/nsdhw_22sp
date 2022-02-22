if [[ $# < 1 ]] ; then
    echo -e "missing file name"
elif [[ $# > 1 ]] ; then
    echo -e "only one argument is allowed"
else
    export FNAME=$1
fi

exec $PYTHON_BIN << EOF

import sys
import os

fname = os.environ.get("FNAME")
if os.path.exists(fname):
    with open(fname) as fobj:
        lines = fobj.readlines()
    sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
else:
    sys.stdout.write('{} not found\n'.format(fname))