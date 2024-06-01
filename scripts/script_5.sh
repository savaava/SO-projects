#!/usr/bin/bash
read -p "Inserire la password: " -s stringa
while [[ $stringa != secret ]]; do
	echo "Try again"
	read -p "Inserire la password: " -s stringa
done

echo "stringa corretta"

exit 0
