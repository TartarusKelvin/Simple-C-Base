#include "pool.h"
#include "basemacro.h"
#include <assert.h> 
#include <stdlib.h>
#include <string.h>
typedef PoolAlloc*tPA;typedef mempool_node tFR;
Vt mempool_init(tPA p,SZt n,SZt c,SZt a){                  // Initiallize a PoolAllocator 
 assert(c>=SIZE(tFR*));                                    //  Chunk size must be large enough for next pointer
 a=a?a:DEFAULT_ALIGNMENT;assert(0==(a&(a-1)));             //  If align is 0 set to def and ensure a=2^n
 SZt m=c&(a-1);c=m?c+a-m:c;                                //  Align the chunk size (note this means we over allocate)
 p->b_len=n*c;p->b=malloc(p->b_len);p->chunk_size=c;       //  Allocate the buffer and define defaults
 p->chunk_count=n;mempool_freeall(p);}                     //  Generate the free list
Vt mempool_freeall(tPA p){                                 // Free all chunks(or re-init chunks)
 REPEAT(p->chunk_count){tFR*n=(tFR*)&p->b[i*p->chunk_size];//  For each header node (n) of the chunk list 
 n->next=p->h;p->h=n;}}                                    //  Push to the free list
Vt mempool_destroy(tPA p){free(p->b);}                     // Free the allocator
Vt*mempool_alloc(tPA p){                                   // Allocate a chunk 
 tFR*node=p->h;if(node==NULL)R NULL;p->h=node->next;       //  Pop a node from the free list (if none return NULL)
 R MEMCL(node,p->chunk_size);}                             //  Clear the chunk before returning 
Vt mempool_free(tPA p,Vt*f){                               // Return Chunk to allocator
 tFR*n=(tFR*)f;n->next=p->h;p->h=n;}                       //  Push the chunk to the head and redefine its header
