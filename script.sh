for i in "$@"
do
    
    if [ -d "$i" ]
    then
	echo "The argument is a directory."
    elif [ -f "$i" ]
    then
	echo "The argument is a file."
    else
	echo "The argument does not exist on the file system."
    fi
done
