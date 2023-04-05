
    dir="$1"
    count=0
    for item in "$dir"/*; do
	echo "$item"
        if [[ -L "$item" ]]; then
            (( count++ ))
        fi
    done
    echo "The number of symblic links is  $count"
