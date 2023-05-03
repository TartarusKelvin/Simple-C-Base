/*
=================================
Simple Fixed Chunk Pool Allocator
=================================

Uses the first 8 Bytes of each free chunk to point to the
next free chunk. We then store the head of this list in
the allocator. This means that allocations and frees
are in O(1) time however the initial allocation of the 
backing buffer is O(n). Best used when we expect to make
a series of similar sized allocations that we want to keep
close in memory aswell as be able to "deallocate".

Example Memory Layout After Init:
   0000<- 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
 ->0010-| a0 52 bb e5 a7 55 00 00 00 00 00 00 00 00 00 00  .R...U..........
 |-0020   b0 52 bb e5 a7 55 00 00 00 00 00 00 00 00 00 00  .R...U..........

Memory Layout After Allocating 2 Strings:
   0000   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
   0010   53 6f 6d 65 20 46 69 6c 6c 65 72 20 54 65 78 74  Some Filler Text
   0020   46 69 72 73 74 20 41 6c 6c 6f 63 61 74 69 6f 6e  First Allocation

Memory Layout After Returing The First Chunk:
 ->0000   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
 | 0010   53 6f 6d 65 20 46 69 6c 6c 65 72 20 54 65 78 74  Some Filler Text
 \-0020   a0 52 bb e5 a7 55 00 00 6c 6f 63 61 74 69 6f 6e  .R...U..location


//TODO{NS}: Add a "is full" func currently can just check
            if (p->h==NULL)
*/

#ifndef pool_scbase
#define pool_scbase
#include <stdint.h>
#include <stddef.h>
#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif
typedef struct mempool_node mempool_node;
struct mempool_node{mempool_node* next;};
typedef struct {
  unsigned char* b;
  size_t b_len;
  size_t chunk_size;
  size_t chunk_count;
  mempool_node* h;
} PoolAlloc;

/* Initalize a PoolAlloc struct with backing buffer
 PARAMS:
 p     pointer to the struct to intialize (Normally p={0})
 n     the number of chunks you want to have available
 c     the minimum size of a chunk
 align The memory alignment to use(0 for DEFAULT_ALIGNMENT)
       must be a power of 2
 NOTE:
 c will be aligned to the chosen alignment, as such 
 the buffer that gets allocated for the allocator may be
 larger then c*n, To get the size of an allocated chunk
 see p->chunk_size
*/
void mempool_init(PoolAlloc* p,size_t n,size_t c,size_t align);

/* Get a pointer to a free chunk from PoolAlloc
 Returns NULL if the PoolAlloc is full, otherwise returns
 a zero initialized buffer of memory of size p->chunk_size.
 This will be the first node currently in the free list
*/
void*mempool_alloc(PoolAlloc* p);

/* Return chunk to PoolAlloc
 Pushes chunk back onto the free list (see above)
 TODO{NS}:this should check that f is in the backing buffer
*/
void mempool_free(PoolAlloc* p, void* f);

/* Destroy given PoolAlloc pointer
 Simply calls free on the backing buffer
*/
void mempool_destroy(PoolAlloc* p);

/* Initialize the free list
 Adds every chunk to the free list equivalent to freeing
 each non free chunk. Note that the memory doesnt get wiped
*/
void mempool_freeall(PoolAlloc* p);
#endif
