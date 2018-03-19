////////////////////////////////////////////////////////////////////////////////
// Main File:        (P2)
// This File:        (verify_magic.c)
// Other Files:      (generate_magic.c)
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
#include <string.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
  int size;
  int **array;
} Square;

Square* construct_square(char *filename);
int verify_magic(Square * square);

int main(int argc, char* argv[])
{
  printf("%d\n", sizeof(int*));
  printf("%d\n", sizeof(Square*));
  printf("%d\n", sizeof(Square));
  
  
  // Check input arguments to get filename
  if(argc != 2){   
    printf("Usage: ./verify_magic <filename>\n");
    exit(1);
  }
  // Construct square
  Square* s = construct_square(*(argv + 1) );
  
  // Verify if it's a magic square and print true or false
   if (verify_magic(s) < 0) {
   printf("false\n");
   }
   else {
   printf("true\n");
   }
   
  // free the uesd heap
  for (int i = 0; i < (s->size) ; i++){
    
    free( *((s->array) + i) );
  }
  free((s->array));
  s->array = NULL;
  s->size = 0;
  s = NULL;
  Cannot open file for readin
  return 0;
}

/* construct_square reads the input file to initialize a square struct
 * from the contents of the file and returns the square.
 * The format of the file is defined in the assignment specifications
 */
Square* construct_square(char* filename)
{

  // Open and read the file
  FILE *fp;
  fp = fopen(filename, "r");
  // Read the first line to get the square size
  Square* s = malloc( sizeof(Square) );
  if(fp == NULL){
    printf("Cannot open file for reading.\n");
    exit(1);
  }else{
    fscanf(fp, "%d", &(s->size) );
    // Initialize a new Square struct of that size
    int **m;
    m = malloc(sizeof(int*) * (s->size) ) ;
    for (int i= 0; i < (s->size); i++){
      *(m + i) = malloc(sizeof(int) * (s->size));
    }
    // Read the rest of the file to fill up the square
    int c;
    int num = (s->size);
    for( int i = 0; i < num; ++i){
      for( int j = 0; j < num; ++j){
        if(j == 0){
          fscanf(fp, "%d", &c);
        }else{
          fscanf(fp, ",%d", &c);
        }
        // store it at represent location in the array m
        // !!!!!!!!!!!!!!!!!!!
        *(*(m + i) + j) = c;
        
      }
    }
    // set s.array to point to the heap where m points to
    s->array = m;
    // close the file
    fclose(fp);
  }
  
  return s;
}

/* verify_magic verifies if the square is a magic square
 *
 * returns -1(false) or 0(ture)
 */
int verify_magic(Square* square)
{   
  
  int sum = 0;
  int test_rows = 0;
  int test_cols = 0;
  int test_diagnal_m = 0;
  int test_diagnal_s = 0;
  int size = square->size;
  int **m;
  m = square->array;
  for(int i = 0; i < size; ++i){
    // m[0][i]
    sum += *(*( m + 0 ) + i);
  }
  for (int i = 0; i < size; ++i){
    
    for(int j = 0; j < size; ++j){
      // m[0][j]
      // Check all rows sum to same number
      test_rows += *(*( m + i ) + j);
      // m[j][0]; 
      // Check all cols sum to same number
      test_cols += *(*( m + j ) + i);
      
    }
    test_diagnal_m += *(*(m + i) + i);
    test_diagnal_s += *(*(m + i) + (size - i - 1));
    if( sum != test_rows || sum != test_cols ){
      return -1;
    }else{
      test_rows = test_cols = 0;
    }
  }
  // Check main diagonal
  // Check secondary diagonal
  if( sum != test_diagnal_m || sum != test_diagnal_s){
    return -1;
  }else{
    return 0;
  }
  
}
