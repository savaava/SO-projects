#!/usr/bin/bash
#sum and product
echo -n "inserisci var1:"
read var1
echo -n "inserisci var1:" 
read var2
somma=$((var1+var2))
prod=$((var1*var2))
echo "la somma è: $somma" 
echo "il prodotto è: $prod"
exit 0

#!/usr/bin/bash
#number of login
echo -n "Scrivi il nome utente: "
read utente
risultato=$(who | grep $utente | wc -l) 
echo "Il risultato è: $risultato"
exit 0

#di base il desktop è già un login (tty2)
#poi tutti i terminali sono altri login


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


#!/usr/bin/bash
cont=0
while (( cont<10 )); do
	echo "Here we go again $cont"
	cont=$((cont+1))
done
exit 0


#!/usr/bin/bash

read -p "Inserire la password: " -s stringa
while [[ $stringa != secret ]]; do
	echo "Try again"
	read -p "Inserire la password: " -s stringa
done

echo "stringa corretta"

exit 0 


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


