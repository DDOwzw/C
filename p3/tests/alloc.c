/* a simple 8 byte allocation */
#include <assert.h>
#include <stdlib.h>
#include "mem.h"

int main() {
   assert(Mem_Init(4096) == 0);
   void* ptr = Mem_Alloc(8);
   void* ptr1 = Mem_Alloc(8);
   assert(ptr != NULL);
   //Mem_Dump();
   exit(0);
}
