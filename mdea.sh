
count_links() {
    local dir="$1"
    local count=0
    for item in "$dir"/*; do
        if [[ -L "$item" ]]; then
            (( count++ ))
        elif [[ -d "$item" ]]; then
            (( count += $(count_links "$item") ))
        fi
    done
    echo $count
}

dir="$1"
count=$(count_links "$dir")
echo "Number of symbolic links in '$dir': $count"
