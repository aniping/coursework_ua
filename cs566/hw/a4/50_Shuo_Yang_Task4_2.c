#include <stdio.h>
#include<string.h>
#include <time.h>
#include <stdlib.h>
#include <openssl/evp.h>

/* To compile and run: 
 gcc -I/home/seed/openssl-1.0.1/include -o sample sample.c -L/home/seed/openssl-1.0.1/ -lcrypto -ldl 
 && ./sample md5
 */

#define MAX_MSG_LEN 20

void random_string(char *str, int *str_len)
{
  int i;
  
  *str_len = rand() % (MAX_MSG_LEN-1) + 1;

  for (i = 0; i < *str_len; ++i) {
    str[i] = '0' + rand() % 72; // from '0' to '}'
  }

  str[*str_len] = '\0';
}

main(int argc, char *argv[])
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    char mess1[] = "Hello world";
    char mess2[MAX_MSG_LEN+1];
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned char md_value_mess2[EVP_MAX_MD_SIZE];
    int md_len, i, mess2_len;
    int trial_count;
    
    OpenSSL_add_all_digests();
    
    if(!argv[1]) {
        printf("Usage: mdtest digestname\n");
        exit(1);
    }
    
    md = EVP_get_digestbyname(argv[1]) ;
    
    if(!md) {
        printf("Unknown message digest %s\n", argv[1]);
        exit(1);
    }
    
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);
    
    /* printf("Input: %s\n",mess1); */
    /* printf("Digest: "); */
    /* for(i = 0; i < md_len; i++) { */
    /*   printf("%02x", md_value[i]); */
    /* } */
    /* printf("\n"); */

    trial_count = 0;
    srand(time(NULL));
    while (1) {
      random_string(mess2, &mess2_len);
    
      mdctx = EVP_MD_CTX_create();
      EVP_DigestInit_ex(mdctx, md, NULL);
      EVP_DigestUpdate(mdctx, mess2, mess2_len);
      EVP_DigestFinal_ex(mdctx, md_value_mess2, &md_len);
      EVP_MD_CTX_destroy(mdctx);

      if (md_value_mess2[0] == md_value[0] &&
	  md_value_mess2[1] == md_value[1] &&
	  md_value_mess2[2] == md_value[2]) {
	printf("Matching Input: %s\n", mess2);
	printf("Matching Digest (24 bits): ");
	for(i = 0; i < md_len; i++) {
	  printf("%02x", md_value_mess2[i]);
	}
	printf("\n");
      
	break;
      }
      trial_count++;
    }
    printf("Number of trial: %d\n", trial_count);
    
    /* Call this once before exit. */
    EVP_cleanup();
    exit(0);
}
