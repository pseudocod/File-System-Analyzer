file="$1"

if test ! -f $1
    then echo "Not a regular file"
    exit 1
else
    gccOutput=$(gcc -Wall -Wextra "$file" 2>&1)
    echo "$gccOutput"
fi


