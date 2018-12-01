/**
 * Flattened C code.
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
int main()
{
  int t, a, b, c, next;

  next = 0;

  for (;;) {
    printf("t = %d\n", t);
    switch (next) {
    case 0:
      t = 23; a = t % 5 + 7; b = a + 99; c = a + b - t;
      next = 1; break;
    case 1:
      if (a > b) next = 2; else next = 3; break;
    case 2:
      { int k = a + 9; b = a - k; next = 4; break; }
    case 3:
      a = a * 3; b = b - a; t = b; next = 4; break;
    case 4:
      if (b > 2) next = 5; else next = 6; break;
    case 5:
      a = a - 14 + t; next = 6; break;
    case 6:
      {
	int d = a - b + t - 7;
	int final = (t + 23) % 9;
	next = 7; break;
      }
    case 7:
      if (t > 1) next = 1; else next = 8; break;
    case 8:
      return t;
    }
  }
}
