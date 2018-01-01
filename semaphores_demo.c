/***********************************************************************\
*
* $Source$
* $Revision: 919 $
* $Author: torsten $
* Contents: semaphores demo
* Systems: all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <pthread.h>
#include <sched.h>

#include "global.h"
#include "semaphores.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/
LOCAL Semaphore semaphore;
LOCAL Semaphore semaphoreA,semaphoreB,semaphoreC;

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

LOCAL void demoThreadCode(void)
{
  if (Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE,NO_WAIT))
  {
    sched_yield();
    printf("Thread 0x%016x try without wait OK\n",(int)pthread_self()); fflush(stdout);
    sleep(3);
    Semaphore_unlock(&semaphore);
  }
  else
  {
    sched_yield();
    printf("Thread 0x%016x try without wait fail\n",(int)pthread_self()); fflush(stdout);
  }
  sched_yield();

  Semaphore_forceLock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE);
  {
    printf("Thread 0x%016x try nested lock for read/write...",(int)pthread_self()); fflush(stdout);
    sched_yield();
    sleep(3);
    Semaphore_forceLock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE);
    {
      sched_yield();
      printf("OK\n");
    }
    Semaphore_unlock(&semaphore);
  }
  Semaphore_unlock(&semaphore);
}

LOCAL void deadlockThreadCode(int n)
{
  switch (n)
  {
    case 1:
      printf("Thread 1 try lock A\n");
      Semaphore_lock(&semaphoreA,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER);
      printf("Thread 1 lock A OK\n");
      printf("Thread 1 wait\n");
      sleep(1);
      printf("Thread 1 try lock B\n");
      Semaphore_lock(&semaphoreB,SEMAPHORE_LOCK_TYPE_READ,WAIT_FOREVER);
      printf("Thread 1 lock B OK\n");
      Semaphore_unlock(&semaphoreB);
      Semaphore_unlock(&semaphoreA);
      break;
    case 2:
      printf("Thread 2 try lock B\n");
      Semaphore_lock(&semaphoreB,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER);
      printf("Thread 2 lock B OK\n");
      printf("Thread 2 wait\n");
      sleep(1);
      printf("Thread 2 try lock C\n");
      Semaphore_lock(&semaphoreC,SEMAPHORE_LOCK_TYPE_READ,WAIT_FOREVER);
      printf("Thread 2 lock C OK\n");
      Semaphore_unlock(&semaphoreC);
      Semaphore_unlock(&semaphoreB);
      break;
    case 3:
      printf("Thread 3 try lock C\n");
      Semaphore_lock(&semaphoreC,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER);
      printf("Thread 3 lock C OK\n");
      printf("Thread 3 wait\n");
      sleep(1);
      printf("Thread 3 try lock A\n");
      Semaphore_lock(&semaphoreA,SEMAPHORE_LOCK_TYPE_READ,WAIT_FOREVER);
      printf("Thread 3 lock A OK\n");
      Semaphore_unlock(&semaphoreA);
      Semaphore_unlock(&semaphoreC);
      break;
  }
fprintf(stderr,"%s, %d: done %d \n",__FILE__,__LINE__,n);
}

int main(int argc, char *argv[])
{
  pthread_t t1,t2,t3;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);
  Semaphore_init(&semaphore);

#if 1
  printf("Try lock for read..."); fflush(stdout);
  if (!Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ,WAIT_FOREVER))
  {
    printf("FAIL");
    exit(1);
  }
  printf("OK\n");
  Semaphore_unlock(&semaphore);

  printf("Try lock for read/write..."); fflush(stdout);
  if (!Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER))
  {
    printf("FAIL");
    exit(1);
  }
  printf("OK\n");
  Semaphore_unlock(&semaphore);

  printf("Try force lock for read/write..."); fflush(stdout);
  Semaphore_forceLock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE);
  printf("OK\n");
  Semaphore_unlock(&semaphore);

  printf("Try multiple lock for read/write..."); fflush(stdout);
  Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER);
  Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER);
  printf("OK\n");
  Semaphore_unlock(&semaphore);
  Semaphore_unlock(&semaphore);
#endif

#if 0
  printf("Try multiple lock for read+read/write..."); fflush(stdout);
  Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ,WAIT_FOREVER);
fprintf(stderr,"%s, %d: 1 ok\n",__FILE__,__LINE__);
  Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER);
  printf("OK\n");
  Semaphore_unlock(&semaphore);
  Semaphore_unlock(&semaphore);
#endif

#if 1
  printf("Try nested lock for read/write..."); fflush(stdout);
  Semaphore_forceLock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE);
  {
    Semaphore_forceLock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE);
    {
      printf("OK\n");
    }
    Semaphore_unlock(&semaphore);
  }
  Semaphore_unlock(&semaphore);
#endif

#if 1
  pthread_create(&t1,NULL,(void*(*)(void*))demoThreadCode,NULL);
  pthread_create(&t2,NULL,(void*(*)(void*))demoThreadCode,NULL);
  pthread_join(t2,NULL);
  pthread_join(t1,NULL);
#endif

  Semaphore_done(&semaphore);

  Semaphore_init(&semaphoreA);
  Semaphore_init(&semaphoreB);
  Semaphore_init(&semaphoreC);

#if 0
fprintf(stderr,"%s, %d: \n",__FILE__,__LINE__);
Semaphore_lock(&semaphoreA,SEMAPHORE_LOCK_TYPE_READ,WAIT_FOREVER);
fprintf(stderr,"%s, %d: \n",__FILE__,__LINE__);
//fprintf(stderr,"%s, %d: \n",__FILE__,__LINE__);
//asm("int3");
//Semaphore_lock(&semaphoreA,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER);
Semaphore_lock(&semaphoreA,SEMAPHORE_LOCK_TYPE_READ,WAIT_FOREVER);
fprintf(stderr,"%s, %d: \n",__FILE__,__LINE__);
//exit(1);
#endif

#if 1
  printf("Dead lock detection...\n"); fflush(stdout);
  pthread_create(&t1,NULL,(void*(*)(void*))deadlockThreadCode,(void*)1);
  pthread_create(&t2,NULL,(void*(*)(void*))deadlockThreadCode,(void*)2);
  pthread_create(&t3,NULL,(void*(*)(void*))deadlockThreadCode,(void*)3);
  pthread_join(t3,NULL);
  pthread_join(t2,NULL);
  pthread_join(t1,NULL);
  printf("OK\n");
#endif

  Semaphore_done(&semaphoreC);
  Semaphore_done(&semaphoreB);
  Semaphore_done(&semaphoreA);

  return(0);
}

/* end of file */
