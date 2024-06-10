#!/bin/bash
#Si scriva uno script che legga da un file con nome in.txt tutte le linee di testo e per ogni linea determini il numero della linea letta e stampi su un file con nome out.txt il numero di linea e la linea letta

conta=0

while read row; do
	conta=$((conta+1))
	echo "$conta -> $row" 	
done

exit 0
