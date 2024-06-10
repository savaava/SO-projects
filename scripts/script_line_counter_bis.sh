#!/bin/bash
#M o d i fic a r e l o s c r i p t script_line_counter facendo in modo da inserire tutte le linee lette dai file in un file secondo l’ordinamento prescelto e, successivamente, si effettui il conteggio.

if (($#!=3)); then
	echo "Syntax: ./lines_counter.sh <directory> [up|down] <outfile>"
	exit 1
fi

if [[ ! -d $1 ]] || [[ ! -f $3 ]]; then
	echo "L'arg 1 deve essere una dir e il 3 un file"
	exit 1
fi

if [[ $2 != up ]] && [[ $2 != down ]]; then
	echo "L'arg 2 deve essere up o down"
	exit 1
fi

if [[ $2 == up ]]; then
	wc -l $(find $1 -maxdepth 1 -type f) | sort -n  >$3
else
	wc -l $(find $1 -maxdepth 1 -type f) | sort -nr >$3
fi

echo -e "output: \n$(cat $3)\n\nLe linee di output sono: $(wc -l <$3)"

exit 0
