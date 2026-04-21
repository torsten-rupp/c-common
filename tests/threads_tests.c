#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/global.h"

#include "common/threadpools.h"

LOCAL void testThread(void *argument)
{
  UNUSED_VARIABLE(argument);

  sleep(5);
}

LOCAL void testThreadYield(void *argument)
{
  UNUSED_VARIABLE(argument);

  Thread_yield();
  sleep(5);
}

LOCAL void testThreadDelay(void *argument)
{
  UNUSED_VARIABLE(argument);

  Thread_delay(5000);
}

CTEST(threads,init_join_done)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));
  Thread_join(&thread);
  Thread_done(&thread);
}

CTEST(threads,get_set_priority)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));

  ASSERT_TRUE(Thread_setPriority(&thread,0));
  ASSERT_EQUAL_U(0,Thread_getPriority(&thread));

  Thread_join(&thread);
  Thread_done(&thread);
}

CTEST(threads,run_join)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));

  ASSERT_FALSE(Thread_isQuit(&thread));
  Thread_join(&thread);

  Thread_done(&thread);
}

CTEST(threads,quit)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));

  ASSERT_FALSE(Thread_isQuit(&thread));
  Thread_quit(&thread);
  ASSERT_TRUE(Thread_isQuit(&thread));

  Thread_join(&thread);
  Thread_done(&thread);
}

CTEST(threads,terminated)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));

  ASSERT_FALSE(Thread_isTerminated(&thread));
  Thread_quit(&thread);
  Thread_join(&thread);
  ASSERT_TRUE(Thread_isTerminated(&thread));

  Thread_done(&thread);
}

CTEST(threads,yield)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThreadYield,NULL));

  Thread_join(&thread);

  Thread_done(&thread);
}

CTEST(threads,delay)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThreadDelay,NULL));

  Thread_join(&thread);

  Thread_done(&thread);
}

CTEST(threads,id)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));

  ThreadId threadId = Thread_getId(&thread);
  Thread_getIdString(threadId);
  Thread_getCurrentId();
  Thread_getCurrentIdString();
  Thread_getCurrentLWPId();
  Thread_join(&thread);

  Thread_done(&thread);
}

CTEST(threads,equals)
{
  Thread thread1,thread2;
  ASSERT_TRUE(Thread_init(&thread1,"test1",0,testThread,NULL));
  ASSERT_TRUE(Thread_init(&thread2,"test2",0,testThread,NULL));

  ThreadId threadId1 = Thread_getId(&thread1);
  ThreadId threadId2 = Thread_getId(&thread2);
  ASSERT_TRUE(Thread_equalThreads(threadId1,threadId1));
  ASSERT_FALSE(Thread_equalThreads(threadId1,threadId2));
  Thread_join(&thread2);
  Thread_join(&thread1);

  Thread_done(&thread2);
  Thread_done(&thread1);
}

CTEST(threads,current)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));

  ThreadId threadId = Thread_getId(&thread);
  ASSERT_FALSE(Thread_isCurrentThread(threadId));
  Thread_join(&thread);

  Thread_done(&thread);
}

CTEST(threads,name)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));

  ASSERT_STR("test",Thread_getName(Thread_getId(&thread)));
  Thread_join(&thread);

  Thread_done(&thread);
}

CTEST(threads,init_done_local)
{
  Thread thread;
  ASSERT_TRUE(Thread_init(&thread,"test",0,testThread,NULL));

  ThreadLocalStorage threadLocalStorage;
  Thread_initLocalVariable(&threadLocalStorage,NULL,NULL);
  Thread_doneLocalVariable(&threadLocalStorage,NULL,NULL);

  Thread_join(&thread);
  Thread_done(&thread);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
