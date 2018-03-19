#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure representing Square
typedef struct _Square {
  int a;
  int b;
} Square;

int main(){
  struct _Square sq;
  sq.a = 1;
  sq.b = 2;
  
  Square sx;
  sx.a = 3;
  sx.b = 4;
  printf("%i, %i\n", sq.a, sq.b);
  
  printf("%i, %i\n", sx.a, sx.b);
  return 0;
}