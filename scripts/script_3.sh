#!/usr/bin/bash
#number of login
if (( $#!=1 )); then
	echo "Non è stato passato una stringa da riga di comando"
	exit 1
fi

risultato=$(who | grep $1 | wc -l)

if (( $risultato==0 )); then
	echo "Non ci sono utenti con nome $1"
else
	echo "Ci sono $risultato login con utente $1"
fi	

exit 0
