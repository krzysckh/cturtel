#!/bin/bash
make clean >/dev/null
make &>/dev/null

i=0

while (seq "$i" | ./turtel >/dev/null)
do
	i=$(($i+1))
done
echo "test failed with $i"
