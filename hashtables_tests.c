#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/hashtables.h"

CTEST(hashTables,init_done)
{
  HashTable hashTable;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_done(&hashTable);
}

CTEST(hashTables,new_delete)
{
  HashTable *hashTable;

  hashTable = HashTable_new(100,
                            CALLBACK_(NULL,NULL),
                            CALLBACK_(NULL,NULL),
                            CALLBACK_(NULL,NULL)
                           );
  ASSERT_NOT_NULL(hashTable);
  HashTable_delete(hashTable);
}

CTEST(hashTables,put)
{
  HashTable hashTable;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_put(&hashTable,
                "test",
                4,
                "data",
                4
               );
  ASSERT_FALSE(HashTable_isEmpty(&hashTable));
  HashTable_done(&hashTable);
}

CTEST(hashTables,remove)
{
  HashTable hashTable;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_put(&hashTable,
                "test",
                4,
                "data",
                4
               );
  ASSERT_FALSE(HashTable_isEmpty(&hashTable));
  HashTable_remove(&hashTable,
                   "test",
                   4
                  );
  ASSERT_TRUE(HashTable_isEmpty(&hashTable));
  HashTable_done(&hashTable);
}

CTEST(hashTables,isEmpty)
{
  HashTable hashTable;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  ASSERT_TRUE(HashTable_isEmpty(&hashTable));
  HashTable_done(&hashTable);
}

CTEST(hashTables,count)
{
  HashTable hashTable;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  ASSERT_EQUAL(HashTable_count(&hashTable),0);
  HashTable_put(&hashTable,
                "test",
                4,
                "data",
                4
               );
  ASSERT_EQUAL(HashTable_count(&hashTable),1);
  HashTable_done(&hashTable);
}


CTEST(hashTables,clear)
{
  HashTable hashTable;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_put(&hashTable,
                "test",
                4,
                "data",
                4
               );
  HashTable_put(&hashTable,
                "test2",
                5,
                "data",
                4
               );
  ASSERT_FALSE(HashTable_isEmpty(&hashTable));
  HashTable_clear(&hashTable);
  ASSERT_TRUE(HashTable_isEmpty(&hashTable));
  HashTable_done(&hashTable);
}

CTEST(hashTables,find)
{
  HashTable            hashTable;
  const HashTableEntry *hashTableEntry;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_put(&hashTable,
                "test",
                4,
                "data",
                4
               );
  HashTable_put(&hashTable,
                "test2",
                5,
                "data",
                4
               );

  hashTableEntry = HashTable_find(&hashTable,"test",4);
  ASSERT_NOT_NULL(hashTableEntry);
  ASSERT_TRUE(hashTableEntry->keyLength == 4);
  ASSERT_TRUE(memcmp(hashTableEntry->keyData,"test",4) == 0);
  ASSERT_TRUE(hashTableEntry->length == 4);
  ASSERT_TRUE(memcmp(hashTableEntry->data,"data",4) == 0);

  ASSERT_NULL(HashTable_find(&hashTable,"testX",5));
  HashTable_done(&hashTable);
}

CTEST(hashTables,contains)
{
  HashTable hashTable;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_put(&hashTable,
                "test",
                4,
                "data",
                4
               );
  HashTable_put(&hashTable,
                "test2",
                5,
                "data",
                4
               );
  ASSERT_TRUE(HashTable_contains(&hashTable,"test",4));
  ASSERT_TRUE(HashTable_contains(&hashTable,"test2",5));
  ASSERT_FALSE(HashTable_contains(&hashTable,"test2X",6));
  HashTable_done(&hashTable);
}

CTEST(hashTables,iterator_init_get_next_done)
{
  HashTable         hashTable;
  HashTableIterator hashTableIterator;
  bool              foundFlags[2];
  const void        *keyData,*data;
  ulong             keyLength,length;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_put(&hashTable,
                "test",
                4,
                "data",
                4
               );
  HashTable_put(&hashTable,
                "test2",
                5,
                "data2222",
                8
               );

  foundFlags[0] = FALSE;
  foundFlags[1] = FALSE;
  HashTable_initIterator(&hashTableIterator,
                         &hashTable
                        );
  ASSERT_TRUE(HashTable_getNext(&hashTableIterator,
                                &keyData,
                                &keyLength,
                                &data,
                                &length
                               )
             );
  if ((keyLength == 4) && (memcmp(keyData,"test",4) == 0)) foundFlags[0] = TRUE;
  if ((keyLength == 5) && (memcmp(keyData,"test2",5) == 0)) foundFlags[1] = TRUE;
  ASSERT_TRUE(HashTable_getNext(&hashTableIterator,
                                &keyData,
                                &keyLength,
                                &data,
                                &length
                               )
             );
  if ((keyLength == 4) && (memcmp(keyData,"test",4) == 0)) foundFlags[0] = TRUE;
  if ((keyLength == 5) && (memcmp(keyData,"test2",5) == 0)) foundFlags[1] = TRUE;
  ASSERT_FALSE(HashTable_getNext(&hashTableIterator,
                                 &keyData,
                                 &keyLength,
                                 &data,
                                 &length
                                )
             );
  ASSERT_TRUE(foundFlags[0]);
  ASSERT_TRUE(foundFlags[1]);
  HashTable_doneIterator(&hashTableIterator);

  HashTable_done(&hashTable);
}

CTEST(hashTables,iterate)
{
  HashTable hashTable;
  bool      foundFlags[2];

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_put(&hashTable,
                "test",
                4,
                "data",
                4
               );
  HashTable_put(&hashTable,
                "test2",
                5,
                "data2222",
                8
               );

  foundFlags[0] = FALSE;
  foundFlags[1] = FALSE;
  HashTable_iterate(&hashTable,
                    CALLBACK_INLINE(bool,(const void *keyData, ulong keyLength, const void *data, ulong length, void *userData),
                    {
                      UNUSED_VARIABLE(data);
                      UNUSED_VARIABLE(length);
                      UNUSED_VARIABLE(userData);

                      if ((keyLength == 4) && (memcmp(keyData,"test",4) == 0)) foundFlags[0] = TRUE;
                      if ((keyLength == 5) && (memcmp(keyData,"test2",5) == 0)) foundFlags[1] = TRUE;
                      
                      return TRUE;
                    },NULL)
                   );
  ASSERT_TRUE(foundFlags[0]);
  ASSERT_TRUE(foundFlags[1]);

  HashTable_done(&hashTable);
}

CTEST(hashTables,printStatistic)
{
  HashTable hashTable;

  HashTable_init(&hashTable,
                 100,
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL),
                 CALLBACK_(NULL,NULL)
                );
  HashTable_printStatistic(&hashTable);
  HashTable_done(&hashTable);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
