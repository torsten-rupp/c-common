/***********************************************************************\
*
* $Source: /home/torsten/cvs/bar/bar/dictionaries.c,v $
* $Revision$
* $Author$
* Contents: hash table functions
* Systems: all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "global.h"

#include "dictionaries.h"

/****************** Conditional compilation switches *******************/

#define COLLISION_ALGORITHM_LINEAR_PROBING    1
#define COLLISION_ALGORITHM_QUADRATIC_PROBING 2
#define COLLISION_ALGORITHM_REHASH            3

//#define COLLISION_ALGORITHM COLLISION_ALGORITHM_LINEAR_PROBING
//#define COLLISION_ALGORITHM COLLISION_ALGORITHM_QUADRATIC_PROBING
#define COLLISION_ALGORITHM COLLISION_ALGORITHM_REHASH

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/
#define DATA_START_SIZE (16*1024)
#define DATA_DELTA_SIZE ( 4*1024)
#define MAX_DATA_SIZE   (2*1024*1024*1024)

/* hash table sizes */
LOCAL const uint TABLE_SIZES[] =
{
  1031,
  2053,
  4099,
  8209,
  16411,
  32771,
  65537,
  131101,
  262147,
  524309
};

#if COLLISION_ALGORITHM==COLLISION_ALGORITHM_LINEAR_PROBING
  #define LINEAR_PROBING_COUNT 4
#endif
#if COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING
  #define QUADRATIC_PROBING_COUNT 4
#endif
#if COLLISION_ALGORITHM==COLLISION_ALGORITHM_REHASH
  #define REHASHING_COUNT 4
#endif

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

#if COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING
/***********************************************************************\
* Name   : modulo
* Purpose: n mod m
* Input  : n,m - numbers
* Output : -
* Return : n mod m
* Notes  : -
\***********************************************************************/

LOCAL_INLINE ulong modulo(ulong n, ulong m)
{
  return n%m;
}

/***********************************************************************\
* Name   : addModulo
* Purpose: add and modulo
* Input  : n,d,m - numbers
* Output : -
* Return : (n+d) mod m
* Notes  : -
\***********************************************************************/

LOCAL_INLINE ulong addModulo(ulong n, uint d, ulong m)
{
  return (n+d)%m;
}

/***********************************************************************\
* Name   : subModulo
* Purpose: sub and modulo
* Input  : n,d,m - numbers
* Output : -
* Return : (n-d) mod m
* Notes  : -
\***********************************************************************/

LOCAL_INLINE ulong subModulo(ulong n, uint d, ulong m)
{
  return (n+m-d)%m;
}
#endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING */

/***********************************************************************\
* Name   : rotHash
* Purpose: rotate hash value
* Input  : hash - hash value
*          n    - number of bits to rotate hash value
* Output : -
* Return : rotated hash value
* Notes  : -
\***********************************************************************/

LOCAL_INLINE ulong rotHash(ulong hash, int n)
{
  uint shift;

  assert(n < 32);

  shift = 32-n;
  return ((hash & (0xFFFFffff << shift)) >> shift) | (hash << n);
}

/***********************************************************************\
* Name   : calculateHash
* Purpose: calculate hash
* Input  : data   - data
*          length - length of data
* Output : -
* Return : hash value
* Notes  : -
\***********************************************************************/

LOCAL ulong calculateHash(const void *keyData, ulong keyLength)
{
  byte       hashBytes[4];
  const byte *p;
  uint       z;

  assert(keyData != NULL);

  p = (const byte*)keyData;

  hashBytes[0] = (keyLength > 0) ? (*p) : 0; p++;
  hashBytes[1] = (keyLength > 1) ? (*p) : 0; p++;
  hashBytes[2] = (keyLength > 2) ? (*p) : 0; p++;
  hashBytes[3] = (keyLength > 3) ? (*p) : 0; p++;
  for (z = 4; z < keyLength; z++)
  {
    hashBytes[z%4] ^= (*p); p++;
  }

  return (ulong)(hashBytes[3] << 24) |
         (ulong)(hashBytes[2] << 16) |
         (ulong)(hashBytes[1] <<  8) |
         (ulong)(hashBytes[0] <<  0);
}

/***********************************************************************\
* Name   : equalsEntry
* Purpose: check if entry is equal to data
* Input  : entry - entry
*          hash  - data hash value
*          key   - key value
* Output : -
* Return : TRUE if entry is equal, FALSE otherwise
* Notes  : -
\***********************************************************************/

LOCAL_INLINE bool equalsEntry(const HashTableEntry *entry,
                              ulong                hash,
                              ulong                key,
                             )
{
  assert(entry != NULL);
  assert(keyData != NULL);

  if ((hash == entry->hash) && (entry->keyLength == keyLength))
  {
    if (entry->key == key)
    {
      return TRUE;
    }
  }

  return FALSE;
}

/***********************************************************************\
* Name   : findEntryIndex
* Purpose: find entry index of entry in table
* Input  : entryTable - entry table
*          hash       - data hash value
*          key        - key value
* Output : -
* Return : index of -1 if entry not found
* Notes  : -
\***********************************************************************/

LOCAL int findEntryIndex(HashTableEntryTable      *entryTable,
                         ulong                    hash,
                         ulong                    key,
                         HashTableCompareFunction hashTableCompareFunction,
                         void                     *hashTableCompareUserData
                        )
{
  uint z,i;
  int  entryIndex;

  assert(entryTable != NULL);
  assert(keyData != NULL);

  for (z = 0; z <= entryTable->sizeIndex; z++)
  {
    #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_LINEAR_PROBING
      for (i = 0; i < LINEAR_PROBING_COUNT; i++)
      {
        entryIndex = addModulo(hash,i,TABLE_SIZES[z]);
        if (equalsEntry(&entryTable->entries[entryIndex],
                        hash,
                        keyData,
                        keyLength,
                        hashTableCompareFunction,
                        hashTableCompareUserData
                       )
           )
        {
          return entryIndex;
        }
      }
    #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_LINEAR_PROBING */
    #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING
      entryIndex = modulo(hash,TABLE_SIZES[z]);
      if (equalsEntry(&entryTable->entries[entryIndex],
                      hash,
                      keyData,
                      keyLength,
                      hashTableCompareFunction,
                      hashTableCompareUserData
                     )
         )
      {
        return entryIndex;
      }
      for (i = 1; i < QUADRATIC_PROBING_COUNT; i++)
      {
        entryIndex = addModulo(hash,i*i,TABLE_SIZES[z]);
        if (equalsEntry(&entryTable->entries[entryIndex],
                        hash,
                        keyData,
                        keyLength,
                        hashTableCompareFunction,
                        hashTableCompareUserData
                       )
           )
        {
          return entryIndex;
        }
        entryIndex = subModulo(hash,i*i,TABLE_SIZES[z]);
        if (equalsEntry(&entryTable->entries[entryIndex],
                        hash,
                        keyData,
                        keyLength,
                        hashTableCompareFunction,
                        hashTableCompareUserData
                       )
           )
        {
          return entryIndex;
        }
      }
    #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING */
    #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_REHASH
      for (i = 0; i < REHASHING_COUNT; i++)
      {
        entryIndex = rotHash(hash,i)%TABLE_SIZES[z];
        if (equalsEntry(&entryTable->entries[entryIndex],
                        hash,
                        keyData,
                        keyLength,
                        hashTableCompareFunction,
                        hashTableCompareUserData
                       )
           )
        {
          return entryIndex;
        }
      }
    #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_REHASH */
  }

  return -1;
}

/***********************************************************************\
* Name   : findFreeEntryIndex
* Purpose: find free index in hash table
* Input  : entryTable - entry table
*          hash       - hash value
* Output : -
* Return : index of -1 if not free entry in table
* Notes  : -
\***********************************************************************/

LOCAL int findFreeEntryIndex(HashTableEntryTable *entryTable,
                             ulong               hash
                            )
{
  uint z,i;
  int  entryIndex;

  assert(entryTable != NULL);

  for (z = 0; z <= entryTable->sizeIndex; z++)
  {
    #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_LINEAR_PROBING
      for (i = 0; i < LINEAR_PROBING_COUNT; i++)
      {
        entryIndex = addModulo(hash,i,TABLE_SIZES[z]);
        if (entryTable->entries[entryIndex].data == NULL)
        {
          return entryIndex;
        }
      }
    #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_LINEAR_PROBING */
    #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING
      entryIndex = modulo(hash,TABLE_SIZES[z]);
      if (entryTable->entries[entryIndex].data == NULL)
      {
        return entryIndex;
      }
      for (i = 0; i < QUADRATIC_PROBING_COUNT; i++)
      {
        entryIndex = addModulo(hash,i*i,TABLE_SIZES[z]);
        if (entryTable->entries[entryIndex].data == NULL)
        {
          return entryIndex;
        }
        entryIndex = subModulo(hash,i*i,TABLE_SIZES[z]);
        if (entryTable->entries[entryIndex].data == NULL)
        {
          return entryIndex;
        }
      }
    #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING */
    #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_REHASH
      for (i = 0; i < REHASHING_COUNT; i++)
      {
        entryIndex = rotHash(hash,i)%TABLE_SIZES[z];
        if (entryTable->entries[entryIndex].data == NULL)
        {
          return entryIndex;
        }
      }
    #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_REHASH */
  }

  return -1;
}

/***********************************************************************\
* Name   : findEntry
* Purpose: find entry in hash table
* Input  : hashTable                - hash table
*          hash                     - data hash value
*          data                     - data
*          length                   - length of data
* Output : hashTableEntryTable - hash table entry table
*          index               - index in table
* Return : TRUE if entry found, FALSE otherwise
* Notes  : -
\***********************************************************************/

LOCAL bool findEntry(HashTable           *hashTable,
                     ulong               hash,
                     const void          *keyData,
                     ulong               keyLength,
                     HashTableEntryTable **hashTableEntryTable,
                     uint                *index
                    )
{
  uint z;
  uint tableIndex;
  int  i;

  assert(hashTable != NULL);
  assert(keyData != NULL);
  assert(hashTableEntryTable != NULL);
  assert(index != NULL);

  (*hashTableEntryTable) = NULL;
  (*index)                = -1;
  z = 0;
  while ((z < hashTable->entryTableCount) && ((*hashTableEntryTable) == NULL))
  {
    tableIndex = (hash+z)%hashTable->entryTableCount;
    i = findEntryIndex(&hashTable->entryTables[tableIndex],
                       hash,
                       keyData,
                       keyLength,
                       hashTable->hashTableCompareFunction,
                       hashTable->hashTableCompareUserData
                      );
    if (i >= 0)
    {
      (*hashTableEntryTable) = &hashTable->entryTables[tableIndex];
      (*index)               = i;
    }
    z++;
  }

  return ((*hashTableEntryTable) !=NULL);
}

/***********************************************************************\
* Name   : findFreeEntry
* Purpose: find free entry in hash table
* Input  : hashTable - hash table
*          hash      - data hash value
* Output : hashTableEntryTable - hash table entry table
*          index               - index in table
* Return : TRUE if entry found, FALSE otherwise
* Notes  : -
\***********************************************************************/

LOCAL bool findFreeEntry(HashTable           *hashTable,
                         ulong               hash,
                         HashTableEntryTable **hashTableEntryTable,
                         uint                *index
                        )
{
  uint z;
  uint tableIndex;
  int  i;

  assert(hashTable != NULL);
  assert(hashTableEntryTable != NULL);
  assert(index != NULL);

  (*hashTableEntryTable) = NULL;
  (*index)                = -1;
  z = 0;
  while ((z < hashTable->entryTableCount) && ((*hashTableEntryTable) == NULL))
  {
    tableIndex = (hash+z)%hashTable->entryTableCount;
    i = findFreeEntryIndex(&hashTable->entryTables[tableIndex],
                           hash
                          );
    if (i >= 0)
    {
      (*hashTableEntryTable) = &hashTable->entryTables[tableIndex];
      (*index)               = i;
    }
    z++;
  }

  return ((*hashTableEntryTable) !=NULL);
}

/***********************************************************************\
* Name   : growTable
* Purpose: grow table size
* Input  : entries - current table entries
*          oldSize - old number of entries in table
*          newSize - new number of entries in table
* Output : -
* Return : new table entries or NULL on insufficient memory
* Notes  : -
\***********************************************************************/

LOCAL HashTableEntry *growTable(HashTableEntry *entries, uint oldSize, uint newSize)
{
  assert(entries != NULL);
  assert(newSize > oldSize);

  entries = realloc(entries,newSize*sizeof(HashTableEntry));
  if (entries != NULL)
  {
    memset(&entries[oldSize],0,(newSize-oldSize)*sizeof(HashTableEntry));
  }

  return entries;
}

/*---------------------------------------------------------------------*/

bool HashTable_init(HashTable *hashTable)
{
  assert(hashTable != NULL);

  hashTable->entryTables = (HashTableEntryTable*)malloc(sizeof(HashTableEntryTable)*1);
  if (hashTable->entryTables == NULL)
  {
    return FALSE;
  }
  hashTable->entryTableCount = 1;

  hashTable->entryTables[0].entries = (HashTableEntry*)calloc(TABLE_SIZES[0],sizeof(HashTableEntry));
  if (hashTable->entryTables[0].entries == NULL)
  {
    free(hashTable->entryTables);
    return FALSE;
  }
  hashTable->entryTables[0].sizeIndex  = 0;
  hashTable->entryTables[0].entryCount = 0;

  hashTable->hashTableCompareFunction = hashTableCompareFunction;
  hashTable->hashTableCompareUserData = hashTableCompareUserData;

  return TRUE;
}

void HashTable_done(HashTable             *hashTable,
                    HashTableFreeFunction hashTableFreeFunction,
                    void                  *hashTableFreeUserData
                   )
{
  uint z;
  uint index;

  assert(hashTable != NULL);
  assert(hashTable->entryTables != NULL);

  for (z = 0; z < hashTable->entryTableCount; z++)
  {
    assert(hashTable->entryTables[z].entries != NULL);

    for (index = 0; index < TABLE_SIZES[hashTable->entryTables[z].sizeIndex]; index++)
    {
      if (hashTable->entryTables[z].entries[index].data != NULL)
      {
        if (hashTableFreeFunction != NULL)
        {
          hashTableFreeFunction(hashTableFreeUserData,
                                hashTable->entryTables[z].entries[index].data,
                                hashTable->entryTables[z].entries[index].length
                               );
        }
        free(hashTable->entryTables[z].entries[index].data);
        free(hashTable->entryTables[z].entries[index].keyData);
      }
    }
    free(hashTable->entryTables[z].entries);
  }
  free(hashTable->entryTables);
}

void HashTable_clear(HashTable             *hashTable,
                     HashTableFreeFunction hashTableFreeFunction,
                     void                  *hashTableFreeUserData
                    )
{
  uint z;
  uint index;

  assert(hashTable != NULL);
  assert(hashTable->entryTables != NULL);

  for (z = 0; z < hashTable->entryTableCount; z++)
  {
    assert(hashTable->entryTables[z].entries != NULL);

    for (index = 0; index < TABLE_SIZES[hashTable->entryTables[z].sizeIndex]; index++)
    {
      if (hashTable->entryTables[z].entries[index].data != NULL)
      {
        if (hashTableFreeFunction != NULL)
        {
          hashTableFreeFunction(hashTableFreeUserData,
                                hashTable->entryTables[z].entries[index].data,
                                hashTable->entryTables[z].entries[index].length
                               );
        }
        free(hashTable->entryTables[z].entries[index].data);
        free(hashTable->entryTables[z].entries[index].keyData);

        hashTable->entryTables[z].entries[index].data = NULL;
      }
    }
  }
}

ulong HashTable_count(const HashTable *hashTable)
{
  ulong count;
  uint  z;

  assert(hashTable != NULL);
  assert(hashTable->entryTables != NULL);

  count = 0;
  for (z = 0; z < hashTable->entryTableCount; z++)
  {
    count += hashTable->entryTables[z].entryCount;
  }

  return count;
}

bool HashTable_add(HashTable *hashTable,
                   const void *keyData,
                   ulong      keyLength,
                   const void *data,
                   ulong      length
                  )
{
  ulong               hash;
  HashTableEntryTable *hashTableEntryTable;
  uint                entryIndex;
  void                *newData;
  uint                tableIndex;
  uint                newSizeIndex;
  uint                z,i;
  HashTableEntry      *newEntries;
  HashTableEntryTable *entryTables;

  assert(hashTable != NULL);

  hash = calculateHash(keyData,keyLength);

  /* update entry */
  if (findEntry(hashTable,hash,keyData,keyLength,&hashTableEntryTable,&entryIndex))
  {
    assert(hashTableEntryTable->entries != NULL);

    // allocate/resize data memory
    if (hashTableEntryTable->entries[entryIndex].length != length)
    {
      newData = realloc(hashTableEntryTable->entries[entryIndex].data,length);
      if (newData == NULL)
      {
        return FALSE;
      }
      hashTableEntryTable->entries[entryIndex].data   = newData;
      hashTableEntryTable->entries[entryIndex].length = length;
    }

    // copy data
    memcpy(hashTableEntryTable->entries[entryIndex].data,data,length);

    return TRUE;
  }

  /* add entry in existing table */
  if (findFreeEntry(hashTable,hash,&hashTableEntryTable,&entryIndex))
  {
    assert(hashTableEntryTable->entries != NULL);

    // allocate key memory
    hashTableEntryTable->entries[entryIndex].keyData = malloc(keyLength);
    if (hashTableEntryTable->entries[entryIndex].keyData == NULL)
    {
      return FALSE;
    }

    // allocate data memory
    hashTableEntryTable->entries[entryIndex].data = malloc(length);
    if (hashTableEntryTable->entries[entryIndex].data == NULL)
    {
      free(hashTableEntryTable->entries[entryIndex].keyData);
      return FALSE;
    }

    // copy key data
    hashTableEntryTable->entries[entryIndex].hash = hash;
    memcpy(hashTableEntryTable->entries[entryIndex].keyData,keyData,keyLength);
    hashTableEntryTable->entries[entryIndex].keyLength = keyLength;

    // copy data
    memcpy(hashTableEntryTable->entries[entryIndex].data,data,length);
    hashTableEntryTable->entries[entryIndex].length = length;

    hashTableEntryTable->entryCount++;

    return TRUE;
  }

  /* find a table which can be resized and where new entry can be
     stored in extended table, store entry in extended table
  */
  hashTableEntryTable = NULL;
  z = 0;
  while ((z < hashTable->entryTableCount) && (hashTableEntryTable == NULL))
  {
    assert(hashTable->entryTables != NULL);

    tableIndex = (hash+z)%hashTable->entryTableCount;
    newSizeIndex = hashTable->entryTables[tableIndex].sizeIndex+1;
    while ((newSizeIndex < SIZE_OF_ARRAY(TABLE_SIZES)) && (hashTableEntryTable == NULL))
    {
      #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_LINEAR_PROBING
        entryIndex = 0;
        i = 0;
        while ((i < LINEAR_PROBING_COUNT) && (entryIndex < TABLE_SIZES[hashTable->entryTables[tableIndex].sizeIndex]))
        {
          entryIndex = addModulo(hash,i,TABLE_SIZES[newSizeIndex]);
          i++;
        }
      #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_LINEAR_PROBING */
      #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING
        entryIndex = modulo(hash,TABLE_SIZES[newSizeIndex]);
        if (entryIndex < TABLE_SIZES[hashTable->entryTables[tableIndex].sizeIndex])
        {
          i = 0;
          while (i < QUADRATIC_PROBING_COUNT)
          {
            entryIndex = addModulo(hash,i*i,TABLE_SIZES[newSizeIndex]);
            if (entryIndex >= TABLE_SIZES[hashTable->entryTables[tableIndex].sizeIndex]) break;
            entryIndex = subModulo(hash,i*i,TABLE_SIZES[newSizeIndex]);
            if (entryIndex >= TABLE_SIZES[hashTable->entryTables[tableIndex].sizeIndex]) break;
            i++;
          }
        }
      #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_QUADRATIC_PROBING */
      #if COLLISION_ALGORITHM==COLLISION_ALGORITHM_REHASH
        entryIndex = 0;
        i = 0;
        while ((i < REHASHING_COUNT) && (entryIndex < TABLE_SIZES[hashTable->entryTables[tableIndex].sizeIndex]))
        {
          entryIndex = rotHash(hash,i)%TABLE_SIZES[newSizeIndex];
          i++;
        }
      #endif /* COLLISION_ALGORITHM==COLLISION_ALGORITHM_REHASH */
      if (entryIndex >= TABLE_SIZES[hashTable->entryTables[tableIndex].sizeIndex])
      {
//fprintf(stderr,"%s,%d: vor grow %p\n",__FILE__,__LINE__,hashTable->entryTables[tableIndex].entries);
        newEntries = growTable(hashTable->entryTables[tableIndex].entries,
                               TABLE_SIZES[hashTable->entryTables[tableIndex].sizeIndex],
                               TABLE_SIZES[newSizeIndex]
                              );
        if (newEntries != NULL)
        {
          hashTable->entryTables[tableIndex].entries   = newEntries;
          hashTable->entryTables[tableIndex].sizeIndex = newSizeIndex;

          hashTableEntryTable = &hashTable->entryTables[tableIndex];
        }
//fprintf(stderr,"%s,%d: nach grow %p\n",__FILE__,__LINE__,hashTable->entryTables[tableIndex].entries);
      }
      newSizeIndex++;
    }
    z++;
  }
  if (hashTableEntryTable != NULL)
  {
    assert(hashTableEntryTable->entries != NULL);

    // allocate key memory
    hashTableEntryTable->entries[entryIndex].keyData = malloc(keyLength);
    if (hashTableEntryTable->entries[entryIndex].keyData == NULL)
    {
      return FALSE;
    }

    // allocate data memory
    hashTableEntryTable->entries[entryIndex].data = malloc(length);
    if (hashTableEntryTable->entries[entryIndex].data == NULL)
    {
      free(hashTableEntryTable->entries[entryIndex].keyData);
      return FALSE;
    }

    // clopy key data
    hashTableEntryTable->entries[entryIndex].hash = hash;
    memcpy(hashTableEntryTable->entries[entryIndex].keyData,keyData,keyLength);
    hashTableEntryTable->entries[entryIndex].keyLength = keyLength;

    // copy data
    memcpy(hashTableEntryTable->entries[entryIndex].data,data,length);
    hashTableEntryTable->entries[entryIndex].length = length;

    hashTableEntryTable->entryCount++;

    return TRUE;
  }

  /* add new table and store entry in new table */
  newEntries = (HashTableEntry*)calloc(TABLE_SIZES[0],sizeof(HashTableEntry));
  if (newEntries == NULL)
  {
    return FALSE;
  }
  entryTables = (HashTableEntryTable*)realloc(hashTable->entryTables,(hashTable->entryTableCount+1)*sizeof(HashTableEntryTable));
  if (entryTables == NULL)
  {
    free(newEntries);
    return FALSE;
  }
  entryTables[hashTable->entryTableCount].entries    = newEntries;
  entryTables[hashTable->entryTableCount].sizeIndex  = 0;
  entryTables[hashTable->entryTableCount].entryCount = 0;
  hashTable->entryTables = entryTables;
  hashTable->entryTableCount++;

  hashTableEntryTable = &entryTables[hashTable->entryTableCount-1];
  entryIndex = rotHash(hash,0)%TABLE_SIZES[0];
  hashTableEntryTable->entries[entryIndex].keyData = malloc(keyLength);
  if (hashTableEntryTable->entries[entryIndex].keyData == NULL)
  {
    return FALSE;
  }
  hashTableEntryTable->entries[entryIndex].data = malloc(length);
  if (hashTableEntryTable->entries[entryIndex].data == NULL)
  {
    free(hashTableEntryTable->entries[entryIndex].keyData);
    return FALSE;
  }
  hashTableEntryTable->entries[entryIndex].hash = hash;
  memcpy(hashTableEntryTable->entries[entryIndex].keyData,keyData,keyLength);
  hashTableEntryTable->entries[entryIndex].keyLength = keyLength;
  memcpy(hashTableEntryTable->entries[entryIndex].data,data,length);
  hashTableEntryTable->entries[entryIndex].length = length;
  hashTableEntryTable->entryCount++;

  return TRUE;
}

void HashTable_rem(HashTable             *hashTable,
                   const void            *keyData,
                   ulong                 keyLength,
                   HashTableFreeFunction hashTableFreeFunction,
                   void                  *hashTableFreeUserData
                  )
{
  ulong               hash;
  HashTableEntryTable *hashTableEntryTable;
  uint                index;

  assert(hashTable != NULL);

  hash = calculateHash(keyData,keyLength);

  /* remove entry */
  if (findEntry(hashTable,hash,keyData,keyLength,&hashTableEntryTable,&index))
  {
    assert(hashTableEntryTable->entries != NULL);
    assert(hashTableEntryTable->entryCount > 0);

    if (hashTableFreeFunction != NULL)
    {
      hashTableFreeFunction(hashTableFreeUserData,
                            hashTableEntryTable->entries[index].data,
                            hashTableEntryTable->entries[index].length
                           );
    }
    free(hashTableEntryTable->entries[index].data);
    free(hashTableEntryTable->entries[index].keyData);

    hashTableEntryTable->entries[index].data      = NULL;
    hashTableEntryTable->entries[index].length    = 0;
    hashTableEntryTable->entries[index].keyData   = NULL;
    hashTableEntryTable->entries[index].keyLength = 0;

    hashTableEntryTable->entryCount--;
  }
}

bool HashTable_find(HashTable  *hashTable,
                    const void *keyData,
                    ulong      keyLength,
                    void       **data,
                    ulong      *length
                   )
{
  ulong               hash;
  HashTableEntryTable *hashTableEntryTable;
  uint                index;

  assert(hashTable != NULL);

  hash = calculateHash(keyData,keyLength);

  if (findEntry(hashTable,hash,keyData,keyLength,&hashTableEntryTable,&index))
  {
    assert(hashTableEntryTable->entries != NULL);

    if (data   != NULL) (*data)   = hashTableEntryTable->entries[index].data;
    if (length != NULL) (*length) = hashTableEntryTable->entries[index].length;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

bool HashTable_contain(HashTable  *hashTable,
                       const void *keyData,
                       ulong      keyLength
                      )
{
  assert(hashTable != NULL);

  return HashTable_find(hashTable,keyData,keyLength,NULL,NULL);
}

void HashTable_initIterator(HashTableIterator *hashTableIterator,
                            const HashTable   *hashTable
                           )
{
  assert(hashTableIterator != NULL);
  assert(hashTable != NULL);

  hashTableIterator->hashTable = hashTable;
  hashTableIterator->i          = 0;
  hashTableIterator->j          = 0;
}

void HashTable_doneIterator(HashTableIterator *hashTableIterator)
{
  assert(hashTableIterator != NULL);

  UNUSED_VARIABLE(hashTableIterator);
}

bool HashTable_getNext(HashTableIterator *hashTableIterator,
                       const void        **keyData,
                       ulong             *keyLength,
                       const void        **data,
                       ulong             *length
                      )
{
  bool           foundFlag;
  HashTableEntry *hashTableEntry;

  assert(hashTableIterator != NULL);

  if (keyData   != NULL) (*keyData)   = NULL;
  if (keyLength != NULL) (*keyLength) = 0;
  if (data      != NULL) (*data)      = NULL;
  if (length    != NULL) (*length)    = 0;

  foundFlag = FALSE;
  if (hashTableIterator->i < hashTableIterator->hashTable->entryTableCount)
  {
    assert(hashTableIterator->hashTable->entryTables != NULL);

    do
    {
      /* get entry */
      assert(hashTableIterator->hashTable->entryTables[hashTableIterator->i].entries != NULL);
      hashTableEntry = &hashTableIterator->hashTable->entryTables[hashTableIterator->i].entries[hashTableIterator->j];

      /* check if used/empty */
      if (hashTableEntry->data != NULL)
      {
        if (keyData   != NULL) (*keyData)   = hashTableEntry->keyData;
        if (keyLength != NULL) (*keyLength) = hashTableEntry->keyLength;
        if (data      != NULL) (*data)      = hashTableEntry->data;
        if (length    != NULL) (*length)    = hashTableEntry->length;
        foundFlag = TRUE;
      }

      /* next entry */
      if (hashTableIterator->j < TABLE_SIZES[hashTableIterator->hashTable->entryTables[hashTableIterator->i].sizeIndex]-1)
      {
        hashTableIterator->j++;
      }
      else
      {
        hashTableIterator->i++;
        hashTableIterator->j = 0;
      }
    }
    while (!foundFlag
           && (hashTableIterator->i < hashTableIterator->hashTable->entryTableCount)
          );
  }

  return foundFlag;
}

bool HashTable_iterate(HashTable                *hashTable,
                       HashTableIterateFunction hashTableIterateFunction,
                       void                     *hashTableIterateUserData
                      )
{
  HashTableIterator hashTableIterator;
  bool              okFlag;
  const void        *keyData;
  ulong             keyLength;
  const void        *data;
  ulong             length;

  assert(hashTable != NULL);
  assert(hashTableIterateFunction != NULL);

  okFlag = TRUE;
  HashTable_initIterator(&hashTableIterator,hashTable);
  while (   HashTable_getNext(&hashTableIterator,
                               &keyData,
                               &keyLength,
                               &data,
                               &length
                              )
         && okFlag
        )
  {
    okFlag = hashTableIterateFunction(hashTableIterateUserData,keyData,keyLength,data,length);
  }
  HashTable_doneIterator(&hashTableIterator);

  return okFlag;
}

#ifndef NDEBUG
void HashTable_printStatistic(const HashTable *hashTable)
{
  ulong totalEntryCount,totalIndexCount;
  uint  z;

  assert(hashTable != NULL);

  fprintf(stderr,"Hash table statistics:\n");
  fprintf(stderr,"  tables : %d\n",hashTable->entryTableCount);

  totalEntryCount = 0;
  totalIndexCount = 0;
  for (z = 0; z < hashTable->entryTableCount; z++)
  {
    fprintf(stderr,"    table #%02d: %u entries/%u size\n",z,hashTable->entryTables[z].entryCount,TABLE_SIZES[hashTable->entryTables[z].sizeIndex]);
    totalEntryCount += hashTable->entryTables[z].entryCount;
    totalIndexCount += TABLE_SIZES[hashTable->entryTables[z].sizeIndex];
  }

  fprintf(stderr,"  total entries: %lu\n",totalEntryCount);
  fprintf(stderr,"  total size:    %lu\n",totalIndexCount);
}
#endif /* NDEBUG */

#ifdef __cplusplus
  }
#endif

/* end of file */
