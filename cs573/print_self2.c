int main()
{
  char *c="int main()%c{%c  char *c=%c%s%c;%c  printf(c,10,10,34,c,34,10,10,10);%c}%c";
  printf(c,10,10,34,c,34,10,10,10);
}
