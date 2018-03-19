////////////////////////////////////////////////////////////////////////////////
// Main File:        (P4)
// This File:        (cache2Drows.c)
// Other Files:      (cache1D.c cache2Dcols.c csim.c)
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
#include <stdlib.h>

int arr2D[3000][500];
/*
 * main - Main routine
 * assign values to each elems in the array
 * By row-wise order 
 */
int main(){
  for(int i = 0; i < 3000; ++i){
    for(int j = 0; j < 500; ++j){
      arr2D[i][j] = i + j;
    }
  }
  return 0;
}