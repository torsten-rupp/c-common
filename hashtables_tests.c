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
  ASSERT_NOT_EQUAL(hashTable,NULL);
  HashTable_delete(hashTable);
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
                "test1",
                5,
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

CTEST(hashTables,interate)
{
  HashTable hashTable;
#if 0
  String         s;
  StringIterator stringIterator;
  uint           i;
  char           ch;
  Codepoint      codepoint;

  s = HashTable_new();

  HashTable_setCString(s,"test");
  ASSERT_STR("test",HashTable_cString(s));

  ASSERT_STR("aXaa",
             HashTable_cString(HashTable_iterate(s,
                                           CALLBACK_INLINE(const char*,(char ch, void *userData),
                                                           {
                                                             ASSERT_EQUAL(userData,NULL);

                                                             return (ch == 'e') ? "X" : "a";
                                                           },NULL
                                                          )
                                          )
                           )
            );

  HashTable_setCString(s,"test");

  stringIterator = HashTable_iterateBegin(s);
  i = 0;
  while (stringIterator < HashTable_iterateEnd(s))
  {
    ch = HashTable_iterateNext(s,&stringIterator);
    switch (i)
    {
      case 0: ASSERT_EQUAL(ch,'t'); break;
      case 1: ASSERT_EQUAL(ch,'e'); break;
      case 2: ASSERT_EQUAL(ch,'s'); break;
      case 3: ASSERT_EQUAL(ch,'t'); break;
    }
    i++;
  }

  stringIterator = HashTable_iterateBegin(s);
  i = 0;
  while (stringIterator < HashTable_iterateEnd(s))
  {
    codepoint = HashTable_iterateNextUTF8(s,&stringIterator);
    switch (i)
    {
      case 0: ASSERT_EQUAL(codepoint,'t'); break;
      case 1: ASSERT_EQUAL(codepoint,'e'); break;
      case 2: ASSERT_EQUAL(codepoint,'s'); break;
      case 3: ASSERT_EQUAL(codepoint,'t'); break;
    }
    i++;
  }

  HashTable_delete(s);
#endif
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
