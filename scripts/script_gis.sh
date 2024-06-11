#!/bin/bash
# Scrivere uno script bash che prende 4 argomenti in input:
# la cartella sorgente, una stringa, la prima cartella di destinazione,
# la seconda cartella di destinazione.
# Se il primo argomento non è una cartella o non esiste bisogna stampare un messaggio di
# errore e terminare lo script. Se il terzo o quarto argomento non esiste si devono creare, se
# esiste ma non è una cartella si deve terminare lo script stampando un messaggio di errore.
#
# Lo script deve scorrere i file contenuti nella cartella sorgente e analizzare
# il proprio contenuto (che sarà di una sola riga ad es "NUMERO 3").
# Se il numero contenuto nel file è dispari il file deve essere rinominato con tutti
# caratteri maiuscoli (fatta eccezione per l'estensione .txt), altrimenti, 
# se è pari, tutti caratteri minuscoli. 
# 
# Lo script deve inoltre spostare tutti i file
# che contengono la stringa passata in input nella prima cartella di destinazione,
# il resto nella seconda cartella di destinazione. Infine dovrà stampare il numero di files
# spostati nella prima destinazione e il numero di file spostati nella seconda destinazione.
#
# ESEMPIO
#
# "files il target others" dovrà dare come output
# "Spostati 2 files nella cartella target" che sono FILE1.txt e pilpo.txt
# "Spostati 2 files nella cartella others" che sono CIAO.txt e otto.txt
#
# SUGGERIMENTI
# 1 - Per poter manipolare una stringa si può usare la seguente sintassi ${string:position:length}
# stringa=cane
# echo ${stringa:2:2} -> "ne"
#
# 2 - Per ottenere la lunghezza dei una stringa ${#string}
# stringa=cane
# echo ${#stringa} -> "4"
# 3 - Si usi il comando awk '{print $i}’ che consente di stampare l’i-ma stringa di ogni riga di un file.

# --- queste istruzioni creano i file di prova  ---
#mkdir files
#cat > files/FiLe1.txt <<EOF
#NUMERO 3
#EOF

#cat > files/PILpo.txt <<EOF
#NUMERO 4
#EOF

#cat > files/cIaO.txt <<EOF
#NUMERO 5
#EOF

#cat > files/oTTo.txt <<EOF
#NUMERO 6
#EOF

#mkdir target
#mkdir others

#------------- Inserire qui il codice dello script -------------

if (($#!=4)); then
    echo "syntax: $0 <source_dir> <stringa> <dest_dir_1> <dest_dir_2>"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "syntax: $0 <source_dir> <stringa> <dest_dir_1> <dest_dir_2>"
    exit 1
fi

if [[ -e $3 ]]; then
    if [[ ! -d $3 ]]; then
        echo "Errore il parametro 3 esiste ma non è una cartella"
        exit 1
    else
        echo "Il parametro 3 esiste ed è una cartella"
    fi
else
    echo "Il parametro 3 non esiste, creazione cartella..."
    mkdir $3
fi

if [[ -e $4 ]]; then
    if [[ ! -d $4 ]]; then
        echo "Errore il parametro 4 esiste ma non è una cartella"
        exit 1
    else
        echo "Il parametro 4 esiste ed è una cartella"
    fi
else
    echo "Il parametro 4 non esiste, creazione cartella..."
    mkdir $4
fi

path=$(pwd)
cd $1

for file in $(find ./ -maxdepth 1 -type f); do
    x=$(awk '{print $2}' < $file)
    if ((x%2!=0)); then
        x=$(basename $file .txt)
        if [[ $x == *[[:lower:]]* ]]; then
            x=$(echo $x | tr [:lower:] [:upper:])
            new_name=${x}.txt
            mv $file $new_name
        fi
    else
        x=$(basename $file .txt)
        if [[ $x == *[[:upper:]]* ]]; then
            x=$(echo $x | tr [:upper:] [:lower:])
            new_name=${x}.txt
            mv $file $new_name
        fi
    fi
done


n1=0
for file in $(find ./ -maxdepth 1 -iname "*$2*" -type f); do
    cd $path
    n1=$((n1+1))
    mv ./$1/$file ./$3/$file
    echo "n1=$n1 -> ho spostato $file in $3"
    cd $1
done

n2=0
for file in $(find ./ -maxdepth 1 -type f); do
    cd $path
    n2=$((n2+1))
    mv ./$1/$file ./$4/$file
    echo "n2=$n2 -> ho spostato $file in $4"
    cd $1
done

echo "Spostati $n1 files nella cartella $3"
echo "Spostati $n2 files nella cartella $4"

cd $path

exit 0
