/**
 * De-obfuscated program for assignment 2-Task A-1.
 * *************************************************
 *
 * Author: Shuo Yang
 * Email: shuoyang@email.arizona.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * The main program.
 */
int main( int argc, char *argv[] )
{
  unsigned long input_value, tmp1, tmp2, output_value;

  if (argc != 2) {
    printf("Call this program with %i arguments\n", 1);
    exit(-1);
  }

  input_value = strtoul((char const *)argv[1], NULL, 10);
  tmp1 = input_value & 680373089UL;
  tmp2 = input_value + 282530747185134996UL;
  tmp2 /= 2UL;

  output_value = ((tmp1 + tmp2) << 7UL) | ((tmp1 + tmp2) >> 57UL);

  printf("%lu\n", output_value);

  return 0;
}
