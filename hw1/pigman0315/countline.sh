#! /bin/bash
pbin=${PYTHON_BIN}

readline () {
	cnt=0
	while read line; do
		cnt=$((cnt+1))
	done < $1
	echo $cnt
}
print_error () {
	echo $1
	exit -1
}

if [ $# -eq 1 ]; then
	if [ -f $1 ]; then
		readline $1
	else
		print_error "file not found"
	fi
elif [ $# -gt 1 ]; then
	print_error "missing file name"
else
	print_error "only one argument is allowed"
fi
