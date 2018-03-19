////////////////////////////////////////////////////////////////////////////////
// Main File:        (P3)
// This File:        (mem.c)
// Other Files:      (mem.h, etc)
// Semester:         CS 354 Spring 2017
//
// Author:           (zhongwei)
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
//////////////////////////// 80 columns wide ///////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"


/*
 * This structure serves as the header for each allocated and free block
 * It also serves as the footer for each free block
 * The blocks are ordered in the increasing order of addresses 
 */
typedef struct block_tag
{

  int size_status;

  /*
  * Size of the block is always a multiple of 4
  * => last two bits are always zero - can be used to store other information
  *
  * LSB -> Least Significant Bit (Last Bit)
  * SLB -> Second Last Bit 
  * LSB = 0 => free block
  * LSB = 1 => allocated/busy block
  * SLB = 0 => previous block is free
  * SLB = 1 => previous block is allocated/busy
  * 
  * When used as the footer the last two bits should be zero
  */

  /*
  * Examples:
  * 
  * For a busy block with a payload of 24 bytes (i.e. 24 bytes data + an additional 4 bytes for header)
  * Header:
  * If the previous block is allocated, size_status should be set to 31
  * If the previous block is free, size_status should be set to 29
  * 
  * For a free block of size 28 bytes (including 4 bytes for header + 4 bytes for footer)
  * Header:
  * If the previous block is allocated, size_status should be set to 30
  * If the previous block is free, size_status should be set to 28
  * Footer:
  * size_status should be 28
  * 
  */

} block_tag;

/* Global variable - This will always point to the first block
 * i.e. the block with the lowest address */
block_tag *first_block = NULL;

/* Global variable - Total available memory */
int total_mem_size = 0;


/*
 * Helper function to print out the block_tag
 * With the hex address first and size_status next
 * 
 * block_tag* ptr the pointer to the block_tag
 */
void printTag(block_tag* ptr){
  if(NULL == ptr)
    printf("NULL%p\n", ptr);
  else
    printf("%p, %d\n",ptr , ptr-> size_status);
}

/*
 * Helper function to check is current blk free
 * Return 0 if is busy, 1 if is free
 * 
 * block_tag* ptr the pointer to the block_tag
 */
int isCurFree(block_tag *ptr)
{
  int a = ptr->size_status;
  if (a & 1)
    // current is busy
    return 0;
  else
    // current is free
    return 1;
}


/*
 * Helper function to check is previouos blk free
 * Return 0 if is busy, 1 if is free
 * 
 * block_tag* ptr the pointer to the block_tag
 */
int isPreFree(block_tag *ptr)
{
  int a = ptr->size_status;
  //printf("%d\n", a);
  if (a & 2)
    // prev is busy
    return 0;
  else
    // prev is free
    return 1;
}

/*
 * Helper function to get to the next blk
 * Return a pointer to a block_tag or NULL if out of bound
 * Not sure what is the min capacity of last chunk of mem that is usable????????????????
 * 
 * void* ptr the pointer to the block_tag
 */
block_tag *nextBlk(void *cur)
{ // calculate out the accurate size of bytes should in this blk
  int t_size = ((block_tag *)cur)->size_status;
  // check is current blk free
  if (t_size & 1 )
  {// LSB = 1 => busy block
    t_size = t_size - 1 + 4;
  }else
  {// It is Free blk;
  }
  // check is previous blk free
  if (t_size & 2 )
  {// prveious is busy
    t_size = t_size - 2;
  }else
  {// previous is free blk
  }
  // increase to the next pointer;
  block_tag *nex = (block_tag *)((char *)cur + t_size);

  // in the bound
  if (nex < (block_tag *)((char *)first_block + total_mem_size -7))// is it correct? Only hearder???
    return nex;
  else
    // out bound
    return NULL;
}


/* 
 * Function for allocating 'size' bytes
 * Returns address of the payload in the allocated block on success 
 * Returns NULL on failure 
 * Here is what this function should accomplish 
 * - If size is less than equal to 0 - Return NULL
 * - Round up size to a multiple of 4 
 * - Traverse the list of blocks and allocate the best free block which can accommodate the requested size 
 * - Also, when allocating a block - split it into two blocks when possible 
 * Tips: Be careful with pointer arithmetic 
 */
void *Mem_Alloc(int size)
{ /* Your code goes in here */
  // If size is less than equal to 0 - Return NULL
  if (size <= 0)
    return NULL;

  // Round up size to a multiple of 4
  int padsize = size % 4;
  
  padsize = (4 - padsize) % 4;
  size += padsize;
  
  // actual needed size for a success allocate, need 4 more bytes for header
  int actuSize = size + 4;
  
  block_tag *current = first_block;
  block_tag *best = NULL;
  
  // Traverse the list of blocks and find the best free block which can accommodate the requested size
  
  while (current != NULL)
  {
    if ((current->size_status & 1) == 0)
    { // current is free
      if( (current->size_status) >= actuSize){
        if (NULL == best ){
          best = current;
          //printTag(best);
        }else{
          int c = current->size_status;
          int b = best->size_status;
          if (c >= actuSize && c < b){
            //printf("%d\n", c);
            best = current;
            //printTag(best);
          }
        }
      }
    }
    current = nextBlk(current);
    
  }
  
  // update all the header and footer
  if (NULL == best)
    return NULL;
  int t_bes_size = best->size_status - 2;
 
  int margin = t_bes_size - actuSize;
  if( margin > 4){
   
  // divide
  block_tag *next = (block_tag *)((char *)best + actuSize);
  next->size_status = t_bes_size - actuSize +2;
  
 
  
  block_tag *footer = (block_tag *)((char *)best + t_bes_size - 4);
  footer->size_status -= actuSize;
  
  
  best->size_status = size + 3;
  
  
  return (void *)((char *)best + 4);

  }else{
  // check exist of next
  if(NULL == nextBlk(best)){
    // no next
    printf("best's Next is NULL?!\n");
  }else{
    block_tag * next = nextBlk(best);
    next->size_status += 2;
 
  }
  
  best->size_status -= 3;

  return (void *)((char *)best + 4);
}

  return NULL;
}

/* 
 * Function for freeing up a previously allocated block 
 * Argument - ptr: Address of the payload of the allocated block to be freed up 
 * Returns 0 on success 
 * Returns -1 on failure 
 * Here is what this function should accomplish 
 * - Return -1 if ptr is NULL
 * - Return -1 if ptr is not within the range of memory allocated by Mem_Init()
 * - Return -1 if ptr is not 4 byte aligned
 * - Mark the block as free 
 * - Coalesce if one or both of the immediate neighbours are free 
 */
int Mem_Free(void *ptr)
{
  /* Your code goes in here */
  // Return -1 if ptr is NULL
  if (NULL == ptr)
  {
    return -1;
  }
  
    block_tag *curPtr = (block_tag *)ptr;
    // Return -1 if curPtr is not within the range of memory allocated by Mem_Init()
    
    if( curPtr >= (block_tag *)((char *)first_block + total_mem_size -8) ||
        curPtr < (block_tag *)((char *)first_block ) )
      return -1;
    // Return -1 if ptr is not 4 byte aligned
    // ?????????????????????Is it the correct thing need to check?
    if( ( (int)curPtr % 4) != 0)
      return -1;

// correct the curPtr to the actual position of the header!!
curPtr = (block_tag *)((char *)curPtr - 4);


// find the next block first!

    block_tag *next = nextBlk(curPtr);
    if(NULL != next){
      int isNF = isCurFree(next);
      int isPF = isPreFree(curPtr);
      
      if(isNF && isPF){
        // Both pre and next are free!
        int sizeOfCur = curPtr->size_status - 1 + 4;
        block_tag * preFooter = (block_tag *)((char *)curPtr  - 4);
        int sizeOfPre = preFooter->size_status;
        int sizeOfNex = next->size_status -2;
        int totalSize = sizeOfCur + sizeOfPre + sizeOfNex;
        block_tag * preBlk = (block_tag *)((char *)preFooter  - (sizeOfPre - 4) );
        
        preBlk->size_status = totalSize + 2;
        block_tag * footer = (block_tag *)((char *)preBlk  + (totalSize - 4) );
        footer->size_status = totalSize;
      }else if(isNF){
        //printf("Next is free!\n");
        int realSize = curPtr->size_status - 3 + 4; 
        realSize += next->size_status -2;
        block_tag * footer = (block_tag *)((char *)curPtr  + (realSize - 4));
        footer->size_status = realSize;
        curPtr->size_status = realSize + 2;
      }else if(isPF){
        //printf("Prev is free!\n");
        int realSize = curPtr->size_status - 1 + 4;
        block_tag * footer = (block_tag *)((char *)curPtr  + (realSize - 4) );
        block_tag * preFooter = (block_tag *)((char *)curPtr  - 4);
        int addSize = preFooter->size_status;
        addSize += realSize;
        footer->size_status = addSize;
      
        block_tag *PreAdd = (block_tag *)((char *)preFooter - (preFooter->size_status - 4));
        
       
        PreAdd->size_status = addSize + 2;
        next->size_status -= 2;
      }else{
        //printf("Both are busy!\n");
        int realSize = curPtr->size_status - 3 + 4;
        block_tag * footer = (block_tag *)((char *)curPtr  + (realSize - 4));
        footer->size_status = realSize;
  
        curPtr->size_status = realSize + 2;
      
        next->size_status -= 2;
      }
    }else{
      int isPF = isPreFree(curPtr);
      if(isPF){
        //printf("Prev is free!No Next\n");
        int realSize = curPtr->size_status - 1 + 4;
        block_tag * footer = (block_tag *)((char *)curPtr  + (realSize - 4) );
        block_tag * preFooter = (block_tag *)((char *)curPtr  - 4);
        int addSize = preFooter->size_status;
        addSize += realSize;
        footer->size_status = addSize;
        
        block_tag *PreAdd = (block_tag *)((char *)preFooter - (preFooter->size_status - 4));
        
        PreAdd->size_status = addSize + 2;
        
      }else{
        //printf("Pre is busy! No Next\n");
        int realSize = curPtr->size_status - 3 + 4;
        block_tag * footer = (block_tag *)((char *)curPtr  + (realSize - 4));
        footer->size_status = realSize;
        curPtr->size_status = realSize + 2;
      }
    }

    return 0;
  
}

/*
 * Function used to initialize the memory allocator
 * Not intended to be called more than once by a program
 * Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated
 * Returns 0 on success and -1 on failure 
 */
int Mem_Init(int sizeOfRegion)
{
  int pagesize;
  int padsize;
  int fd;
  int alloc_size;
  void *space_ptr;
  static int allocated_once = 0;

  if (0 != allocated_once)
  {
    fprintf(stderr, "Error:mem.c: Mem_Init has allocated space during a previous call\n");
    return -1;
  }
  if (sizeOfRegion <= 0)
  {
    fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
    return -1;
  }

  // Get the pagesize
  pagesize = getpagesize();

  // Calculate padsize as the padding required to round up sizeOfRegion to a multiple of pagesize
  padsize = sizeOfRegion % pagesize;
  padsize = (pagesize - padsize) % pagesize;

  alloc_size = sizeOfRegion + padsize;

  // Using mmap to allocate memory
  fd = open("/dev/zero", O_RDWR);
  if (-1 == fd)
  {
    fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
    return -1;
  }
  space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (MAP_FAILED == space_ptr)
  {
    fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
    allocated_once = 0;
    return -1;
  }

  allocated_once = 1;

  // Intialising total available memory size
  total_mem_size = alloc_size;

  // To begin with there is only one big free block
  first_block = (block_tag *)space_ptr;

  // Setting up the header
  first_block->size_status = alloc_size;
  // Marking the previous block as busy
  first_block->size_status += 2;

  // Setting up the footer
  block_tag *footer = (block_tag *)((char *)first_block + alloc_size - 4);
  footer->size_status = alloc_size;
 
  return 0;
}

/* 
 * Function to be used for debugging 
 * Prints out a list of all the blocks along with the following information for each block 
 * No.      : serial number of the block 
 * Status   : free/busy 
 * Prev     : status of previous block free/busy
 * t_Begin  : address of the first byte in the block (this is where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block (as stored in the block header)(including the header/footer)
 */
void Mem_Dump()
{
  int counter;
  char status[5];
  char p_status[5];
  char *t_begin = NULL;
  char *t_end = NULL;
  int t_size;

  block_tag *current = first_block;
  counter = 1;

  int busy_size = 0;
  int free_size = 0;
  int is_busy = -1;

  fprintf(stdout, "************************************Block list***********************************\n");
  fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
  fprintf(stdout, "---------------------------------------------------------------------------------\n");

  while (current < (block_tag *)((char *)first_block + total_mem_size))
  {

    t_begin = (char *)current;

    t_size = current->size_status;

    if (t_size & 1)
    {
      // LSB = 1 => busy block
      strcpy(status, "Busy");
      is_busy = 1;
      t_size = t_size - 1 + 4;
    }
    else
    {
      strcpy(status, "Free");
      is_busy = 0;
    }

    if (t_size & 2)
    {
      strcpy(p_status, "Busy");
      t_size = t_size - 2;
    }
    else
      strcpy(p_status, "Free");

    if (is_busy)
      busy_size += t_size;
    else
      free_size += t_size;

    t_end = t_begin + t_size - 1;

    fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, p_status,
            (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

    current = (block_tag *)((char *)current + t_size);
    counter = counter + 1;
  }
  fprintf(stdout, "---------------------------------------------------------------------------------\n");
  fprintf(stdout, "*********************************************************************************\n");

  fprintf(stdout, "Total busy size = %d\n", busy_size);
  fprintf(stdout, "Total free size = %d\n", free_size);
  fprintf(stdout, "Total size = %d\n", busy_size + free_size);
  fprintf(stdout, "*********************************************************************************\n");
  fflush(stdout);
  return;
}

