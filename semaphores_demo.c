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
LOCAL Semaphore semaphoreA,semaphoreB;

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

LOCAL void demoThreadCode(void)
{
  if (Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE,NO_WAIT))
  {
    sched_yield();
    printf("Thread %d try without wait OK\n",(int)pthread_self()); fflush(stdout);
    sleep(3);
    Semaphore_unlock(&semaphore);
  }
  else
  {
    sched_yield();
    printf("Thread %d try without wait fail\n",(int)pthread_self()); fflush(stdout);
  }
  sched_yield();

  Semaphore_forceLock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE);
  {
    printf("Thread %d try nested lock for read/write...",(int)pthread_self()); fflush(stdout);
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
      break;
    case 2:
      printf("Thread 2 try lock B\n");
      Semaphore_lock(&semaphoreB,SEMAPHORE_LOCK_TYPE_READ_WRITE,WAIT_FOREVER);
      printf("Thread 2 lock B OK\n");
      printf("Thread 2 wait\n");
      sleep(1);
      printf("Thread 2 try lock A\n");
      Semaphore_lock(&semaphoreA,SEMAPHORE_LOCK_TYPE_READ,WAIT_FOREVER);
      printf("Thread 2 lock A OK\n");
      break;
  }
}

int main(int argc, char *argv[])
{
  pthread_t t1,t2;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

#if 0
  Semaphore_init(&semaphore);

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

  pthread_create(&t1,NULL,(void*(*)(void*))demoThreadCode,NULL);
  pthread_create(&t2,NULL,(void*(*)(void*))demoThreadCode,NULL);
  pthread_join(t2,NULL);
  pthread_join(t1,NULL);

  Semaphore_done(&semaphore);
#endif

  Semaphore_init(&semaphoreA);
  Semaphore_init(&semaphoreB);

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

  printf("Dead lock detection..."); fflush(stdout);
  pthread_create(&t1,NULL,(void*(*)(void*))deadlockThreadCode,(void*)1);
  pthread_create(&t2,NULL,(void*(*)(void*))deadlockThreadCode,(void*)2);
  pthread_join(t2,NULL);
  pthread_join(t1,NULL);
  printf("OK\n");

  Semaphore_done(&semaphoreB);
  Semaphore_done(&semaphoreA);

  return(0);
}

/* end of file */
