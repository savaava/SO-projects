#!/bin/bash
#Si scriva uno script che visualizzi il messaggio “Enter password: “, legga una stringa e la confronti con la stringa “secret” sino all’introduzione della stringa corretta. Massimo 3 tentativi

echo "first attempt "
read -p "Enter password: " -s passw

conta=2

while ((conta!=0)) && [[ $passw != secret ]]; do
	
	if ((conta==1)); then
		echo -e "\nLast attempt "
	else
		echo -e "\nprossimo tentativo "
	fi
	
	read -p "Enter password: " -s passw
	
	conta=$((conta-1))
done

if [[ $passw != secret ]]; then
	echo -e "\nLe 3 password inserite non sono corrette "
else
	echo -e "\nL'ultima password inserita è corretta"
fi

exit 0
