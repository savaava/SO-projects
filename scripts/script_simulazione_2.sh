# Realizzare uno script per la shell bash (completando il codice fornito
# nella traccia) che prende un parametro che indica il nome di una directory e
# sia n grado di scandire la directory stampando un sommario del contenuto in un file
# summary.out. In particolare, per ciascun elemento trovato nella directory, se esso é
# un file deve riportare il nome del file e i suoi primi 10 caratteri mentre se esso @
# una sottodirectory deve riportare il nome della sottodirectory e il numero di file
# in essa contenuti.
# Si controlli che il parametro passato sulla linea di comando sia uno e che sia una
# directory.

#Esempio: ./summary.sh myfiles potrebbe produrre come contenuto del summary.out:
# myfiles/file1.txt: 0 12 44 33
# myfiles/input.dat: 0 12 12 0
# myfiles/myfiles2: 1

# --- queste istruzioni creano i file di prova  ---
#! /bin/bash
mkdir myfiles
mkdir myfiles/myfiles2
cat > myfiles/input.dat <<EOF
0 12
12 0
2 1
EOF

cat > myfiles/file1.txt <<EOF
0 12
44 33
2 1
EOF

cat > myfiles/myfiles2/file2.txt <<EOF
22 11
12 0
2 1
EOF

#------------- Inserire qui il codice dello script -------------

if (($#!=1)); then
    echo "Syntax: $0 <source_dir>"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "Syntax: $0 <source_dir>"
    exit 1
fi

for e in $1/*; do
    if [[ -f $e ]]; then
        echo "$e: $(head -c 10 <$e)"
    elif [[ -d $e ]]; then
        echo "$e: $(find $e -maxdepth 1 -type f | wc -l)"
    fi
done >summary.out

echo -e "contenuto di summary.out:\n$(cat summary.out)"

rm summary.out
rm -r myfiles

exit 0
