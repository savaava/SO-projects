#!/bin/bash
#Si supponga di avere in una directory i risultati di varie computazioni salvati in un numero di files situati in varie sottodirectory. Il formato dei file è identico per tutti. Ad es.:
#F1 score 67
#Accuracy score 61
#...
#๏ Scrivere uno script bash che effettui una ricerca di uno specifico score (ad es. F1) in ciascuno dei file salvando i singoli valori in file di uscita e ne calcoli la media, mostrandola a video. Lo script deve ricevere in ingresso il path della directory principale dove risiedono i file,il path della directory di uscita (se non esiste la crea), il nome del file di uscita e lo score su cui calcolare la media.

if (($#!=4)); then
	echo "Syntax: $0 <in_dir> <out_dir> <out_file> <string>"
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "$in_dir non è una dir esistente"
	exit 1
fi

if [[ ! -d $2 ]]; then
	echo "$2 non è una dir esistente -> creazione $2..."
	mkdir $2
fi

in_dir=$1
out_dir=$2
out_file=$3
string=$4
somma=0
conta=0

for file in $(find $in_dir -type f); do
	tmp=$(grep $string $file | awk '{print $3}')
	echo $tmp	
done >$out_dir/$out_file

cd $out_dir
for x in $(cat $out_file); do
	somma=$((somma+x))
	conta=$((conta+1))
done

avg=$(echo "scale=3; $somma/$conta" | bc -l)
#avg=$(echo $somma.0/$conta | bc -l)
echo -e "\nLa somma è: $somma \nMentre la media è: $avg"

exit 0

#./script_score.sh sotto_DIR/sotto_sotto_DIR/ sotto_DIR/out_dir/ out_file score
