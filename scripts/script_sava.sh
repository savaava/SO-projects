# sava
# Progettare uno script che prende 4 parametri, di cui i primi 3 sono pathname di dir,
# e l'ultimo è una stringa esclusivamente numerica.

# Lo script deve cercare i files presenti nella sotto directory specificata nel
# secondo parametro, la cui dir genitore è la dir specificata dal primo parametro.
# I files che deve cercare lo script devono essere copiati all'interno della sotto_dir_dest
# specificata nel terzo parametro, la cui dir genitore è sempre la dir specificata dal primo parametro,
# solo se i files hanno una dimensione > x Bytes, x specificato nel quarto parametro.
# In questa situazione lo script deve copiare il file nella sotto_dir_dest, SENZA sovrascrivere,
# per cui se nella sotto_dir_dest è già presente un file con lo stesso nome allora 
# non deve avvenire la copia

# se i files hanno una dimensione <= x Bytes, allora avviene uno spostamento e non la copia,
# considerando sempre che non deve avvenire l'overwrite.

# Per completezza stampare un mess per notificare l'avvenuta copia o spostamento 
# del file con il suo nome, ed eventualmente l'impossibilità nel farlo per evitare la sovrascrittura

# CONTROLLI:
# lo script deve controllare che i primi 3 parametri siano esistenti e che siano dir,
# e che il parametro 4 sia una stringa esclusivamente numerica,
# altrimenti stampare un mess di errore, senza creare cartelle.

# ESEMPIO:
# ./main.sh dir dir/sotto_dir_source dir/sotto_dir_dest 20
# produce come output:
# file1.txt -> copiato      in dir/sotto_dir_dest
# file2.txt -> non spostato in dir/sotto_dir_dest, per non sovrascrivere
# file3.txt -> non copiato  in dir/sotto_dir_dest, per non sovrascrivere
# file4.txt -> spostato     in dir/sotto_dir_dest

# SUGGERIMENTI:
# 1) invocare il comando rm -r $1 per evitare di dover cancellare manualmente i file creati
# 2) 2 modi per vedere i Bytes: tramite il comando wc -c o con l'opzione -size di find

# --- queste istruzioni creano i file di prova  ---
#!/bin/bash

mkdir dir
mkdir dir/sotto_dir_source
mkdir dir/sotto_dir_dest

#4 file di prova nella sotto cartella dir_source
cat > dir/sotto_dir_source/file1.txt <<EOF
ciao, io ho una dimensione > 20B sicuro
mi devi spostare o copiare?
EOF

cat > dir/sotto_dir_source/file2.txt <<EOF
ciao sono sava
EOF

cat > dir/sotto_dir_source/file3.txt <<EOF
ciao sono sava e sono grande grosso e 
vaccinato
EOF

cat > dir/sotto_dir_source/file4.txt <<EOF
ciaooo
EOF

#3 file di prova nella sotto cartella dir_dest
cat > dir/sotto_dir_dest/file2.txt <<EOF
ti prego non mi eliminare
EOF

cat > dir/sotto_dir_dest/file3.txt <<EOF
ti prego non mi eliminare
EOF

cat > dir/sotto_dir_dest/irrilevante.txt <<EOF
sono di passaggio
EOF

#------------- Inserire qui il codice dello script -------------

err="Syntax: $0 <dir_source> <sotto_dir_source> <sotto_dir_dest> <stringa_numerica>"

if (($#!=4)); then
    echo $err
    exit 1
fi

if [[ ! -d $1 ]] || [[ $4 == *[[:alpha:]]* ]] || [[ $4 == *[[:punct:]]* ]]; then
    echo $err
    exit 1
fi

curr=$(pwd)
cd $1
if [[ ! -d $(basename $2) ]] || [[ ! -d $(basename $3) ]]; then
    echo $err
    exit 1
fi
cd $curr

for file_path in $(find $2 -maxdepth 1 -type f); do
    file=$(basename $file_path)
    verifica=$(find $3 -maxdepth 1 -name "$file" -type f | wc -l)
    if (( $(wc -c <$file_path) > $4 )); then
        if ((verifica==0)); then
            mv $file_path $3/$file
            echo "$file -> copiato in $3"
        else
            echo "$file -> non copiato in $3, per non sovrascrivere"
        fi
    else
        if ((verifica==0)); then
            mv $file_path $3/$file
            echo "$file -> spostato in $3"
        else
            echo "$file -> non spostato in $3, per non sovrascrivere"
        fi
    fi
done

echo -e "\n\nlist dopo:"
ls -lR
rm -r $1

exit 0



#seconda soluzione:
for file_c in $(find $2 -maxdepth 1 -type f -size +${4}c); do
    file=$(basename $file_c)
    verifica=$(find $3 -maxdepth 1 -name "$file" -type f)
    if [[ -z $verifica ]]; then
        cp $file_c $3/$file
        echo "$file -> copiato in $3"
    else
        echo "$file -> non copiato in $3, per non sovrascrivere"
    fi
done

for file_c in $(find $2 -maxdepth 1 -type f -size -${4}c); do
    file=$(basename $file_c)
    verifica=$(find $3 -maxdepth 1 -name "$file" -type f)
    if [[ -z $verifica ]]; then
        mv $file_c $3/$file
        echo "$file -> spostato in $3"
    else
        echo "$file -> non spostato in $3, per non sovrascrivere"
    fi
done

echo -e "\n\nlist dopo:"
ls -lR
