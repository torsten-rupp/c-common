#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/arrays.h"

CTEST(arrays,init_done)
{
  Array array;
  Array_init(&array,
             4,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_done(&array);
}

CTEST(arrays,new_delete)
{
  Array *array = Array_new(4,
                           100,
                           CALLBACK_(NULL,NULL),
                           CALLBACK_(NULL,NULL)
                          );
  ASSERT_NOT_NULL(array);
  Array_delete(array);
}

CTEST(arrays,isEmpty)
{
  Array array;
  Array_init(&array,
             4,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  ASSERT_TRUE(Array_isEmpty(&array));
  Array_done(&array);
}

CTEST(arrays,length)
{
  Array array;
  Array_init(&array,
             4,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  ASSERT_EQUAL(Array_length(&array),0);
  Array_put(&array,
            0,
            "data"
           );
  ASSERT_EQUAL(Array_length(&array),1);
  Array_done(&array);
}

CTEST(arrays,clear)
{
  Array array;
  Array_init(&array,
             5,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data1"
           );
  Array_put(&array,
            1,
            "data2"
           );
  ASSERT_FALSE(Array_isEmpty(&array));
  Array_clear(&array);
  ASSERT_TRUE(Array_isEmpty(&array));
  Array_done(&array);
}

CTEST(arrays,put_get)
{
  Array array;
  Array_init(&array,
             4,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  ASSERT_TRUE(Array_put(&array,
                        0,
                        "data"
                       )
             );
  ASSERT_FALSE(Array_isEmpty(&array));
  char data[4];
  Array_get(&array,0,data);
  ASSERT_EQUAL(memcmp(data,"data",4),0);
  Array_done(&array);
}

CTEST(arrays,insert)
{
  Array array;
  Array_init(&array,
             5,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data1"
           );
  ASSERT_EQUAL(Array_length(&array),1);
  ASSERT_EQUAL(memcmp(Array_get(&array,0,NULL),"data1",5),0);
  Array_insert(&array,
               0,
               "data2"
              );
  ASSERT_EQUAL(Array_length(&array),2);
  ASSERT_EQUAL(memcmp(Array_get(&array,1,NULL),"data1",5),0);
  ASSERT_EQUAL(memcmp(Array_get(&array,0,NULL),"data2",5),0);
  Array_done(&array);
}

CTEST(arrays,append)
{
  Array array;
  Array_init(&array,
             5,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data1"
           );
  ASSERT_EQUAL(Array_length(&array),1);
  ASSERT_EQUAL(memcmp(Array_get(&array,0,NULL),"data1",5),0);
  Array_append(&array,
               "data2"
              );
  ASSERT_EQUAL(Array_length(&array),2);
  ASSERT_EQUAL(memcmp(Array_get(&array,0,NULL),"data1",5),0);
  ASSERT_EQUAL(memcmp(Array_get(&array,1,NULL),"data2",5),0);
  Array_done(&array);
}

CTEST(arrays,remove)
{
  Array array;
  Array_init(&array,
             4,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data"
           );
  ASSERT_FALSE(Array_isEmpty(&array));
  Array_remove(&array,
               0
              );
  ASSERT_TRUE(Array_isEmpty(&array));
  Array_done(&array);
}

CTEST(arrays,removeAll)
{
  Array array;
  Array_init(&array,
             4,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data"
           );
  Array_put(&array,
            1,
            "data"
           );
  ASSERT_FALSE(Array_isEmpty(&array));
  Array_removeAll(&array,"data",CALLBACK_(NULL,NULL));
  ASSERT_TRUE(Array_isEmpty(&array));
  Array_done(&array);
}

CTEST(arrays,contains)
{
  Array array;
  Array_init(&array,
             5,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data1"
           );
  Array_put(&array,
            1,
            "data2"
           );
  ASSERT_TRUE(Array_contains(&array,"data1"));
  ASSERT_TRUE(Array_contains(&array,"data2"));
  ASSERT_FALSE(Array_contains(&array,"dataX"));
  Array_done(&array);
}

CTEST(arrays,find)
{
  Array array;
  Array_init(&array,
             5,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data1"
           );
  Array_put(&array,
            1,
            "data2"
           );

  long index;
  index = Array_find(&array,ARRAY_FIND_FORWARD,"data1",CALLBACK_(NULL,NULL));
  ASSERT_NOT_EQUAL(index,-1);
  index = Array_find(&array,ARRAY_FIND_BACKWARD,"data1",CALLBACK_(NULL,NULL));
  ASSERT_NOT_EQUAL(index,-1);
  index = Array_find(&array,ARRAY_FIND_BACKWARD,"dataX",CALLBACK_(NULL,NULL));
  ASSERT_EQUAL(index,-1);

  Array_done(&array);
}

CTEST(arrays,findNext)
{
  Array array;
  Array_init(&array,
             4,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data"
           );
  Array_put(&array,
            1,
            "data"
           );

  long index;
  index = Array_find(&array,ARRAY_FIND_FORWARD,"data",CALLBACK_(NULL,NULL));
  ASSERT_EQUAL(index,0);
  index = Array_findNext(&array,ARRAY_FIND_FORWARD,"data",index,CALLBACK_(NULL,NULL));
  ASSERT_EQUAL(index,1);

  Array_done(&array);
}

CTEST(arrays,cArray)
{
  Array array;
  Array_init(&array,
             5,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data1"
           );
  Array_put(&array,
            1,
            "data2"
           );

  const void *cArray = Array_cArray(&array);
  ASSERT_EQUAL(memcmp(cArray,"data1data2",10),0);

  Array_done(&array);
}

CTEST(arrays,iterate)
{
  Array array;
  Array_init(&array,
             5,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data1"
           );
  Array_put(&array,
            1,
            "data2"
           );

  bool foundFlags[2] = { FALSE, FALSE };
  ArrayIterator arrayIterator;
  char data[5];
  ARRAY_ITERATE(&array,arrayIterator,data)
  {
    assert(data != NULL);

    if ((memcmp(data,"data1",5) == 0)) foundFlags[0] = TRUE;
    if ((memcmp(data,"data2",5) == 0)) foundFlags[1] = TRUE;
  };
  ASSERT_TRUE(foundFlags[0]);
  ASSERT_TRUE(foundFlags[1]);

  Array_done(&array);
}

CTEST(arrays,iterateX)
{
  Array array;
  Array_init(&array,
             5,
             100,
             CALLBACK_(NULL,NULL),
             CALLBACK_(NULL,NULL)
            );
  Array_put(&array,
            0,
            "data1"
           );
  Array_put(&array,
            1,
            "data2"
           );

  bool foundFlags[2] = { FALSE, FALSE };
  ArrayIterator arrayIterator;
  char data[5];
  bool condition = TRUE;
  ARRAY_ITERATEX(&array,arrayIterator,data,condition)
  {
    assert(data != NULL);

    if ((memcmp(data,"data1",5) == 0)) foundFlags[0] = TRUE;
    if ((memcmp(data,"data2",5) == 0)) foundFlags[1] = TRUE;

    condition = FALSE;
  };
  ASSERT_TRUE(foundFlags[0]);
  ASSERT_FALSE(foundFlags[1]);

  Array_done(&array);
}

#ifndef NDEBUG
CTEST(arrays,printStatistic)
{
  #ifndef NDEBUG
    Array array;
    Array_init(&array,
               4,
               100,
               CALLBACK_(NULL,NULL),
               CALLBACK_(NULL,NULL)
              );
    Array_debugPrintStatistics();
    Array_done(&array);
  #endif
}
#endif // not NDEBUG

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
