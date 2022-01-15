/***********************************************************************\
*
* $Source: /home/torsten/cvs/bar/bar/dictionaries.h,v $
* $Revision$
* $Author$
* Contents: hash table functions
* Systems: all
*
\***********************************************************************/

#ifndef __DICTIONARIES__
#define __DICTIONARIES__

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "global.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

// hash entry
typedef struct
{
  ulong hash;                                // hash code
  void  *data;                               // data of entry
  ulong length;                              // length of data in entry
} HashTableEntry;

// table with hash entries
typedef struct
{
  HashTableEntry *entries;                   // entries array
  uint           sizeIndex;                  // array size index (see TABLE_SIZES)
  uint           entryCount;                 // number of entries in array
} HashTableEntryTable;

// hash
typedef struct
{
  HashTableEntryTable *entryTables;          // tables array
  uint                entryTableCount;       // number of tables
} HashTable;

typedef struct
{
  const HashTable *hashTable;
  uint             i,j;
} HashTableIterator;

/* delete hash entry function */
typedef void(*HashTableFreeFunction)(void *userData, const void *data, ulong length);

/* iterator function */
typedef bool(*HashTableIterateFunction)(void *userData, const void *keyData, ulong keyLength, const void *data, ulong length);

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

/***********************************************************************\
* Name   : HashTable_init
* Purpose: initialize hash table
* Input  : hashTable - hash table variable
* Output : -
* Return : TRUE if hash table initialized, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool HashTable_init(HashTable *hashTable);

/***********************************************************************\
* Name   : HashTable_done
* Purpose: deinitialize hash table
* Input  : hashTable             - hash table
*          hashTableFreeFunction - hash table entry free function or
*                                  NULL
*          hashTableFreeUserData - hash table entry free function user
*                                  data
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

void HashTable_done(HashTable             *hashTable,
                    HashTableFreeFunction hashTableFreeFunction,
                    void                  *hashTableFreeUserData
                   );

/***********************************************************************\
* Name   : HashTable_clear
* Purpose: clear hash table
* Input  : hashTable             - hash table
*          hashTableFreeFunction - hash table entry free function or
*                                  NULL
*          hashTableFreeUserData - hash table entry free function user
*                                  data
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

void HashTable_clear(HashTable             *hashTable,
                     HashTableFreeFunction hashTableFreeFunction,
                     void                  *hashTableFreeUserData
                    );

/***********************************************************************\
* Name   : HashTable_count
* Purpose: get number of entries in hash table
* Input  : hashTable - hash table
* Output : -
* Return : number of entries in hash table
* Notes  : -
\***********************************************************************/

ulong HashTable_count(const HashTable *hashTable);

/***********************************************************************\
* Name   : HashTable_add
* Purpose: add entry to hash table
* Input  : hashTable - hash table
*          key       - key value
*          data      - entry data
*          length    - length of entry data
* Output : -
* Return : TRUE if entry added, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool HashTable_add(HashTable  *hashTable,
                   ulong      key,
                   const void *data,
                   ulong      length
                  );

/***********************************************************************\
* Name   : HashTable_rem
* Purpose: remove entry from hash table
* Input  : hashTable             - hash table
*          key                   - key value
*          hashTableFreeFunction - hash table entry free function or
*                                  NULL
*          hashTableFreeUserData - hash table entry free function user
*                                  data
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

void HashTable_rem(HashTable             *hashTable,
                   ulong                 key,
                   HashTableFreeFunction hashTableFreeFunction,
                   void                  *hashTableFreeUserData
                  );

/***********************************************************************\
* Name   : HashTable_find
* Purpose: find entry in hash table
* Input  : hashTable - hash table
*          key       - key value
* Output : data   - entry data iff entry found (can be NULL)
*          length - length of data (can be NULL)
* Return : TRUE if entry found, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool HashTable_find(HashTable *hashTable,
                    ulong      key,
                    void       **data,
                    ulong      *length
                   );

/***********************************************************************\
* Name   : HashTable_contain
* Purpose: check if entry is in hash table
* Input  : hashTable - hash table
*          key       - key value
* Output : -
* Return : TRUE if entry is in hash table, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool HashTable_contain(HashTable *hashTable,
                       ulong     key
                      );

/***********************************************************************\
* Name   : HashTable_initIterator
* Purpose: init hash table iterator
* Input  : hashTableIterator - iterator variable
*          hashTable         - hash table
* Output : hashTableIterator - initialized iterator variable
* Return : -
* Notes  : -
\***********************************************************************/

void HashTable_initIterator(HashTableIterator *hashTableIterator,
                            const HashTable   *hashTable
                           );

/***********************************************************************\
* Name   : HashTable_getNext
* Purpose: deinit hash table iterator
* Input  : hashTableIterator - hash table iterator
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

void HashTable_doneIterator(HashTableIterator *hashTableIterator);

/***********************************************************************\
* Name   : HashTable_getNext
* Purpose: get next entry from hash table
* Input  : hashTable - hash table
* Output : key       - key value
*          data      - entry data (can be NULL)
*          length    - length of data (can be NULL)
* Return : TRUE if got entry, FALSE if no more entries
* Notes  : -
\***********************************************************************/

bool HashTable_getNext(HashTableIterator *hashTableIterator,
                       ulong              *key,
                       const void         **data,
                       ulong              *length
                      );

/***********************************************************************\
* Name   : HashTable_iterate
* Purpose: iterate over hash table
* Input  : hashTable                - hash table
*          hashTableIterateFunction - iterator function
*          hashTableIterateUserData - iterator function user data
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

bool HashTable_iterate(HashTable                *hashTable,
                       HashTableIterateFunction hashTableIterateFunction,
                       void                      *hashTableIterateUserData
                      );

#ifndef NDEBUG
void HashTable_printStatistic(const HashTable *hashTable);
#endif /* NDEBUG */

#ifdef __cplusplus
  }
#endif

#endif /* __DICTIONARIES__ */

/* end of file */
