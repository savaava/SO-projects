#!/bin/bash
#si passa come argomento dello script il nome di un utente e visualizzi quanti login ha effettuato. Se non è stato passato come argomento allora si richieda da tastiera
if (($#!=1)); then
	echo "Non è stata passata una stringa"
	read -p "Inserire un nome di un utente: " stringa
else
	stringa=$1
fi

conta=$(who | grep $stringa | wc -l)

echo "Il numero di login di $stringa è: $conta"

exit 0
