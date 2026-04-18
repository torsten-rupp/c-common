#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/global.h"

#include "common/ringbuffers.h"

CTEST(ringBuffer,init_done)
{
  RingBuffer ringBuffer;
  ASSERT_TRUE(RingBuffer_init(&ringBuffer, 1, 64));
  RingBuffer_done(&ringBuffer, NULL, NULL);
  ASSERT_TRUE(RingBuffer_init(&ringBuffer, sizeof(int), 8));
  RingBuffer_done(&ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,new_delete)
{
  RingBuffer *ringBuffer;
  ringBuffer = RingBuffer_new(1, 64);
  ASSERT_TRUE(ringBuffer != NULL);
  RingBuffer_delete(ringBuffer, NULL, NULL);
  ringBuffer = RingBuffer_new(sizeof(int), 8);
  ASSERT_TRUE(ringBuffer != NULL);
  RingBuffer_delete(ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,get_put)
{
  RingBuffer ringBuffer;

  // single element
  RingBuffer_init(&ringBuffer, sizeof(int), 8);

  ASSERT_TRUE(RingBuffer_get(&ringBuffer, NULL, 1) == NULL);

  int inputValue  = 42;
  int outputValue = 0;
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, &inputValue, 1));
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 1);
  ASSERT_EQUAL(RingBuffer_getFree(&ringBuffer), 7);
  ASSERT_FALSE(RingBuffer_isEmpty(&ringBuffer));

  void *result1 = RingBuffer_get(&ringBuffer, &outputValue, 1);
  ASSERT_TRUE(result1 != NULL);
  ASSERT_EQUAL(outputValue, 42);
  ASSERT_TRUE(RingBuffer_isEmpty(&ringBuffer));
  ASSERT_EQUAL(RingBuffer_getFree(&ringBuffer), 8);

  RingBuffer_done(&ringBuffer, NULL, NULL);

  // multiple elements
  RingBuffer_init(&ringBuffer, sizeof(int), 8);

  int inputValues1[] = { 10, 11, 12, 13, 14 };
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, inputValues1, 5));
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 5);
  ASSERT_EQUAL(RingBuffer_getFree(&ringBuffer), 3);

  int outputValues[5];
  ASSERT_TRUE(RingBuffer_get(&ringBuffer, outputValues, 5) != NULL);
  ASSERT_EQUAL(outputValues[0], 10);
  ASSERT_EQUAL(outputValues[1], 11);
  ASSERT_EQUAL(outputValues[2], 12);
  ASSERT_EQUAL(outputValues[3], 13);
  ASSERT_EQUAL(outputValues[4], 14);
  ASSERT_TRUE(RingBuffer_isEmpty(&ringBuffer));

  RingBuffer_done(&ringBuffer, NULL, NULL);

  // wrap around
  RingBuffer_init(&ringBuffer, sizeof(int), 8);

  int firstBatch[] = { 1, 2, 3, 4};
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, firstBatch, 4));

  int drainedValues[2];
  ASSERT_TRUE(RingBuffer_get(&ringBuffer, drainedValues, 2) != NULL);
  ASSERT_EQUAL(drainedValues[0], 1);
  ASSERT_EQUAL(drainedValues[1], 2);

  int secondBatch[] = { 10, 11 };
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, secondBatch, 2));
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 4);

  ASSERT_TRUE(RingBuffer_get(&ringBuffer, outputValues, 4) != NULL);
  ASSERT_EQUAL(outputValues[0], 3);
  ASSERT_EQUAL(outputValues[1], 4);
  ASSERT_EQUAL(outputValues[2], 10);
  ASSERT_EQUAL(outputValues[3], 11);

  RingBuffer_done(&ringBuffer, NULL, NULL);

  // pointer into buffer
  RingBuffer_init(&ringBuffer, sizeof(int), 8);

  int inputValues2[] = { 7, 8 };
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, inputValues2, 2));

  int *result2 = (int*)RingBuffer_get(&ringBuffer, NULL, 2);
  ASSERT_TRUE(result2 != NULL);
  ASSERT_EQUAL(result2[0], 7);
  ASSERT_EQUAL(result2[1], 8);

  RingBuffer_done(&ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,first)
{
  RingBuffer ringBuffer;

  // non empty
  RingBuffer_init(&ringBuffer, sizeof(int), 4);

  int inputValues[] = {5, 6, 7};
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, inputValues, 3));

  int peekedValue1;
  void *result1 = RingBuffer_first(&ringBuffer, &peekedValue1);
  ASSERT_TRUE(result1 != NULL);
  ASSERT_EQUAL(peekedValue1, 5);
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 3);

  /* Peek with NULL returns pointer */
  int *result2 = (int*)RingBuffer_first(&ringBuffer, NULL);
  ASSERT_TRUE(result2 != NULL);
  ASSERT_EQUAL(*result2, 5);

  RingBuffer_done(&ringBuffer, NULL, NULL);

  // empty
  RingBuffer_init(&ringBuffer, sizeof(int), 4);

  int peekedValue2;
  void *result3 = RingBuffer_first(&ringBuffer, &peekedValue2);
  ASSERT_TRUE(result3 == NULL);

  RingBuffer_done(&ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,clear)
{
  RingBuffer ringBuffer;

  RingBuffer_init(&ringBuffer, sizeof(int), 8);

  int inputValues[4] = { 1, 2, 3, 4 };
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, inputValues, 4));
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 4);

  RingBuffer_clear(&ringBuffer, NULL, NULL);
  ASSERT_TRUE(RingBuffer_isEmpty(&ringBuffer));
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 0);
  ASSERT_EQUAL(RingBuffer_getFree(&ringBuffer), 8);

  RingBuffer_done(&ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,resize)
{
  RingBuffer ringBuffer;

  // grow
  RingBuffer_init(&ringBuffer, sizeof(int), 4);

  int inputValues1[] = { 1, 2, 3, 4};
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, inputValues1, 4));

  ASSERT_TRUE(RingBuffer_resize(&ringBuffer, 8));
  ASSERT_EQUAL(RingBuffer_getSize(&ringBuffer), 8);
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 4);

  int result1[4];
  ASSERT_TRUE(RingBuffer_get(&ringBuffer, result1, 4) != NULL);
  ASSERT_EQUAL(result1[0], 1);
  ASSERT_EQUAL(result1[1], 2);
  ASSERT_EQUAL(result1[2], 3);
  ASSERT_EQUAL(result1[3], 4);

  RingBuffer_done(&ringBuffer, NULL, NULL);

  // shrink
  RingBuffer_init(&ringBuffer, sizeof(int), 8);

  int inputValues2[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
  ASSERT_TRUE(RingBuffer_put(&ringBuffer, inputValues2, 8));

  ASSERT_TRUE(RingBuffer_resize(&ringBuffer, 4));
  ASSERT_EQUAL(RingBuffer_getSize(&ringBuffer), 4);
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 4);

  int result2[4];
  ASSERT_TRUE(RingBuffer_get(&ringBuffer, result2, 4) != NULL);
  ASSERT_EQUAL(result2[0], 1);
  ASSERT_EQUAL(result2[1], 2);
  ASSERT_EQUAL(result2[2], 3);
  ASSERT_EQUAL(result2[3], 4);

  RingBuffer_done(&ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,move)
{
  RingBuffer sourceRingBuffer, destinationRingBuffer;

  // contiguous source
  RingBuffer_init(&sourceRingBuffer,      sizeof(int), 8);
  RingBuffer_init(&destinationRingBuffer, sizeof(int), 8);

  int inputValues1[] = { 1, 2, 3, 4 };
  ASSERT_TRUE(RingBuffer_put(&sourceRingBuffer, inputValues1, 4));

  ASSERT_EQUAL(RingBuffer_getAvailable(&sourceRingBuffer), 4);
  ASSERT_TRUE(RingBuffer_isEmpty(&destinationRingBuffer));
  ASSERT_TRUE(RingBuffer_move(&sourceRingBuffer, &destinationRingBuffer, 4));
  ASSERT_TRUE(RingBuffer_isEmpty(&sourceRingBuffer));
  ASSERT_EQUAL(RingBuffer_getAvailable(&destinationRingBuffer), 4);

  int outputValues1[4];
  ASSERT_TRUE(RingBuffer_get(&destinationRingBuffer, outputValues1, 4) != NULL);
  ASSERT_EQUAL(outputValues1[0], 1);
  ASSERT_EQUAL(outputValues1[1], 2);
  ASSERT_EQUAL(outputValues1[2], 3);
  ASSERT_EQUAL(outputValues1[3], 4);

  RingBuffer_done(&sourceRingBuffer,      NULL, NULL);
  RingBuffer_done(&destinationRingBuffer, NULL, NULL);

  // wrapped source
  RingBuffer_init(&sourceRingBuffer,      sizeof(int), 6);
  RingBuffer_init(&destinationRingBuffer, sizeof(int), 6);

  int firstBatch[] = { 1, 2, 3, 4, 5, 6 };
  ASSERT_TRUE(RingBuffer_put(&sourceRingBuffer, firstBatch, 6));

  int drainedValues[3];
  ASSERT_TRUE(RingBuffer_get(&sourceRingBuffer, drainedValues, 3) != NULL);

  int secondBatch[3] = { 10, 11, 12 };
  ASSERT_TRUE(RingBuffer_put(&sourceRingBuffer, secondBatch, 3));

  ASSERT_EQUAL(RingBuffer_getAvailable(&sourceRingBuffer), 6);

  ASSERT_TRUE(RingBuffer_move(&sourceRingBuffer, &destinationRingBuffer, 6));
  ASSERT_TRUE(RingBuffer_isEmpty(&sourceRingBuffer));
  ASSERT_EQUAL(RingBuffer_getAvailable(&destinationRingBuffer), 6);

  int outputValues2[6];
  ASSERT_TRUE(RingBuffer_get(&destinationRingBuffer, outputValues2, 6) != NULL);
  ASSERT_EQUAL(outputValues2[0], 4);
  ASSERT_EQUAL(outputValues2[1], 5);
  ASSERT_EQUAL(outputValues2[2], 6);
  ASSERT_EQUAL(outputValues2[3], 10);
  ASSERT_EQUAL(outputValues2[4], 11);
  ASSERT_EQUAL(outputValues2[5], 12);

  RingBuffer_done(&sourceRingBuffer,      NULL, NULL);
  RingBuffer_done(&destinationRingBuffer, NULL, NULL);

  // destination too small");
  RingBuffer_init(&sourceRingBuffer,      sizeof(int), 8);
  RingBuffer_init(&destinationRingBuffer, sizeof(int), 2);

  int inputValues2[5] = { 1, 2, 3, 4, 5 };
  ASSERT_TRUE(RingBuffer_put(&sourceRingBuffer, inputValues2, 5));

  ASSERT_FALSE(RingBuffer_move(&sourceRingBuffer, &destinationRingBuffer, 5));
  ASSERT_EQUAL(RingBuffer_getAvailable(&sourceRingBuffer), 5);

  RingBuffer_done(&sourceRingBuffer,      NULL, NULL);
  RingBuffer_done(&destinationRingBuffer, NULL, NULL);
}

CTEST(ringBuffer,discard)
{
  RingBuffer ringBuffer;

  RingBuffer_init(&ringBuffer, sizeof(int), 8);

  int inputValues[] = { 1, 2, 3, 4, 5 };
  RingBuffer_put(&ringBuffer, inputValues, 5);

  RingBuffer_discard(&ringBuffer, 2, NULL, NULL);
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 3);

  int outputValues[3];
  ASSERT_TRUE(RingBuffer_get(&ringBuffer, outputValues, 3) != NULL);
  ASSERT_EQUAL(outputValues[0], 3);
  ASSERT_EQUAL(outputValues[1], 4);
  ASSERT_EQUAL(outputValues[2], 5);

  RingBuffer_done(&ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,increment_decrement)
{
  RingBuffer ringBuffer;

  RingBuffer_init(&ringBuffer, sizeof(int), 4);

  RingBuffer_increment(&ringBuffer, 2);
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 2);

  RingBuffer_decrement(&ringBuffer, 2);
  ASSERT_TRUE(RingBuffer_isEmpty(&ringBuffer));

  RingBuffer_done(&ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,cArrayIn_cArrayOut)
{
  RingBuffer ringBuffer;

  RingBuffer_init(&ringBuffer, sizeof(int), 4);

  // Write directly via cArrayIn
  int *inPointer = (int *)RingBuffer_cArrayIn(&ringBuffer);
  ASSERT_TRUE(inPointer != NULL);
  inPointer[0] = 100;
  inPointer[1] = 200;
  RingBuffer_increment(&ringBuffer, 2);
  ASSERT_EQUAL(RingBuffer_getAvailable(&ringBuffer), 2);

  // read directly via cArrayOut
  const int *outPointer = (const int *)RingBuffer_cArrayOut(&ringBuffer);
  ASSERT_TRUE(outPointer != NULL);
  ASSERT_EQUAL(outPointer[0], 100);
  ASSERT_EQUAL(outPointer[1], 200);
  RingBuffer_decrement(&ringBuffer, 2);
  ASSERT_TRUE(RingBuffer_isEmpty(&ringBuffer));

  RingBuffer_done(&ringBuffer, NULL, NULL);
}

CTEST(ringBuffer,struct)
{
  typedef struct { int x; int y; double weight; } Point;

  RingBuffer pointRingBuffer;

  RingBuffer_init(&pointRingBuffer, sizeof(Point), 4);

  Point inputPoint  = {3, 7, 1.5};
  Point outputPoint = {0, 0, 0.0};

  ASSERT_TRUE(RingBuffer_put(&pointRingBuffer, &inputPoint, 1));

  ASSERT_TRUE(RingBuffer_get(&pointRingBuffer, &outputPoint, 1) != NULL);
  ASSERT_EQUAL(outputPoint.x, 3);
  ASSERT_EQUAL(outputPoint.y, 7);
  ASSERT_EQUAL(outputPoint.weight,1.5);

  RingBuffer_done(&pointRingBuffer, NULL, NULL);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
