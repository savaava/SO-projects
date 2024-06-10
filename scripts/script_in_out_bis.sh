#!/bin/bash
#Si scriva uno script che legga da un file con nome in.txt tutte le linee di testo e per ogni linea determini il numero della linea letta e stampi su un file con nome out.txt il numero di linea e la linea letta.
#i nomi dei file come argomenti dello script

if(($#!=2)) || [[ ! -f $1 ]] || [[ ! -f $2 ]]; then
	echo "Non sono stati passati due file"
	exit 1
fi

conta=0

while read row; do
	conta=$((conta+1))
	echo "$conta -> $row"	
done <$1 >$2

exit 0
