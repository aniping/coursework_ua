#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define MAXITERS 10 // number of iterations
#define N 800 // grid size

double ** grid; // shared grid
int num_threads = 4;
int height;
double max_diff;
pthread_barrier_t barr;

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

void init_grid( int first_row, int last_row )
{
  int i,j;
  /* Initialize grid, including boundaries. */
  for (i = first_row; i <= last_row; ++i ) {
    for (j = 0; j <= (N+1); ++j) {
      if (i == 0 || i == (N+1) ||
	  j == 0 || j == (N+1) ) {
	grid[i][j] = 1;
      } else {
	grid[i][j] = 0;
      }
    }
  }
}

void * worker( void *arg )
{
  int id = *((int *) arg);
  int first_row = id * height + 1;
  int last_row = first_row + height - 1;
  int jstart, iter, i, j;
  double mydiff = 0.0, old;

  if (first_row == 1)
    init_grid( first_row-1, last_row );
  else if (last_row == N)
    init_grid( first_row-1, last_row+1 );
  else
    init_grid( first_row, last_row );

  pthread_barrier_wait( &barr );

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

    pthread_barrier_wait( &barr );
    
    /* Compute new values for red points in the grid. */
    for (i = first_row; i <= last_row; ++i) {
      if (i % 2 == 1) jstart = 2; // odd row
      else jstart = 1; // even row

      for (j = jstart; j <= N; j += 2) {
	grid[ i ][ j ] = ( grid[ i-1 ][ j ] + grid[ i+1 ][ j ] +
			   grid[ i ][ j-1 ] + grid[ i ][ j+1 ] ) * 0.25;
      }
    }

    pthread_barrier_wait( &barr );
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
      mydiff = MAX( mydiff, fabs(old - grid[i][j]) );
    }
  }

  pthread_barrier_wait( &barr );
  
  /* Compute new values for red points in the grid. */
  for (i = first_row; i <= last_row; ++i) {
    if (i % 2 == 1) jstart = 2; // odd row
    else jstart = 1; // even row

    for (j = jstart; j <= N; j += 2) {
      old = grid[ i ][ j ];
      grid[ i ][ j ] = ( grid[ i-1 ][ j ] + grid[ i+1 ][ j ] +
			 grid[ i ][ j-1 ] + grid[ i ][ j+1 ] ) * 0.25;
      mydiff = MAX( mydiff, fabs(old - grid[i][j]) );
    }
  }

  max_diff[ id ] = mydiff;
  printf( "Thread %d: Max diff: %lf\n", id, mydiff );

  return NULL;
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
  int i;
  int *arg;
  double maxdiff = 0.0;
  pthread_t threads[num_threads];
  
  height = N / num_threads;
  grid = allocate_grid( N+2 );
  max_diff = (double *) malloc( num_threads * sizeof(double) );

  pthread_barrier_init(&barr, NULL, num_threads);

  for (i = 0; i < num_threads; ++i) {
    arg = (int *) malloc( sizeof(int) );
    *arg = i;
    pthread_create( &threads[i], NULL, worker, (void *)arg );
  }

  for (i = 0; i < num_threads; ++i) {
    pthread_join( threads[i], NULL );
  }

  for (i = 0; i < num_threads; ++i) {
    maxdiff = MAX( maxdiff, max_diff[i] );
  }

  printf( "The maximum difference on the final iteration: %lf\n", maxdiff );

  return 0;
}
