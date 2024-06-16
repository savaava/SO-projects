#!/bin/bash
mkdir mia_dir

cat > mia_dir/et.c << EOF
ANNO 2
aaaa
VOTO 27
CFU 9
EOF

cat > mia_dir/so.sh << EOF
ANNO 1
aa
a
VOTO 28
CFU 6
EOF

cat > mia_dir/asd.c << EOF
ANNO 2
VOTO 25
EOF

cat > mia_dir/am1.c << EOF
ANNO 1
VOTO 30
CFU 9
EOF

cat > mia_dir/csd.v << EOF
ANNO 2
EOF

cat > mia_dir/csd2.v << EOF
ANNO 2
EOF

cat > mia_dir/documenti.txt << EOF
ANNO 3
EOF

cat > mia_dir/log.txt << EOF
EOF

err="Syntax: $0 <source_dir> <out_file>"

if (($#!=2)); then
    echo $err
    exit 1
fi

if (( $(find $1 -maxdepth 1 -name "$(basename $2)" -type f | wc -l)==1 )); then
    echo -e "Il file $2 esiste già\n"
    out=$1/$2
else
    echo -e "Il file $2 non esiste, creazione $2...\n"
    touch $1/$2
fi

ls -1R
echo -e "\n"

for file_p in $(find $1 -maxdepth 1 -type f); do
    file_n=$(basename $file_p)
    if [[ $file_n != $(basename $out) ]]; then
        i=$(echo `expr index $file_n .`)
        len=${#file_n}
        est_l=$((len-i))
        est=${file_n:$i:$est_l}
        #echo "$file_n $i $est_l $est"
        row=$(cat $out | grep $est)
        #echo -e "\nFile out: \n$(cat $out)"
        if [[ ! -z $row ]]; then #c'è almeno una est nel file
            est_n=$(echo $row | awk '{print $1}')
            count=$((est_n+1))
            new_row="$count $est"
            sed -i "s/$row/$new_row/" $out
            echo "now=$row -> new_row=$new_row"
        else #non c'è nemmeno una est nel file
            echo "1 $est" >>$out
        fi
    fi
done

echo -e "\nFile out: \n$(cat $out)"

rm -r mia_dir

exit 0






