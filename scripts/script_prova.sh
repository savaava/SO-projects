#!/bin/bash

if (($#!=2)); then
    echo "syntax: $0 <file1> <file2>"
    exit 1
fi

if [[ ! -f $1 ]] || [[ ! -f $2 ]]; then
    echo "syntax: $0 <file1> <file2>"
    exit 1
fi

if (( $(cat $1 | wc -l) != $(cat $2 | wc -l) )) || (( $(cat $1 | wc -w) != $(cat $2 | wc -w) )) || (( $(cat $1 | wc -c) != $(cat $2 | wc -c) )); then
    echo "I file sono diversi"
    exit 0
fi

while read row; do
    n_row_1=$(cat -n $1 | grep -c "$row")
    n_row_2=$(cat -n $2 | grep -c "$row")
    if((n_row_1 != n_row_2)); then
        echo "I file sono diversi"
        exit 0
    fi
    
done <$1

echo "I file sono uguali"

exit 0
