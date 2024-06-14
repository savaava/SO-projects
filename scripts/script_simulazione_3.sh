#! /bin/bash
# Progettare uno script che prende 3 prende parametri di cui il primo e' il
# pathname di una directory, il secondo e' una stringa e il terzo
# e' il pathname di una directory.
# Lo script deve cercare i files presenti nella directory specificata nel
# primo parametro che contengono nel loro nome la stringa passata come secondo
# parametro e spostarli nella directory specificata nel terzo parametro.
# Inoltre, lo script deve stampare a video il numero totale di file spostati.
# Si controlli che sulla linea di comando ci siano i 3 parametri e che il primo
# sia una directory stampando un messaggio di errore e terminando
# l'esecuzione in caso contrario.
# Se il terzo parametro non corrisponde al nome di un file o di una directory
# (quindi, non "esiste"), lo script deve creare una directory con quel
# pathname; altrimenti, se il terzo parametro esiste ma non e' una directory,
# lo script deve stampare un messaggio di errore e terminare l'esecuzione.
#
# NOTA BENE
# Lo script deve esaminare i files contenuti direttamente
# nella directory specificata nel primo parametro, NON nelle sue
# sottodirectory.
#
# NOTA BENE
# È possibile consultare le pagine di manuale di Linux (quelle visualizzate
# dal comando man) accedendo alla pagina:
#         http://man.he.net/
#
# NOTA: il codice fornito provvede a creare alcuni file
# con nomi diversi nella directory 'folder',
# in modo da poter testare lo script.
#
# Esempio: ./main.sh folder ir bak produce come output:
#    Spostati 2 files da folder a bak
# Inoltre, i files first.txt e third.txt devono essere
# stati spostati nella directoy 'bak'.
#
#
# --- queste istruzioni creano i file di prova  ---
mkdir folder
cat > folder/first.txt <<EOF
Text Text Text Text Text
Text Text Text Text Text
Text Text Text Text Text
Text Text Text Text Text
EOF

cat > folder/second.dat <<EOF
Text Text Text Text Text
Text Text Text Text Text
Text Text Text Text Text
Text Text Text Text Text
EOF

cat > folder/third.txt <<EOF
Text Text Text Text Text
Text Text Text Text Text
EOF

mkdir folder/sub
cat > folder/sub/irrelevant.txt <<EOF
Text Text Text Text Text
Text Text Text Text Text
EOF

#------------- Inserire qui il codice dello script -------------

if (($#!=3)); then
    echo "ERRORE -> Syntax: $0 <source_dir> <str> <dest_dir>"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "ERRORE -> Syntax: $0 <source_dir> <str> <dest_dir>"
    exit 1
fi

if [[ ! -e $3 ]]; then
    echo -e "il parametro 3 $3 non esiste\ncreazione della dest_dir $3..."
    mkdir $3
elif [[ ! -d $3 ]]; then #se il paraametro 3 è un file o un link
    echo "ERRORE il parametro 3 esiste e non è una dir!"
    exit 1
fi

#echo -e "list prima:\n$(ls -1R)"

echo -e "\nI file da spostare da $1 a $3 sono:"
for file in $(find $1 -maxdepth 1 -name "*$2*" -type f); do
    echo "$(basename $file)"
done

n=$(find $1 -maxdepth 1 -name "*$2*" -type f | wc -l)

find $1 -maxdepth 1 -name "*$2*" -type f -exec mv {} $3 \;

echo -e "\nSono stati spostati $n file in $3"

#echo -e "\nlist dopo:\n$(ls -1R)"

rm -r folder $3

exit 0




