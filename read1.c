#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"btree.h"
int main()
{
  char a,b;
  long j;
  char *key;
  char *value;
  BTA *btact;
  btact=btopn("Tudien",0,1);
  if(btact==NULL)
  btact=btcrt("Tudien",0,1);
  BTint x;
  FILE *f;
  f=fopen("database","r");
  
  while(1)
    {
      key=(char*)calloc(100,sizeof(key));	
      value=(char*)calloc(100000,sizeof(value));
      a=fgetc(f);
      
       
         j=0;
         while((a=fgetc(f))!='/')
	   {
              if(a=='\n')
                break;
	      key[j]=a;
	      j++;      
	   }
         
         if(a=='/')
         {
         key[j-1]='\0';
         fseek(f,-1,SEEK_CUR);
         }
         j=0;
         while((b=fgetc(f))!='@')
           {
             value[j]=b;
             j++;
             if(b==EOF)break;
           }
         value[j-1]=='\0';
         if(b=='@')
         fseek(f,-1,SEEK_CUR);
        if(bfndky(btact,key,&x)!=0)
          {   
      	btins(btact,key,value,10000);
          }
        else
            {
              int s;
             
              char *value2;
              value2=(char*)calloc(100000,sizeof(value2));
              btsel(btact,key,value2,10000,&s);
              
              strcat(value2,value);
              btupd(btact,key,value2,10000);
              free(value2);
            }
        free(key);
        free(value);
        if(b==EOF)break;       
    }

        
          char a1[100];
	  char s1[100000];
	  int size;
	  printf("Nhap tu:");
	  __fpurge(stdin);
	  gets(a1);
	  if(btsel(btact,a1,s1,10000,&size))
	    printf("Khong co tu\n");
	  else
	    printf("%s",s1);
         
}
