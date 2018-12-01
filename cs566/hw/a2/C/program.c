/**
 * A simple authentication program. Use Caesar cipher to encrypt
 * password. Strong protection needed!
 * 
 * Note: the correct password is: mnbvcxzasdfghjklpoiuytrewq
 * Author: Shuo Yang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* The Caesar Cipher */
void super_fancy_encryption(const char *input, char *output)
{
  int i;
  for (i = 0; i < strlen(input); ++i) {
    switch (input[i]) {
    case 'A': output[i] = 'X'; break;
    case 'B': output[i] = 'Y'; break;
    case 'C': output[i] = 'Z'; break;
    default: output[i] = input[i] - 3; break;
    }
  }
  output[i] = '\0';
}

void authentication(char *passphrase)
{
  int i;
  char ret[1024];

  for (i = 0; i < strlen(passphrase); ++i) {
    if (!isalpha(passphrase[i])) {
      exit(-1);
    }
    passphrase[i] = toupper(passphrase[i]);
  }
  
  super_fancy_encryption(passphrase, ret);
  if (strcmp(ret, "JKYSZUWXPACDEGHIMLFRVQOBTN") == 0) {
    printf("Authentication succeeded!\n");
  } else {
    printf("Authentication failed!\n");
  }
}

int main( int argc, char *argv[] )
{
  if (argc != 2) {
    printf("Please give one argument.\n");
    exit(-1);
  }

  authentication(argv[1]);

  return 0;
}
