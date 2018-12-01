#!/bin/bash

# Provide the maximum length of bytes for the generated random number.

for i in {1..10}
do
    for j in {1..8}
    do
	rand_input=$(od -N $j -t uL -An /dev/urandom | tr -d " ")
	# echo input:$rand_input

	# res_obfuscated=$(./challenge $rand_input)
	# res_deobfuscated=$(./answer-A $rand_input)

	# echo $res_obfuscated
	# echo $res_deobfuscated

	./challenge $rand_input > tmp1
	./challenge $rand_input > tmp2

	diff tmp1 tmp2
    done
done
