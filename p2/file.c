#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  
  FILE *fp;
  fp = fopen("11", "r");
  if(fp == NULL){
    printf("Cannot open file for reading.\n");
  }
  int a;
  if(fp){
    fscanf(fp, "%d", &a );
  }
  printf("%d\n", a);
  //char str[99];
  
  int c;
  int num = 3;
  for( int i = 0; i < num; ++i){
    for( int j = 0; j < num; ++j){
      if(j == 0){
        fscanf(fp, "%d", &c);
        printf("%d  ", c);
      }else{
        fscanf(fp, ",%d", &c);
        printf("%d  ", c);
      }
      
    }
    printf("\n");
  }
  
  /*
  while(1)
  {
    //c = fgetc(fp);
    fscanf(fp, "%d", &c );
    if( feof(fp) )
    { 
      break ;
    }
    printf("%d\n", c);
  }
  */
  
  fclose(fp);
  return 0;
}