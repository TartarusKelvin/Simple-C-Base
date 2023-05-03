#include "pool.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void hexDump (const char * desc,const void * addr,const int len,int perLine) {
  if (perLine < 4 || perLine > 64) perLine = 16;
  int i;
  unsigned char buff[perLine+1];
  const unsigned char * pc = (const unsigned char *)addr;
  if (desc != NULL) printf ("%s:\n", desc);
  if (len == 0) {
    printf("  ZERO LENGTH\n");
    return;
  }
  if (len < 0) {
    printf("  NEGATIVE LENGTH: %d\n", len);
    return;
  }
  for (i = 0; i < len; i++) {
    if ((i % perLine) == 0) {
      if (i != 0) printf ("  %s\n", buff);
      printf ("  %04x ", i);
    }
    printf (" %02x", pc[i]);
    if ((pc[i] < 0x20) || (pc[i] > 0x7e)) // isprint() may be better.
      buff[i % perLine] = '.';
    else
      buff[i % perLine] = pc[i];
    buff[(i % perLine) + 1] = '\0';
  }
  while ((i % perLine) != 0) {
    printf ("   ");
    i++;
  }
  printf ("  %s\n", buff);
}

void parray(int* arr, size_t count)
{
  for (int i = 0; i < count; i++)
    printf("%d ",arr[i]);
  printf("\n");
}

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
