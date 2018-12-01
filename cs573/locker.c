#include <stdio.h>

void change_state( char *states, int index )
{
  if (states[index] == 'c')
    states[index] = 'o';
  else
    states[index] = 'c';
}

int main()
{
  int locker[100];
  char states[100];
  int i,j, k;

  for (i = 0; i < 100; ++i) {
    states[i] = 'c';
  }

  for (i = 1; i <= 100; ++i) { // students
    for (j = 1; j <= 100; ++j) { // locker state
      if (j % i == 0) {
	change_state( states, j-1 );
      }
    }
    printf("student %d: ", i);
    for (k = 0; k < 100; ++k) {
      if (states[k] == 'o')
	printf("%d ", k+1);
    }
    putchar('\n');
  }
}
