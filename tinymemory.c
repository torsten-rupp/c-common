/***********************************************************************\
*
* $Source$
* $Revision$
* $Author$
* Contents: tiny memory pool managment functions
* Systems : all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "global.h"

#include "tinymemory.h"

/****************** Conditional compilation switches *******************/
#define TINY_MEMORY_DEBUG

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/
typedef struct
{
  ulong size;
} Chunk;

typedef struct ChunkNode
{
  struct ChunkNode *next;
  ulong            size;
} ChunkNode;

struct __TinyMemory
{
  ChunkNode *chunkNode;
};

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef TINY_MEMORY_DEBUG
void dump(struct __TinyMemory *tinyMemory)
{
  ChunkNode *node;

  assert(tinyMemory != NULL);

  printf("Dump memory blocks:\n");
  printf("----------------------\n");
  node = tinyMemory->chunkNode;
  while (node != NULL)
  {
    printf("%p: %lu\n",node,node->size);
    node = node->next;
  }
  printf("----------------------\n");
}
#endif


LOCAL void addChunk(struct __TinyMemory *tinyMemory, ChunkNode *chunkNode)
{
  ChunkNode *prevNode,*node;

  assert(tinyMemory != NULL);
  assert(chunkNode != NULL);

  prevNode = NULL;
  node     = tinyMemory->chunkNode;
  while ((node != NULL) && (chunkNode->size > node->size))
  {
    prevNode = node;
    node = node->next;
  }

  if (prevNode != NULL)
  {
    prevNode->next = chunkNode;
    chunkNode->next = prevNode->next;
  }
  else
  {
    chunkNode->next = node;
    tinyMemory->chunkNode = chunkNode;
  }
}

LOCAL void remChunk(struct __TinyMemory *tinyMemory, ChunkNode *chunkNode)
{
  ChunkNode *prevNode,*node;

  assert(tinyMemory != NULL);
  assert(chunkNode != NULL);

  prevNode = NULL;
  node     = tinyMemory->chunkNode;
  while ((node != NULL) && (node != chunkNode))
  {
    prevNode = node;
    node = node->next;
  }
  assert(node != NULL);

  if (prevNode != NULL)
  {
    prevNode->next = chunkNode->next;
  }
  else
  {
    tinyMemory->chunkNode = chunkNode->next;
  }
}

LOCAL Chunk *allocChunk(TinyMemory tinyMemory, ulong size)
{
  ChunkNode *node;
  ulong     n;
  Chunk     *chunk;

  assert(tinyMemory != NULL);

  size = (size+sizeof(Chunk)-1) & ~(sizeof(Chunk)-1);

  node = tinyMemory->chunkNode;
  while ((node != NULL) && ((size+sizeof(Chunk)) > node->size))
  {
    node = node->next;
  }
  if (node == NULL)
  {
    return NULL;
  }

  n = node->size;
  if (n > (sizeof(Chunk)+size+sizeof(ChunkNode)))
  {
    /* split chunk */
    remChunk(tinyMemory,node);

    chunk = (Chunk*)node;
    chunk->size = sizeof(Chunk)+size;

    node = (ChunkNode*)((char*)node+sizeof(Chunk)+size);
    node->size = n-(sizeof(Chunk)+size);

    addChunk(tinyMemory,node);
  }
  else
  {
    /* use complete chunk */
    chunk = (Chunk*)node;
    chunk->size = n;
  }

  #ifdef TINY_MEMORY_DEBUG
    dump(tinyMemory);
  #endif

  return chunk;
}

LOCAL void freeChunk(TinyMemory tinyMemory, Chunk *chunk)
{
  ChunkNode *prevNode,*node,*nextNode;
  ChunkNode *chunkNode;
  ulong     n;

  assert(tinyMemory != NULL);
  assert(chunk != NULL);

  prevNode = NULL;
  node     = tinyMemory->chunkNode;
  while (   (node != NULL)
         && (((char*)node+node->size) != (char*)chunk)
         && ((char*)node != ((char*)chunk+chunk->size))
        )
  {
    prevNode = node;
    node     = node->next;
  }
  if (node != NULL)
  {
    /* merge with existing chunk */

    nextNode = node->next;

    remChunk(tinyMemory,node);

    if ((((char*)node+node->size) == (char*)chunk))
    {
      /* merge with existing chunk node at memory end */
      n = node->size+chunk->size;
      chunkNode = node;

      /* check if merging with next existing chunk is possible */
      if ((nextNode != NULL) && (((char*)chunkNode+n) == (char*)nextNode))
      {
        /* merge next chunk, too */
        n += nextNode->size;
        chunkNode = node;
        remChunk(tinyMemory,nextNode);
      }
    }
    else
    {
      /* merge with existing chunk node at memory start */
      n = node->size+chunk->size;
      chunkNode = (ChunkNode*)chunk;

      /* check if merging with previous excisting chunk is possible */
      if ((prevNode != NULL) && ((char*)chunkNode == ((char*)prevNode+prevNode->size)))
      {
        /* merge previous chunk, too */
        n += prevNode->size;
        chunkNode = prevNode;
        remChunk(tinyMemory,prevNode);
      }
    }
  }
  else
  {
    /* add new chunk */
    n = chunk->size;
    chunkNode = (ChunkNode*)chunk;
  }

  chunkNode->size = n;
  addChunk(tinyMemory,chunkNode);

  #ifdef TINY_MEMORY_DEBUG
    dump(tinyMemory);
  #endif
}

/*---------------------------------------------------------------------*/

TinyMemory tinyMemory_init(void *p, ulong size)
{
  struct __TinyMemory *tinyMemory;
  ChunkNode           *chunkNode;

  assert(p != NULL);

  if (size < (sizeof(struct __TinyMemory) + sizeof(ChunkNode)))
  {
    return NULL;
  }

  tinyMemory = (struct __TinyMemory*)((char*)p + 0                          );
  chunkNode  = (ChunkNode*          )((char*)p + sizeof(struct __TinyMemory));
  size = size - (sizeof(struct __TinyMemory) + sizeof(ChunkNode));

  tinyMemory->chunkNode = NULL;

  chunkNode->size = size;
  addChunk(tinyMemory,chunkNode);

  return tinyMemory;
}

void tinyMemory_done(TinyMemory tinyMemory)
{
  assert(tinyMemory != NULL);
}

void *tinyMemory_alloc(TinyMemory tinyMemory, ulong size)
{
  Chunk *chunk;

  assert(tinyMemory != NULL);

  size = (size+sizeof(Chunk)-1) & ~(sizeof(Chunk)-1);
  chunk = allocChunk(tinyMemory,size);

  return (chunk != NULL)?(void*)((char*)chunk+sizeof(Chunk)):NULL;
}

void *tinyMemory_realloc(TinyMemory tinyMemory, void *p, ulong newSize)
{
  Chunk *chunk;
  Chunk *newChunk;
  ulong n;

  assert(tinyMemory != NULL);

  chunk = (Chunk*)((char*)p-sizeof(chunk));

  newSize = (newSize+sizeof(Chunk)-1) & ~(sizeof(Chunk)-1);
  newChunk = allocChunk(tinyMemory,newSize);
  if (newChunk != NULL)
  {
    n = MIN(chunk->size,newChunk->size);
    memcpy((char*)newChunk+sizeof(Chunk),
           (char*)chunk+sizeof(Chunk),
           n-sizeof(Chunk)
          );
    freeChunk(tinyMemory,chunk);
  }

  return (newChunk != NULL)?(void*)((char*)newChunk+sizeof(Chunk)):NULL;
}

void tinyMemory_free(TinyMemory tinyMemory, void *p)
{
  Chunk *chunk;

  assert(tinyMemory != NULL);

  if (p != NULL)
  {
    chunk = (Chunk*)((char*)p-sizeof(chunk));
    freeChunk(tinyMemory,chunk);
  }
}

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef __cplusplus
  }
#endif

/* end of file */
