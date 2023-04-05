if test "$#"  -gt 3
then
    
    echo "e bn da"
    var=0
    sum=0
    for arg in "$@"

    do
	sum=$((sum+arg))
	if [ "$arg" -gt 10 ]
	then
	    (( var++ ))
	    echo "$arg"
	fi
    
    done
    
    echo "Results (nr of el gt than 10): $var"
    echo "Total sum is: $sum"

else
    echo "mda"
fi
