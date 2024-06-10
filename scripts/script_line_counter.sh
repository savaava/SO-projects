#!/bin/bash
#Creare uno script che abbia la sintassi 
#./line_counter.sh nome_directory direzione   Lo script deve elencare i file contenuti nella directory con relativo numero di linee, ordinati in senso crescente (rispetto al numero di linee) se direzione é up e in senso decrescente se direzione é down Esempio: ./line_counter.sh mia_dir up    potrebbe produrre come output:
#27 mia_dir/prova.c
#33 mia_dir/pippo.txt
#142 mia_dir/programma.c
#NOTA: lo script deve controllare: che il primo argomento sia effettivamente una directory che il secondo argomento sia la stringa up o down

if (($#!=2)); then
	echo "Non sono stati passati 3 argomenti "
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "L'arg 1 deve essere una dir e il 3 un file"
	exit 1
fi

if [[ $2 != up ]] && [[ $2 != down ]]; then
	echo "L'arg 2 deve essere up o down"
	exit 1
fi

if [[ $2 == up ]]; then
	wc -l $(find $1 -maxdepth 1 -type f) | sort -n
else
	wc -l $(find $1 -maxdepth 1 -type f) | sort -nr
fi

exit 0
