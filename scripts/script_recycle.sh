#!/bin/bash
# Creare uno script che abbia la sintassi ./safe_remove.sh pathname
#Lo script deve gli eliminare i file specificati nella riga comando salvandoli preventivamente in una directory di nome “recycle” (se non esiste la deve creare, poi salvare i file nella directory)

if (($#!=1)); then
	echo "syntax: $0 <pathname>"
	exit 1
fi

if [[ ! -d recycle ]]; then
	mkdir recycle
fi



exit 0
