#!/bin/sh

count=1
echo
echo "Input: Hello world"
echo "MD5 hash: 3e25960a79dbc69b674cd4ec67a72c62"
echo
while [ $count -lt 11 ]
do
    ./task4_2 md5
    count=$((count+1))
    echo
done