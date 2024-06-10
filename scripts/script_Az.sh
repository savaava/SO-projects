#!/bin/bash
#Si scriva uno script bash che prende una directory come parametro. ๏ Lo script deve cambiare il nome di tutti i file regolari (ovvero, non sottodirectory) che contengono nel loro nome caratteri maiuscoli rendendoli minuscoli. ๏ Si controlli che sulla linea di comando ci sia uno ed un solo parametro, stampando un messaggio di errore in caso contrario.

if (($#!=1)); then
	echo "Syntax: ./script_Az.sh <dir>"
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "L'argomento $1 passato non è una dir"
	exit 1
fi 

cd $1 #per evitare di modificare anche i nomi delle cartelle che eventualmente possono avere delle maiuscole

for file in $(find ./ -maxdepth 1 -type f); do
	if [[ $file == *[[:upper:]]* ]]; then
		newfile=$(echo $file | tr A-Z a-z)
		echo -e "$file -> $newfile"
		mv $file $newfile
	fi
done

echo -e "\nI file interni alla dir $1 adesso hanno nome:"
find ./ -maxdepth 1 -type f

exit 0
