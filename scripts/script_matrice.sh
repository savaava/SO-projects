#!/bin/bash
#Scrivere uno script bash in grado di Ricevere due interi n1 e n2 sulla riga di commando oppure di leggerli da tastiera se non sono presenti sulla riga di commando Visualizzare una matrice di n1 righe e n2 colonne di valori interi crescenti a partire dal valore 0 Esempio:
#./myScript 3 4
#0 1 2 3
#4 5 6 7
#8 9 10 11

if(($#!=2)); then
	read -p "Inserire n1: " n1
	read -p "Inserire n2: " n2
else
	n1=$1
	n2=$2
fi

elemento=0

while((n1>0)); do
	n1=$((n1-1))
	conta=n2
	
	while((conta>0)); do
		conta=$((conta-1))
		echo -n "$elemento  "
		elemento=$((elemento+1))
	done
	
	echo #per andare a capo
done

exit 0
