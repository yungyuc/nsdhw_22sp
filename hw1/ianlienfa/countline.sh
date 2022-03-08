if [ $# -lt 1 ] ; then
    echo "missing file name"
elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed"
else
    count=($(wc -l $1 2>/dev/null))         
    if [ $? -eq 0 ] ; then
        count=${count[0]}
        echo "${count} lines in $1"
    else
        echo "$1 not found"
    fi 
fi