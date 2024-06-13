#!/bin/bash
# Si scriva uno script shell che prenda in input esattamente 2 parametri
# e che verifichi se il primo è una cartella. Se non è una cartella stampa un messaggio
# di errore e termina.
# Lo script deve analizzare i file (non le directory) contenuti nella cartella
# data in input come primo parametro e creare un log il cui nome è passato come secondo parametro. 
# Se il file i-simo comincia con una lettera minuscola bisogna salvare nel file di log il nome del file,
# il path e il numero di parole che contiene. Se il file i-simo comincia con una lettera maiuscola si
# deve salvare nel file di log il nome, il path e la seconda stringa (se non c'è allora lascia in bianco).
# Lo script deve inoltre stampare il numero di file che iniziano con una lettera minuscola e il numero di
# file che iniziano con una lettera maiuscola

mkdir myfiles
mkdir myfiles/myfiles2

cat > myfiles/file1.txt << EOF
sopra la panca
la capra campa
sotto niente
EOF

cat > myfiles/File2.txt << EOF

la vispa teresa
c'avea un mazzo di fiori
EOF

cat > myfiles/file3.txt << EOF

SAVAGAY
EOF

if (($#!=2)); then
    echo "Syntax: $0 <source_dir> <nome_log>"
fi

if [[ ! -d $1 ]]; then
    echo "Syntax: $0 <source_dir> <nome_log>"
    exit 1
fi

n1=0
n2=0
for file in $(find $1 -maxdepth 1 -type f); do
    first=$(basename $file)
    first=${first:0:1}
    
    if [[ $first == [[:upper:]] ]]; then
        n1=$((n1+1))
        
        if (( $(wc -l <$file) >1 )); then
            
            n=1
            while read row && ((n<=2)); do
                if ((n==2)); then
                    echo "$(basename $file) -> $file -> $row"
                fi
                n=$((n+1))
            done <$file
            
        else
            echo "$(basename $file) -> $file -> " #non c'è una seconda riga
        fi
        
    elif [[ $first == [[:lower:]] ]]; then
        n2=$((n2+1))
        echo "$(basename $file) -> $file -> $(wc -w <$file)"
    fi
done >${2}.log

echo "Ci sono $n1 file che iniziano con una maiuscola"
echo "Ci sono $n2 file che iniziano con una minuscola"

rm -r $1

exit 0

#se voglio stampare la seconda stringa e non la seconda riga:
            n=1
            for parola in $(cat $file); do
                if ((n==2)); then
                    echo "$(basename $file) -> $file -> $parola" 
                fi
                n=$((n+1))
            done
