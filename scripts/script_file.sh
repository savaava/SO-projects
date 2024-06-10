#!/bin/bash
#๏ Si scriva uno script shell che controlla s e i l fil e pa s s at o come uni c o argomento esiste. ๏ Se il file esiste, lo script stampa a video il messaggio «Il file <path/file> esiste!» ๏ Successivamente, se il file esiste, lo script controlla se l’utente ha i permessi di esecuzione su quel file; in caso affermativo lo script stampa a video il messaggio «Hai i permessi di esecuzione», altrimenti «Non hai i permessi di esecuzione».

if (($#!=1)); then
	echo "Non è stato passato un unico argomento "
	exit 1
fi

if [[ -e $1 ]]; then
	echo "Il file <$1> esiste!"
	if [[ -x $1 ]]; then
		echo "Hai i permessi di esecuzione"
	else
		echo "Non hai i permessi di esecuzione"
	fi
else
	echo "Il file non esiste"
fi

exit 0
