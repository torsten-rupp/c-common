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

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

LOCAL void demoCode(Semaphore *semaphore)
{
  if (Semaphore_lock(semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE,SEMAPHORE_NO_WAIT))
  {
    sched_yield();
    printf("Thread %d try without wait OK\n",(int)pthread_self()); fflush(stdout);
    sleep(3);
    Semaphore_unlock(semaphore);
  }
  else
  {
    sched_yield();
    printf("Thread %d try without wait fail\n",(int)pthread_self()); fflush(stdout);
  }
  sched_yield();

  Semaphore_forceLock(semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE);
  {
    printf("Thread %d try nested lock for read/write...",(int)pthread_self()); fflush(stdout);
    sched_yield();
    sleep(3);
    Semaphore_forceLock(semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE);
    {
      sched_yield();
      printf("OK\n");
    }
    Semaphore_unlock(semaphore);
  }
  Semaphore_unlock(semaphore);
}

int main(int argc, char *argv[])
 {
  Semaphore semaphore;
  pthread_t t1,t2;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  Semaphore_init(&semaphore);

  printf("Try lock for read..."); fflush(stdout);
  if (!Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ,SEMAPHORE_WAIT_FOREVER))
  {
    printf("FAIL");
    exit(1);
  }
  printf("OK\n");
  Semaphore_unlock(&semaphore);

  printf("Try lock for read/write..."); fflush(stdout);
  if (!Semaphore_lock(&semaphore,SEMAPHORE_LOCK_TYPE_READ_WRITE,SEMAPHORE_WAIT_FOREVER))
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

  pthread_create(&t1,NULL,(void*(*)(void*))demoCode,&semaphore);
  pthread_create(&t2,NULL,(void*(*)(void*))demoCode,&semaphore);
  pthread_join(t2,NULL);
  pthread_join(t1,NULL);

  Semaphore_done(&semaphore);

  return(0);
 }

/* end of file */
