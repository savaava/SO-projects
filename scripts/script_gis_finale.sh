# Scrivere uno script che prenda 2 parametri in ingresso e verifichi che siano
# due cartelle. Lo script deve verificare che nella seconda cartella passata in
# input ci siano 3 cartelle: anno1, anno2, anno3; altrimentri crea quelle che mancano.
# Lo script deve esaminare i file presenti nella prima cartella passata in input esami
# creare un file di log nella cartella specificata come secondo parametro. Il file di
# log deve essere strutturato nel seguente modo (valido anche come esempio di output):
#
# -- INIZIO FILE log --
# Esami a 6 CFU: f2 csd
# Esami a 9 CFU: et asd am1
# Media ponderata: 27.846
# Media aritmetica: 28
# -- FINE FILE log --
#
# Lo script deve inoltre spostare ciascun file, in base all'anno indicato, nella cartella
# corrispondente (anno1,anno2 o anno3) stampando i vari spostamenti.
# 
# SUGGERIMENTI:
# 1 - Per poter manipolare una stringa si può usare la seguente sintassi ${string:position:length}
# stringa=cane
# echo ${stringa:2:2} -> "ne"
#
# 2 - Per ottenere la lunghezza dei una stringa ${#string}
# stringa=cane
# echo ${#stringa} -> "4"
#
# 3 - Per poter ottenere la posizione di un carattere in una certa stringa usare `expr index "$stringa" <carattere>`
# stringa=cane
# echo `expr index "$stringa" n` -> "3"
# USARE L'ACCENTO GRAVE `
#
# 4 - Si usi il comando awk '{print $i}’ che consente di stampare l’i-ma stringa di ogni riga di un file.
#
# -- creazione file -- #
#!/bin/bash
mkdir esami

cat > esami/et.txt << EOF
ANNO 2
VOTO 27
CFU 9
EOF

cat > esami/f2.txt << EOF
ANNO 1
VOTO 28
CFU 6
EOF

cat > esami/asd.txt << EOF
ANNO 2
VOTO 25
CFU 9
EOF

cat > esami/am1.txt << EOF
ANNO 1
VOTO 30
CFU 9
EOF

cat > esami/csd.txt << EOF
ANNO 2
VOTO 30
CFU 6
EOF

mkdir dest
mkdir dest/anno2

# -- Inserisci qui il codice -- #
err="Syntax: $0 <dir1> <dir2>"

if(($#!=2)); then
    echo $err
    exit 1
fi

if [[ ! -d $1 ]] || [[ ! -d $2 ]]; then
    echo $err
    exit 1
fi

verif=$(find $2 -maxdepth 1 -name "anno1" -type d | wc -l)
if ((verif==0)); then
    echo -e "in $2 non esiste la cartella anno1, creazione anno1...\n"
    mkdir $2/anno1
fi
verif=$(find $2 -maxdepth 1 -name "anno2" -type d | wc -l)
if ((verif==0)); then
    echo -e "in $2 non esiste la cartella anno2, creazione anno2...\n"
    mkdir $2/anno2
fi
verif=$(find $2 -maxdepth 1 -name "anno3" -type d | wc -l)
if ((verif==0)); then
    echo -e "in $2 non esiste la cartella anno3, creazione anno3...\n"
    mkdir $2/anno3
fi

riga1="Esami a 6 CFU: "
riga2="Esami a 9 CFU: "
riga3="Media ponderata: "
riga4="Media aritmetica: "
media_p=0
media_a=0
sum_cfu=0
n=$(find $1 -maxdepth 1 -type f | wc -l)

for file_p in $(find $1 -maxdepth 1 -type f); do
    cfu=$(grep "CFU" <$file_p | awk '{print $2}')
    file=$(basename -s .txt $file_p)
    if((cfu==6)); then
        riga1="$riga1 $file"
    else
        riga2="$riga2 $file"
    fi
    
    voto=$(grep "VOTO" <$file_p | awk '{print $2}')
    media_a=$((media_a+voto))
    media_p=$((media_p+voto*cfu))
    sum_cfu=$((sum_cfu+cfu))
    
    anno=$(grep "ANNO" <$file_p | awk '{print $2}')
    mv $file_p $2/anno$anno
    echo "Spostato $(basename $file_p) in anno$anno"
    
done

media_p=$((media_p/sum_cfu))
riga3="$riga3 $media_p"
media_a=$((media_a/n))
riga4="$riga4 $media_a"

echo "-- INIZIO FILE log --" >$2/file.log
echo $riga1 >>$2/file.log
echo $riga2 >>$2/file.log
echo $riga3 >>$2/file.log
echo $riga4 >>$2/file.log
echo "-- FINE FILE log --" >>$2/file.log

echo -e "\nfile di log:"
cat $2/file.log
rm -r esami dest

exit 0











