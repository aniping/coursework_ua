#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXITERS 10
#define N 800

double MAX( double a, double b ) {
  return ( a > b )? a : b;
}

/**
 * Allocate a n*n grid
 */
double ** allocate_grid( int n )
{
  int i;
  double ** outer_ptr = malloc( n * sizeof(double*) );

  if ( outer_ptr != NULL ) {
    for (i = 0; i < n; ++i) {
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

void init_grid( double **grid, int size )
{
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      if (i == 0 || i == (size-1) ||
	  j == 0 || j == (size-1) ) {
	grid[i][j] = 1;
      } else {
	grid[i][j] = 0;
      }
    }
  }
}

void print_grid( double **grid, int size )
{
  int i, j;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      printf( "%lf ", grid[i][j] );
    }
    putchar('\n');
  }
}

int main(int argc, char *argv[])
{
  int height = N;
  double ** grid;
  double max_diff = 0.0, old;
  int first_row = 1, last_row = height;
  int iter, jstart, i, j;

  grid = allocate_grid( N+2 );
  init_grid( grid, N+2 );

  for (iter = 1; iter <= MAXITERS; ++iter) {
    /* Compute new values for red points in the grid. */
    for (i = first_row; i <= last_row; ++i) {
      if (i % 2 == 1) jstart = 1; // odd row
      else jstart = 2; // even row

      for (j = jstart; j <= N; j += 2) {
	grid[ i ][ j ] = ( grid[ i-1 ][ j ] + grid[ i+1 ][ j ] +
			   grid[ i ][ j-1 ] + grid[ i ][ j+1 ] ) * 0.25;
      }
    }

    /* Compute new values for red points in the grid. */
    for (i = first_row; i <= last_row; ++i) {
      if (i % 2 == 1) jstart = 2; // odd row
      else jstart = 1; // even row

      for (j = jstart; j <= N; j += 2) {
	grid[ i ][ j ] = ( grid[ i-1 ][ j ] + grid[ i+1 ][ j ] +
			   grid[ i ][ j-1 ] + grid[ i ][ j+1 ] ) * 0.25;
      }
    }
  }

  /**
   * Do the iteration one more time to compute the max difference among each cell.
   */

  /* Compute new values for red points in the grid. */
  for (i = first_row; i <= last_row; ++i) {
    if (i % 2 == 1) jstart = 1; // odd row
    else jstart = 2; // even row

    for (j = jstart; j <= N; j += 2) {
      old = grid[ i ][ j ];
      grid[ i ][ j ] = ( grid[ i-1 ][ j ] + grid[ i+1 ][ j ] +
			 grid[ i ][ j-1 ] + grid[ i ][ j+1 ] ) * 0.25;
      max_diff = MAX( max_diff, fabs(old - grid[i][j]) );
    }
  }

  /* Compute new values for red points in the grid. */
  for (i = first_row; i <= last_row; ++i) {
    if (i % 2 == 1) jstart = 2; // odd row
    else jstart = 1; // even row

    for (j = jstart; j <= N; j += 2) {
      old = grid[ i ][ j ];
      grid[ i ][ j ] = ( grid[ i-1 ][ j ] + grid[ i+1 ][ j ] +
			 grid[ i ][ j-1 ] + grid[ i ][ j+1 ] ) * 0.25;
      max_diff = MAX( max_diff, fabs(old - grid[i][j]) );
    }
  }

  printf( "The maximum difference on the final iteration: %lf\n", max_diff );

  return 0;
}
