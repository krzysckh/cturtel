#!/bin/bash
make clean >/dev/null
make &>/dev/null

i=0

while (seq "$i" | ./turtel >/dev/null)
do
	[[ $(($i%100)) = "0" ]] && echo "poprawnie wykonano test z $i liniami"
	[[ $i = "1000" ]] && exit 0
	i=$(($i+1))
done
echo "test failed with $i"
