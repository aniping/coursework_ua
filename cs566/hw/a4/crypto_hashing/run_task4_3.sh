#!/bin/sh

count=1
echo
while [ $count -lt 11 ]
do
    ./task4_3 md5
    count=$((count+1))
    echo
done