/**
 * Matrix multiplication using a single thread.
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Allocate a n*n matrix
 */
double ** allocate_matrix( int n )
{
  double ** outer_ptr = malloc( n * sizeof(double*) );

  if ( outer_ptr != NULL ) {
    for (int i = 0; i < n; ++i) {
      outer_ptr[i] = malloc( n * sizeof(double) );
      if ( outer_ptr[i] == NULL ) {
	perror( "malloc error!" );
      }
    }
  } else {
    perror( "malloc error!" );
  }
  
  return outer_ptr;
}

void print_matrix( double **m, int n )
{
  int i, j;
  for ( i = 0; i < n; ++i ) {
    for ( j = 0; j < n; ++j ) {
      printf( "%lf, ", m[ i ][ j ] );
    }
    putchar( '\n' );
  }
}

int main( int argc, char *argv[] )
{
  int n = 100;
  double **m1, **m2, **m3;
  int i, j, k;
  double sum;

  m1 = allocate_matrix( n );
  m2 = allocate_matrix( n );
  m3 = allocate_matrix( n );

  /* Initialize arrays */
  for ( i = 0; i < n; ++i ) {
      for ( j = 0; j < n; ++j ) {
	m1[ i ][ j ] = i * j;
	m2[ i ][ j ] = i * j;
      }
  }

  //print_matrix( m1, n );
  //print_matrix( m2, n );

  /* Multiplying two matrixs */
  for ( i = 0; i < n; ++i ) {
    sum = 0.0;
    for ( j = 0; j < n; ++j ) {
      for ( k = 0; k < n; ++k ) {
	sum += m1[ i ][ k ] * m2[ k ][ j ];
      }
      m3[ i ][ j ] = sum;
    }
  }

  printf( "m3[n-1][n-1] = %lf\n", m3[ n-1 ][ n-1 ] );
  //print_matrix( m3, n );
  
  return 0;
}

