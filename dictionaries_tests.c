#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/global.h"
#include "common/dictionaries.h"

#define TEST_SIZE 5000

#define KEY_BASE   0
#define VALUE_BASE TEST_SIZE

int keys  [TEST_SIZE];
int values[TEST_SIZE];

// ---------------------------------------------------------------------

CTEST(dictionaries,initDoneReference)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL)
                             )
             );

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,addReference)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL)
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_add(&dictionary,
                   &keys[i],
                   sizeof(keys[i]),
                   &values[i],
                   sizeof(values[i])
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));
  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_add(&dictionary,
                   &keys[i],
                   sizeof(keys[i]),
                   &values[i],
                   sizeof(values[i])
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int *dataReference;
    ASSERT_TRUE(Dictionary_find(&dictionary,
                                &keys[i],sizeof(keys[i]),
                                (void**)&dataReference,NULL
                               )
               );
    ASSERT_EQUAL(values[i],*dataReference);
  }

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,removeReference)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL)
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_add(&dictionary,
                   &keys[i],
                   sizeof(keys[i]),
                   &values[i],
                   sizeof(values[i])
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_remove(&dictionary,
                      &keys[i],
                      sizeof(keys[i])
                     );
    ASSERT_EQUAL(TEST_SIZE-i-1,Dictionary_count(&dictionary));
  }

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,clearReference)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL)
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_add(&dictionary,
                   &keys[i],
                   sizeof(keys[i]),
                   &values[i],
                   sizeof(values[i])
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  Dictionary_clear(&dictionary);
  ASSERT_EQUAL(0,Dictionary_count(&dictionary));

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,iteratorReference)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL)
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_add(&dictionary,
                   &keys[i],
                   sizeof(keys[i]),
                   &values[i],
                   sizeof(values[i])
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  DictionaryIterator dictionaryIterator;
  Dictionary_initIterator(&dictionaryIterator,&dictionary);
  const int *keyReference;
  int       *dataReference;
  while (Dictionary_getNext(&dictionaryIterator,
                            (const void**)&keyReference,NULL,
                            (void**)&dataReference,NULL
                           )
        )
  {
    ASSERT_NOT_NULL(keyReference);
    ASSERT_NOT_NULL(dataReference);
  }
  Dictionary_doneIterator(&dictionaryIterator);

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,containsReference)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL),
                              CALLBACK_(NULL,NULL)
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_add(&dictionary,
                   &keys[i],
                   sizeof(keys[i]),
                   &values[i],
                   sizeof(values[i])
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    ASSERT_TRUE(Dictionary_contains(&dictionary,
                                    &keys[i],
                                    sizeof(keys[i])
                                   )
               );
  }
  int unknownKey = 99999;
  ASSERT_FALSE(Dictionary_contains(&dictionary,
                                   &unknownKey,
                                   sizeof(unknownKey)
                                  )
             );

  Dictionary_done(&dictionary);
}

// ---------------------------------------------------------------------

CTEST(dictionaries,initDoneCopy)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_BYTE_INIT_ENTRY,
                              DICTIONARY_BYTE_DONE_ENTRY,
                              DICTIONARY_BYTE_COMPARE_ENTRY
                             )
             );

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,addCopy)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_BYTE_INIT_ENTRY,
                              DICTIONARY_BYTE_DONE_ENTRY,
                              DICTIONARY_BYTE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int key  = i;
    int data = i;
    Dictionary_add(&dictionary,
                   &key,
                   sizeof(key),
                   &data,
                   sizeof(data)
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));
  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int key  = i;
    int data = i;
    Dictionary_add(&dictionary,
                   &key,
                   sizeof(key),
                   &data,
                   sizeof(data)
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int key = i;
    int *dataReference;
    ASSERT_TRUE(Dictionary_find(&dictionary,
                                &key,sizeof(key),
                                (void**)&dataReference,NULL
                               )
               );
    ASSERT_EQUAL(i,*dataReference);
  }

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,removeCopy)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_BYTE_INIT_ENTRY,
                              DICTIONARY_BYTE_DONE_ENTRY,
                              DICTIONARY_BYTE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int  key  = i;
    int  data = i;
    Dictionary_add(&dictionary,
                   &key,
                   sizeof(key),
                   &data,
                   sizeof(data)
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int  key = i;
    Dictionary_remove(&dictionary,
                      &key,
                      sizeof(key)
                     );
    ASSERT_EQUAL(TEST_SIZE-i-1,Dictionary_count(&dictionary));
  }

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,clearCopy)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_BYTE_INIT_ENTRY,
                              DICTIONARY_BYTE_DONE_ENTRY,
                              DICTIONARY_BYTE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int key  = i;
    int data = i;
    Dictionary_add(&dictionary,
                   &key,
                   sizeof(key),
                   &data,
                   sizeof(data)
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  Dictionary_clear(&dictionary);
  ASSERT_EQUAL(0,Dictionary_count(&dictionary));

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,iteratorCopy)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_BYTE_INIT_ENTRY,
                              DICTIONARY_BYTE_DONE_ENTRY,
                              DICTIONARY_BYTE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int key  = i;
    int data = i;
    Dictionary_add(&dictionary,
                   &key,
                   sizeof(key),
                   &data,
                   sizeof(data)
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  DictionaryIterator dictionaryIterator;
  Dictionary_initIterator(&dictionaryIterator,&dictionary);
  const int *keyReference;
  int       *dataReference;
  while (Dictionary_getNext(&dictionaryIterator,
                            (const void**)&keyReference,NULL,
                            (void**)&dataReference,NULL
                           )
        )
  {
    ASSERT_NOT_NULL(keyReference);
    ASSERT_NOT_NULL(dataReference);
  }
  Dictionary_doneIterator(&dictionaryIterator);

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,containsCopy)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_BYTE_INIT_ENTRY,
                              DICTIONARY_BYTE_DONE_ENTRY,
                              DICTIONARY_BYTE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int key  = i;
    int data = i;
    Dictionary_add(&dictionary,
                   &key,
                   sizeof(key),
                   &data,
                   sizeof(data)
                  );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    int key = i;
    ASSERT_TRUE(Dictionary_contains(&dictionary,
                                    &key,
                                    sizeof(key)
                                   )
               );
  }
  int key = 99999;
  ASSERT_FALSE(Dictionary_contains(&dictionary,
                                   &key,
                                   sizeof(key)
                                  )
             );

  Dictionary_done(&dictionary);
}

// ---------------------------------------------------------------------

CTEST(dictionaries,initDoneValue)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_initValue(&dictionary));

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,addValue)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_VALUE_INIT_ENTRY,
                              DICTIONARY_VALUE_DONE_ENTRY,
                              DICTIONARY_VALUE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_addValue(&dictionary,
                        keys[i],
                        values[i]
                       );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));
  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_addValue(&dictionary,
                        keys[i],
                        values[i]
                       );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    intptr_t value;
    ASSERT_TRUE(Dictionary_findValue(&dictionary,
                                     keys[i],
                                     &value
                                    )
               );
    ASSERT_EQUAL(values[i],value);
  }

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,removeValue)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_VALUE_INIT_ENTRY,
                              DICTIONARY_VALUE_DONE_ENTRY,
                              DICTIONARY_VALUE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_addValue(&dictionary,
                        keys[i],
                        values[i]
                       );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_removeValue(&dictionary,
                           keys[i]
                          );
    ASSERT_EQUAL(TEST_SIZE-i-1,Dictionary_count(&dictionary));
  }

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,clearValue)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_VALUE_INIT_ENTRY,
                              DICTIONARY_VALUE_DONE_ENTRY,
                              DICTIONARY_VALUE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_addValue(&dictionary,
                        keys[i],
                        values[i]
                       );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  Dictionary_clear(&dictionary);
  ASSERT_EQUAL(0,Dictionary_count(&dictionary));

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,iteratorValue)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_VALUE_INIT_ENTRY,
                              DICTIONARY_VALUE_DONE_ENTRY,
                              DICTIONARY_VALUE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_addValue(&dictionary,
                        keys[i],
                        values[i]
                       );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  DictionaryIterator dictionaryIterator;
  Dictionary_initIterator(&dictionaryIterator,&dictionary);
  intptr_t key;
  intptr_t value;
  while (Dictionary_getNextValue(&dictionaryIterator,
                                 &key,
                                 &value
                                )
        )
  {
    ASSERT_TRUE(key >= 0);
    ASSERT_TRUE(key < TEST_SIZE);
    ASSERT_TRUE(key >= 0);
    ASSERT_TRUE(key < TEST_SIZE);
  }
  Dictionary_doneIterator(&dictionaryIterator);

  Dictionary_done(&dictionary);
}

CTEST(dictionaries,containsValue)
{
  Dictionary dictionary;
  ASSERT_TRUE(Dictionary_init(&dictionary,
                              DICTIONARY_VALUE_INIT_ENTRY,
                              DICTIONARY_VALUE_DONE_ENTRY,
                              DICTIONARY_VALUE_COMPARE_ENTRY
                             )
             );

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    Dictionary_addValue(&dictionary,
                        keys[i],
                        values[i]
                       );
  }
  ASSERT_EQUAL(TEST_SIZE,Dictionary_count(&dictionary));

  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    ASSERT_TRUE(Dictionary_containsValue(&dictionary,
                                         keys[i]
                                        )
               );
  }
  ASSERT_FALSE(Dictionary_containsValue(&dictionary,
                                        99999
                                       )
             );

  Dictionary_done(&dictionary);
}

// ---------------------------------------------------------------------

int main(int argc, const char *argv[])
{
  for (size_t i = 0; i < TEST_SIZE; i++)
  {
    keys[i]   = KEY_BASE   + i;
    values[i] = VALUE_BASE + i;
  }

  return ctest_main(argc, argv);
}
