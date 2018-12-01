#include <stdio.h>
#include <string.h>

#define MARYEAR 699

int main()
{
  int i, n = 32;
  double NP = 1;

  for ( i = 0; i < n; ++i ) {
    NP *= 1 - (double)i / MARYEAR;
    printf( "%lf", NP );
  }

  printf( "The probablity of two people sharing a birthday among %d people is %lf.\n"
	  , n, 1-NP);
}
