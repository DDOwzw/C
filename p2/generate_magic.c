////////////////////////////////////////////////////////////////////////////////
// Main File:        (P2)
// This File:        (generate_magic.c)
// Other Files:      (verify_magic.c)
// Semester:         CS 354 Spring 2017
//
// Author:           (Zhongwei Wang)
// Email:            (zwang685@wisc.edu)
// CS Login:         (zhongwei)
//
/////////// IF PAIR PROGRAMMING IS ALLOWED, COMPLETE THIS SECTION //////////////
//
// Pair Partner:     (name of your pair programming partner)
// Email:            (email address of your programming partner)
// CS Login:         (partner's CS login name)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
  int size;
  int **array;
} Square;

int get_square_size();
Square* generate_magic(int size);
void write_to_file(Square * square, char *filename);

int main(int argc, char *argv[])
{
  // Check input arguments to get filename
  if(argc != 2){   
    printf("Usage: ./generate_magic <filename>\n");
    exit(1);
  }
  
  // Get size from user
  int s = get_square_size();
  
  // Generate the magic square
  Square* sq = generate_magic(s);
  // Write the square to the output file
  write_to_file(sq, *(argv +1));
  
  
  // free the heap used
  for (int i = 0; i < (sq->size) ; i++){
    
    free( *((sq->array) + i) );
  }
  free((sq->array));
  sq->array = NULL;
  sq->size = 0;
  sq = NULL;
  
  return 0;
}

/* get_square_size prompts the user for the magic square size
 * checks if it is an odd number >= 3 and returns the number
 */
int get_square_size()
{
  int size;
  printf("Enter size of magic square, must be odd\n");
  fscanf(stdin, "%d", &size );
  if(size <= 2 || (size % 2) != 1){
    printf("Size must be an odd number >= 3.\n");
    exit(1);
  }else{
    
    return size;
  }
}

/* generate_magic constructs a magic square of size n
 * using the Siamese algorithm and returns the Square struct
 */
Square* generate_magic(int n)
{
  Square* s = malloc(sizeof(Square));
  s->size = n;
  // Initialize a new Square struct of that size
  int **m;
  m = malloc(sizeof(int*) * (s->size) ) ;
  for (int i= 0; i < (s->size); i++){
    *(m + i) = malloc(sizeof(int) * (s->size));
  }
  s->array = m;
  
  // Initialize all cells with value 0;
  for(int i = 0; i < n; ++i){
    for(int j = 0; j < n; ++j){
      //set m[i][j] = 0;
      *(*(m + i) + j) = 0;
    }
  }
  
  // using the Siamese algorithm
  int i = 0;
  int j = (n-1)/2;
  int next_i = 0;
  int next_j = 0;
  for(int k = 0; k < n*n; ++k){
    
   // set m[i][j] = k+1;
    *(*(m + i) + j) = k+1;
    next_i = i-1;
    next_j = j+1;
    if(next_i < 0){
      next_i = n-1;
    }else if(next_i >= n){
      next_i = 0;
    }
    if(next_j < 0){
      next_j = n-1;
    }else if(next_j >= n){
      next_j = 0;
    }
    // set m[next_i][next_j] == 0
    if( *(*(m + next_i ) + next_j) == 0){
      i = next_i;
      j = next_j;
    }else{
      i = i+1;
      j = j;
    }
  }
  
  
  return s;
}

/* write_to_file opens up a new file(or overwrites the existing file)
 * and writes out the square in the format expected by verify_magic.c
 */
void write_to_file(Square* square, char* filename)
{
  FILE *fp;
  fp = fopen(filename, "w");
  if(fp == NULL){
    printf("Cannot open file for writing.\n");
    exit(1);
  }else{
    fprintf(fp, "%d\n", square->size );
    for(int i = 0; i < square->size; ++i){
      for(int j = 0; j < square->size; ++j){
        if(j == square->size -1){
          fprintf(fp, "%d ", *(*(square->array + i) + j) );
        }else{
          fprintf(fp, "%d,", *(*(square->array + i) + j) );
        }
        
      }
      fprintf(fp,"\n");
    }
    fclose(fp);
  }
  
  
}