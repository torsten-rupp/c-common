#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/global.h"

#include "common/msgqueues.h"

typedef struct
{
  int value;
}
TestMessage;

CTEST(MsgQueues,init_done)
{
  MsgQueue msgQueue;
  ASSERT_TRUE(MsgQueue_init(&msgQueue, 0, NULL, NULL));
  ASSERT_TRUE(MsgQueue_isEmpty(&msgQueue));
  ASSERT_EQUAL(0, MsgQueue_count(&msgQueue));
  MsgQueue_done(&msgQueue);
}

CTEST(MsgQueues,new_delete)
{
  MsgQueue *msgQueue = MsgQueue_new(0, NULL, NULL);
  ASSERT_TRUE(msgQueue != NULL);
  ASSERT_TRUE(MsgQueue_isEmpty(msgQueue));
  ASSERT_EQUAL(0, MsgQueue_count(msgQueue));
  MsgQueue_delete(msgQueue);
}

CTEST(MsgQueues,put_get)
{
  MsgQueue msgQueue;

  // single message
  MsgQueue_init(&msgQueue, 0, NULL, NULL);

  TestMessage sentMessage   = {42};
  ASSERT_TRUE(MsgQueue_put(&msgQueue, &sentMessage, sizeof(sentMessage)));
  ASSERT_FALSE(MsgQueue_isEmpty(&msgQueue));
  ASSERT_EQUAL(1, MsgQueue_count(&msgQueue));

  TestMessage receivedMessage1 = {0};
  size_t      receivedSize    = 0;
  ASSERT_TRUE(MsgQueue_get(&msgQueue, &receivedMessage1, &receivedSize, sizeof(receivedMessage1), WAIT_FOREVER));
  ASSERT_EQUAL(42, receivedMessage1.value);
  ASSERT_EQUAL(sizeof(receivedMessage1), receivedSize);
  ASSERT_TRUE(MsgQueue_isEmpty(&msgQueue));

  MsgQueue_done(&msgQueue);

  // multiple messages
  MsgQueue_init(&msgQueue, 0, NULL, NULL);

  for (int messageIndex = 1; messageIndex <= 5; messageIndex++)
  {
    TestMessage outgoingMessage = {messageIndex};
    MsgQueue_put(&msgQueue, &outgoingMessage, sizeof(outgoingMessage));
  }
  ASSERT_EQUAL(5, MsgQueue_count(&msgQueue));

  bool orderCorrect = TRUE;
  for (int expectedValue = 1; expectedValue <= 5; expectedValue++)
  {
    TestMessage incomingMessage = {0};
    MsgQueue_get(&msgQueue, &incomingMessage, NULL, sizeof(incomingMessage), WAIT_FOREVER);
    if (incomingMessage.value != expectedValue)
    {
      orderCorrect = false;
    }
  }
  ASSERT_TRUE(orderCorrect);
  ASSERT_TRUE(MsgQueue_isEmpty(&msgQueue));

  MsgQueue_done(&msgQueue);

  // get timeout on empty queue
  MsgQueue_init(&msgQueue, 0, NULL, NULL);

  TestMessage receivedMessage2 = {0};
  ASSERT_FALSE(MsgQueue_get(&msgQueue, &receivedMessage2, NULL, sizeof(receivedMessage2), 50));

  MsgQueue_done(&msgQueue);
}

CTEST(MsgQueues,terminate)
{
MsgQueue msgQueue;
  MsgQueue_init(&msgQueue, 0, NULL, NULL);

  TestMessage sentMessage = {5};
  MsgQueue_put(&msgQueue, &sentMessage, sizeof(sentMessage));

  MsgQueue_terminate(&msgQueue);

  /* After terminate, get must return false regardless of queue contents */
  TestMessage receivedMessage = {0};
  ASSERT_FALSE(MsgQueue_get(&msgQueue, &receivedMessage, NULL, sizeof(receivedMessage), WAIT_FOREVER));

  MsgQueue_done(&msgQueue);
}

CTEST(MsgQueues,clear)
{
  MsgQueue msgQueue;
  MsgQueue_init(&msgQueue, 0, NULL, NULL);

  for (int messageIndex = 0; messageIndex < 4; messageIndex++)
  {
    TestMessage outgoingMessage = {messageIndex};
    MsgQueue_put(&msgQueue, &outgoingMessage, sizeof(outgoingMessage));
  }
  ASSERT_EQUAL(4, MsgQueue_count(&msgQueue));

  MsgQueue_clear(&msgQueue);
  ASSERT_TRUE(MsgQueue_isEmpty(&msgQueue));

  TestMessage newMessage = {42};
  MsgQueue_put(&msgQueue, &newMessage, sizeof(newMessage));
  ASSERT_EQUAL(1, MsgQueue_count(&msgQueue));

  MsgQueue_done(&msgQueue);
}

CTEST(MsgQueues,lock_unlock)
{
  MsgQueue msgQueue;
  MsgQueue_init(&msgQueue, 0, NULL, NULL);

  MsgQueue_lock(&msgQueue);
  TestMessage sentMessage = {11};
  ASSERT_TRUE(MsgQueue_put(&msgQueue, &sentMessage, sizeof(sentMessage)));
  MsgQueue_unlock(&msgQueue);
  ASSERT_EQUAL(1, MsgQueue_count(&msgQueue));

  MsgQueue_done(&msgQueue);
}

CTEST(MsgQueues,setendofmsg)
{
  MsgQueue msgQueue;

  MsgQueue_init(&msgQueue, 0, NULL, NULL);

  TestMessage sentMessage = {99};
  MsgQueue_put(&msgQueue, &sentMessage, sizeof(sentMessage));

  MsgQueue_setEndOfMsg(&msgQueue);

  /* Existing message still retrievable */
  TestMessage receivedMessage = {0};
  ASSERT_TRUE(MsgQueue_get(&msgQueue, &receivedMessage, NULL, sizeof(receivedMessage), WAIT_FOREVER));
  ASSERT_EQUAL(99, receivedMessage.value);

  /* Further puts are rejected */
  TestMessage extraMessage  = {100};
  ASSERT_FALSE(MsgQueue_put(&msgQueue, &extraMessage, sizeof(TestMessage)));

  /* Get on empty queue with end-of-msg set returns false immediately */
  ASSERT_FALSE(MsgQueue_get(&msgQueue, &receivedMessage, NULL, sizeof(receivedMessage), WAIT_FOREVER));

  MsgQueue_done(&msgQueue);
}

CTEST(MsgQueues,reset)
{
  MsgQueue msgQueue;
  MsgQueue_init(&msgQueue, 0, NULL, NULL);

  TestMessage sentMessage = {7};
  MsgQueue_put(&msgQueue, &sentMessage, sizeof(sentMessage));
  MsgQueue_setEndOfMsg(&msgQueue);

  MsgQueue_reset(&msgQueue);
  ASSERT_TRUE(MsgQueue_isEmpty(&msgQueue));

  /* Puts must work again after reset */
  TestMessage newMessage     = {8};
  ASSERT_TRUE(MsgQueue_put(&msgQueue, &newMessage, sizeof(newMessage)));

  TestMessage receivedMessage = {0};
  MsgQueue_get(&msgQueue, &receivedMessage, NULL, sizeof(receivedMessage), WAIT_FOREVER);
  ASSERT_EQUAL(8, receivedMessage.value);

  MsgQueue_done(&msgQueue);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
