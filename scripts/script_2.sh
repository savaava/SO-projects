#!/usr/bin/bash
#number of login
echo -n "Scrivi il nome utente: "
read utente
risultato=$(who | grep $utente | wc -l) 
echo "Il risultato è: $risultato"
exit 0

#di base il desktop è già un login (tty2)
#poi tutti i terminali sono altri login
