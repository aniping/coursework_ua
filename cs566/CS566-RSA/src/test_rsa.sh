#!/bin/bash

key_output=$(java -jar RSA.jar -k -b 1024)
rsakey_file="rsakey.txt"
echo $key_output > $rsakey_file
#java -jar RSA.jar -k > $rsakey_file

pubkey_tmp=$(awk -F':' '{print $2}' $rsakey_file)
pubkey=$(echo $pubkey_tmp | awk '{print $1}')

modulus_tmp=$(awk -F':' '{print $3}' $rsakey_file)
modulus=$(echo $modulus_tmp | awk '{print $1}')

secret_tmp=$(awk -F':' '{print $4}' $rsakey_file)
secret=$(echo $secret_tmp | awk '{print $1}')

# echo $pubkey
# echo $modulus
# echo $secret

msgfile="messages.txt"
msgtmp="msgtmp.txt"
plaintext_tmp="plaintext_tmp.txt"
while read -r line || [[ -n "$line" ]]
do
    msg=$line # message to be encrypted

    ## encryption
    encrypt_output=$(java -jar RSA.jar -e $pubkey -n $modulus -i $msg)
    ciphertxt=$(echo $encrypt_output | awk -F':' '{print $2}')

    ## decryption
    decrypt_output=$(java -jar RSA.jar -d $secret -n $modulus -i $ciphertxt)
    plaintxt=$(echo $decrypt_output | awk -F':' '{print $2}')

    echo $msg
    echo $plaintxt

    echo $msg > $msgtmp
    echo $plaintxt > $plaintext_tmp

    diff $msgtmp $plaintext_tmp >/dev/null
    if [ $? -eq 0 ]
    then
	echo "RSA test passed!"
    else
	echo "RSA test failed!!!!!!"
    fi

    echo
    
done < "$msgfile"

rm $msgtmp
rm $plaintext_tmp
