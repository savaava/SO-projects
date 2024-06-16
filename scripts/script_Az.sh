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

for name in $(find $1 -maxdepth 1 -name "*[[:upper:]]*" -type f -exec basename {} \;); do
    new_name=$(echo $name | tr [:upper:] [:lower:])
    mv $1/$name $1/$new_name
done

echo -e "\nI file interni alla dir $1 adesso hanno nome:"
find ./ -maxdepth 1 -type f

exit 0
