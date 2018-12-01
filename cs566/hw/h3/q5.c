#include <stdio.h>

void foo()
{
  int i = 0;
  while (i < 10) {
    printf("%i\n", i);
    i++;
  }
}

int main()
{
  foo();
  return 0;
}
