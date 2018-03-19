#include <stdio.h>
#include <stdlib.h>

int main(){
  char *cc= "ok!\n";
  
  printf("%s", cc);
  
  int **m;
  m = malloc(3*sizeof(int*)) ;
  for (int i= 0; i < 3; i++){
    *(m + i) = malloc(sizeof(int) *3);
  } 
  for (int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      
      // === m[i][j]
      *(*(m + i) + j) = i+j;
      printf("%d ", *(*(m + i) + j));
    }
    printf("\n");
  }
  
  
  for (int i = 0; i < 3; i++){

    free( *(m + i) );
  }
  free(m);
  
  return 0;
}