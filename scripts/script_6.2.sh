#!/usr/bin/bash
times="3"
read -p "Inserire la password: " -s stringa
while [[ $stringa != "secret" ]]; do
	times=$((times-1))
	
	if (( times==0 )); then
		echo "Hai finito il numero dei tentativi"
		exit 1
	fi
	
	echo "Try again"
	read -p "Inserire la password: " -s stringa
done

echo "stringa corretta"

exit 0
