/**
 * A short and concise description goes here.
 * *************************************************
 *
 * Author: Shuo Yang
 * Email: imsure95@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * The main program.
 */
int main( int argc, char *argv[] )
{
  int ret = strncmp("89", "secret", 100);
  printf("%d\n", ret);
  return 0;
}
