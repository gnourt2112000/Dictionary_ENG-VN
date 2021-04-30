#include<stdio.h>
int main()
{
char a;
  char h[100];
  FILE *f;
  f=fopen("history","a+");
  int i=0;
  while(a!=EOF)
  {
    a=fgetc(f);
    h[i++]=a;
  }
  h[i-1]='\0';
  printf("%s ",h);
}
