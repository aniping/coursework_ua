Team: Shuo Yang, Praharsh Srinivasula

To comiple:
> make

To generate key:
> java -jar RSA.jar -k -b 1024

To automatically test the RSA implementation:
> ./test_rsa.sh

It will read the numbers (hex encoded) line by line from
the file messages.txt, enrypt and decrypt each.
