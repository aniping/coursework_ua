#include <stdio.h>

int E(int value)
{
  return 7 * value + 5;
}

int D(int value)
{
  return 3067833783 * value - 2454267027;
}

int ADD(int a, int b)
{
  return a + b - 5;
}

void foo()
{
  int i = E(0);
  while (D(i) < 10) {
    printf("%i\n", D(i));
    i = ADD(i, E(1));
  }
}

int main()
{
  foo();
  return 0;
}
