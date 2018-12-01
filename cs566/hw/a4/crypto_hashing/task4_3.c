#include <stdio.h>
#include<string.h>
#include <time.h>
#include <stdlib.h>
#include <openssl/evp.h>

/* To compile and run: 
 gcc -I/home/seed/openssl-1.0.1/include -o sample sample.c -L/home/seed/openssl-1.0.1/ -lcrypto -ldl 
 && ./sample md5
 */

main(int argc, char *argv[])
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char md_value1[EVP_MAX_MD_SIZE];
    unsigned char md_value2[EVP_MAX_MD_SIZE];
    int input1, input2;
    int md_len, i;
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

    srand(time(NULL));
    trial_count = 0;
    while (1) {
      input1 = rand();
      input2 = rand();
    
      mdctx = EVP_MD_CTX_create();
      EVP_DigestInit_ex(mdctx, md, NULL);
      EVP_DigestUpdate(mdctx, &input1, sizeof(int));
      EVP_DigestFinal_ex(mdctx, md_value1, &md_len);
      EVP_MD_CTX_destroy(mdctx);
    
      mdctx = EVP_MD_CTX_create();
      EVP_DigestInit_ex(mdctx, md, NULL);
      EVP_DigestUpdate(mdctx, &input2, sizeof(int));
      EVP_DigestFinal_ex(mdctx, md_value2, &md_len);
      EVP_MD_CTX_destroy(mdctx);
    
      if (md_value1[0] == md_value2[0] &&
    	  md_value1[1] == md_value2[1] &&
    	  md_value1[2] == md_value2[2]) {
	printf("Input1: %d\n", input1);
	printf("Digest: ");
	for(i = 0; i < md_len; i++) {
	  printf("%02x", md_value1[i]);
	}
	printf("\n");

	printf("Input2: %d\n", input2);
	printf("Digest: ");
	for(i = 0; i < md_len; i++) {
	  printf("%02x", md_value2[i]);
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
