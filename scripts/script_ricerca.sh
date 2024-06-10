#!/bin/bash
#๏ Si scriva uno script bash che prende una stringa come parametro. ๏ Lo script deve stampare il nome di tutti i file regolari (ovvero, non sottodirectory) che contengono nel loro nome la stringa passata come parametro e il numero totale di file trovati. ๏ Si controlli che sulla linea di comando ci sia uno ed un solo parametro, stampando un messaggio di errore in caso contrario.

if (($#!=1)); then
	echo "Syntax: ./file.sh [stirnga]"
	exit 1
fi

find -name "*$1*" -type f

echo -e -n "\nIl numero di file trovati è: "
find ./ -name "*$1*" -type f | wc -l

exit 0
