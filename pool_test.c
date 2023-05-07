#include "pool.h"
#include "debug.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(char* argv[], int argc) {
  PoolAlloc a={0};
  printf("%d\n",sizeof(mempool_node));
  mempool_init(&a,3,4*sizeof(unsigned int),0);
  hexDump("backing buffer Before allocs",a.b,a.b_len,16);
  char* node1 = mempool_alloc(&a);
  memcpy(node1,"First Allocation",16);
  hexDump("backing buffer After 1 alloc",a.b,a.b_len,16);
  char* node2 = mempool_alloc(&a);
  memcpy(node2,"Some Filler Text",16);
  hexDump("backing buffer After 2 alloc",a.b,a.b_len,16);
  mempool_free(&a,node1);
  hexDump("backing buffer After 1 alloc and 1 free",a.b,a.b_len,16);
  mempool_destroy(&a);
  return 0;
}
