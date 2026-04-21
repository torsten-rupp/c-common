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

CTEST(threadPools,init_done)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));
  ThreadPool_done(&threadPool);
}

CTEST(threadPools,init_done_set)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));

  ThreadPoolSet threadPoolSet;
  ThreadPool_initSet(&threadPoolSet,&threadPool);
  ThreadPool_doneSet(&threadPoolSet);

  ThreadPool_done(&threadPool);
}

CTEST(threadPools,run_join)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));

  ThreadPoolNode *threadPoolNode = ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_join(&threadPool,threadPoolNode);

  ThreadPool_done(&threadPool);
}

CTEST(threadPools,add_set)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));

  ThreadPoolSet threadPoolSet;
  ThreadPool_initSet(&threadPoolSet,&threadPool);

  ThreadPoolNode *threadPoolNode = ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_setAdd(&threadPoolSet,threadPoolNode);

  ThreadPool_doneSet(&threadPoolSet);

  ThreadPool_done(&threadPool);
}

CTEST(threadPools,add_join_set)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));

  ThreadPoolSet threadPoolSet;
  ThreadPool_initSet(&threadPoolSet,&threadPool);

  ThreadPoolNode *threadPoolNode = ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_setAdd(&threadPoolSet,threadPoolNode);

  ThreadPool_joinSet(&threadPoolSet);

  ThreadPool_doneSet(&threadPoolSet);

  ThreadPool_done(&threadPool);
}

CTEST(threadPools,run_joinall)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));

  ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_joinAll(&threadPool);

  ThreadPool_done(&threadPool);
}

CTEST(threadPools,idle_count)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));

  ASSERT_EQUAL_U(4,ThreadPool_idleCount(&threadPool));

  ThreadPool_done(&threadPool);
}

CTEST(threadPools,running_count)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));

  ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_run(&threadPool,testThread,NULL);
  ASSERT_EQUAL_U(2,ThreadPool_idleCount(&threadPool));
  ASSERT_EQUAL_U(2,ThreadPool_runningCount(&threadPool));
  ThreadPool_joinAll(&threadPool);

  ThreadPool_done(&threadPool);
}

CTEST(threadPools,run_increase)
{
  ThreadPool threadPool;
  ASSERT_TRUE(ThreadPool_init(&threadPool,"test",0,4,8));

  ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_run(&threadPool,testThread,NULL);
  ThreadPool_run(&threadPool,testThread,NULL);
  ASSERT_EQUAL_U(0,ThreadPool_idleCount(&threadPool));
  ASSERT_EQUAL_U(5,ThreadPool_runningCount(&threadPool));
  ThreadPool_joinAll(&threadPool);

  ThreadPool_done(&threadPool);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
