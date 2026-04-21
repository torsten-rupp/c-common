#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/bitsets.h"

CTEST(bitsets,init_done)
{
  BitSet bitSet;
  BitSet_init(&bitSet,100);
  BitSet_done(&bitSet);
}

CTEST(bitsets,new_delete)
{
  BitSet *bitSet = BitSet_new(100);
  ASSERT_NOT_NULL(bitSet);
  BitSet_delete(bitSet);
}

CTEST(bitsets,setAll)
{
  BitSet bitSet;
  BitSet_init(&bitSet,100);

  for (size_t i = 0; i < 100; i++)
  {
    BitSet_set(&bitSet,i,1);
  }

  for (size_t i = 0; i < 100; i++)
  {
    ASSERT_TRUE(BitSet_isSet(&bitSet,i));
    ASSERT_FALSE(BitSet_isCleared(&bitSet,i));
  }

  BitSet_done(&bitSet);
}

CTEST(bitsets,clearAll)
{
  BitSet bitSet;
  BitSet_init(&bitSet,100);

  for (size_t i = 0; i < 100; i++)
  {
    BitSet_set(&bitSet,i,1);
  }

  for (size_t i = 0; i < 100; i++)
  {
    ASSERT_TRUE(BitSet_isSet(&bitSet,i));
    ASSERT_FALSE(BitSet_isCleared(&bitSet,i));
  }
  
  BitSet_clearAll(&bitSet);

  for (size_t i = 0; i < 100; i++)
  {
    ASSERT_FALSE(BitSet_isSet(&bitSet,i));
    ASSERT_TRUE(BitSet_isCleared(&bitSet,i));
  }

  BitSet_done(&bitSet);
}

CTEST(bitsets,set_clear_isSet)
{
  BitSet bitSet;
  BitSet_init(&bitSet,100);

  ASSERT_FALSE(BitSet_isSet(&bitSet,5));
  BitSet_set(&bitSet,5,1);
  ASSERT_TRUE(BitSet_isSet(&bitSet,5));
  ASSERT_FALSE(BitSet_isCleared(&bitSet,5));
  BitSet_clear(&bitSet,5,1);
  ASSERT_FALSE(BitSet_isSet(&bitSet,5));
  ASSERT_TRUE(BitSet_isCleared(&bitSet,5));

  BitSet_clearAll(&bitSet);
  for (size_t i = 0; i < 100; i += 2)
  {
    BitSet_set(&bitSet,i,1);
  }
  for (size_t i = 0; i < 100; i += 2)
  {
    ASSERT_TRUE(BitSet_isSet(&bitSet,i));
  }
  for (size_t i = 1; i < 100; i += 2)
  {
    ASSERT_TRUE(BitSet_isCleared(&bitSet,i));
  }

  BitSet_clearAll(&bitSet);
  for (size_t i = 1; i < 100; i += 2)
  {
    BitSet_set(&bitSet,i,1);
  }
  for (size_t i = 1; i < 100; i += 2)
  {
    ASSERT_TRUE(BitSet_isSet(&bitSet,i));
  }
  for (size_t i = 0; i < 100; i += 2)
  {
    ASSERT_TRUE(BitSet_isCleared(&bitSet,i));
  }

  BitSet_done(&bitSet);
}

CTEST(bitsets,set_clear_range)
{
  BitSet bitSet;
  BitSet_init(&bitSet,100);

  BitSet_clearAll(&bitSet);
  BitSet_set(&bitSet,1,6);
  ASSERT_EQUAL(0x7E,bitSet.data[0]);

  BitSet_clearAll(&bitSet);
  BitSet_set(&bitSet,0,9);
  ASSERT_EQUAL(0xFF,bitSet.data[0]);
  ASSERT_EQUAL(0x01,bitSet.data[1]);

  BitSet_clearAll(&bitSet);
  BitSet_set(&bitSet,1,9);
  ASSERT_EQUAL(0xFE,bitSet.data[0]);
  ASSERT_EQUAL(0x03,bitSet.data[1]);

  BitSet_clearAll(&bitSet);
  BitSet_set(&bitSet,43,41);
  for (size_t i = 0; i < 43; i++)
  {
    ASSERT_FALSE(BitSet_isSet(&bitSet,i));
  }
  for (size_t i = 43; i < 43+41; i++)
  {
    ASSERT_TRUE(BitSet_isSet(&bitSet,i));
  }
  for (size_t i = 43+41; i < 100; i++)
  {
    ASSERT_FALSE(BitSet_isSet(&bitSet,i));
  }

  BitSet_clear(&bitSet,47,9);
  for (size_t i = 0; i < 43; i++)
  {
    ASSERT_FALSE(BitSet_isSet(&bitSet,i));
  }
  for (size_t i = 43; i < 47; i++)
  {
    ASSERT_TRUE(BitSet_isSet(&bitSet,i));
  }
  for (size_t i = 47; i < 47+9; i++)
  {
    ASSERT_FALSE(BitSet_isSet(&bitSet,i));
  }
  for (size_t i = 47+9; i < 43+41; i++)
  {
    ASSERT_TRUE(BitSet_isSet(&bitSet,i));
  }
  for (size_t i = 43+41; i < 100; i++)
  {
    ASSERT_FALSE(BitSet_isSet(&bitSet,i));
  }

  BitSet_done(&bitSet);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
