#!/usr/bin/bash
cont=0
while (( cont<10 )); do
	echo "Here we go again $cont"
	cont=$((cont+1))
done
exit 0
