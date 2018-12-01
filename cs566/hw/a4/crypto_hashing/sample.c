#include <stdio.h>
#include<string.h>
#include <openssl/evp.h>

/* To compile and run: 
 gcc -I/home/seed/openssl-1.0.1/include -o sample sample.c -L/home/seed/openssl-1.0.1/ -lcrypto -ldl 
 && ./sample md5
 */

main(int argc, char *argv[])
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    char mess1[] = "so code many debug much logic very program wow";
    unsigned char md_value[EVP_MAX_MD_SIZE];
    int md_len, i;
    
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
    
    printf("Input: %s\n",mess1);
    printf("Digest: ");
    for(i = 0; i < md_len; i++)
    printf("%02x", md_value[i]);
    printf("\n");
    
    /* Call this once before exit. */
    EVP_cleanup();
    exit(0);
}