/***********************************************************************\
*
* $Source$
* $Revision$
* $Author$
* Contents: ring buffer demo
* Systems: *nix
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "global.h"
#include "ringbuffers.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

typedef struct
{
  int  i;
  char s[32];
} Data;

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

LOCAL void freeData(void *data, void *userData)
{
  UNUSED_VARIABLE(userData);

  free(data);
}

/*---------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
  RingBuffer *ringBuffer;
  RingBuffer *ringBuffer1,*ringBuffer2;
  int        i;
  int        n;
  Data       *data;
  int        m[3];
  int        *p;
  char       s[100];
  const char *t;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  // init/done ring buffer

  // allocate and free a ring buffer
  ringBuffer = RingBuffer_new(sizeof(int),10);
  RingBuffer_delete(ringBuffer,NULL,NULL);

  ringBuffer = RINGBUFFER_NEW(int,10);
  RINGBUFFER_DELETE(ringBuffer);

  // put/get elements to/from ring buffer
  ringBuffer = RINGBUFFER_NEW(int,20);
  for (i = 1; i <= 10; i++)
  {
    n = i*i;
    RingBuffer_put(ringBuffer,&n,1);
  }
  printf("avail %lu\n",RingBuffer_getAvailable(ringBuffer));
  printf("free %lu\n",RingBuffer_getFree(ringBuffer));
  for (i = 1; i <= 10; i++)
  {
    RingBuffer_get(ringBuffer,&n,1);
    printf(" %d",n);
  }
  printf("\n");
  RINGBUFFER_DELETE(ringBuffer);

  ringBuffer = RingBuffer_new(sizeof(Data*),10);
  for (i = 1; i <= 10; i++)
  {
    data = (Data*)malloc(sizeof(Data));
    data->i = i*i;
    sprintf(data->s,"%d",i*i);
    RingBuffer_put(ringBuffer,&data,1);
  }
  for (i = 1; i <= 10; i++)
  {
    RingBuffer_get(ringBuffer,&data,1);
    printf(" %p: %d %s\n",data,data->i,data->s);
  }
  RingBuffer_delete(ringBuffer,freeData,NULL);

  // put/get multiple elements to/from ring buffer
  ringBuffer = RINGBUFFER_NEW(int,10);
  for (i = 1; i <= 3; i++)
  {
    m[0] = i;
    m[1] = i*i;
    m[2] = i*i*i;
    RingBuffer_put(ringBuffer,m,3);
  }
  for (i = 1; i <= 3; i++)
  {
    RingBuffer_get(ringBuffer,m,3);
    printf(" %d %d %d\n",m[0],m[1],m[2]);
  }
  RINGBUFFER_DELETE(ringBuffer);

  // iterate over elements in ring buffer
  ringBuffer = RINGBUFFER_NEW(int,10);
  for (i = 1; i <= 5; i++)
  {
    n = i*i;
    RingBuffer_put(ringBuffer,&n,1);
  }
  for (i = 1; i <= 5; i++)
  {
    RingBuffer_get(ringBuffer,&n,1);
  }
  for (i = 1; i <= 10; i++)
  {
    n = i*i;
    RingBuffer_put(ringBuffer,&n,1);
  }
  RINGBUFFER_ITERATE(ringBuffer,p)
  {
    printf(" %d",*p);
  }
  printf("\n");
  RINGBUFFER_DELETE(ringBuffer);

  // move from ring buffer to ring buffer
  memset(s,0,sizeof(s));
  ringBuffer1 = RINGBUFFER_NEW(char,20);
  ringBuffer2 = RINGBUFFER_NEW(char,20);
  RingBuffer_put(ringBuffer1,"Hello World!",13);
  RingBuffer_move(ringBuffer1,ringBuffer2,13);
  RingBuffer_get(ringBuffer2,s,13);
  printf("moved: %s\n",s);
  RINGBUFFER_DELETE(ringBuffer2);
  RINGBUFFER_DELETE(ringBuffer1);

  // get array
  ringBuffer = RINGBUFFER_NEW(char,20);
  RingBuffer_put(ringBuffer,"123456789ABC",13);
  RingBuffer_get(ringBuffer,s,13);
  RingBuffer_put(ringBuffer,"Hello World!",13);
  t = (const char*)RingBuffer_cArray(ringBuffer);
  printf("array: %s\n",t);
  RINGBUFFER_DELETE(ringBuffer);

  /* uncomment to see debug functions */

  #if 0
  /* debug functions: lost ring buffer */
  ringBuffer = RINGBUFFER_NEW(int,20);
  #endif /* 0 */

  #if 0
  /* debug function: duplicate free */
  r = RINGBUFFER_NEW(int,20);
  RINGBUFFER_DELETE(ringBuffer);
  RINGBUFFER_DELETE(ringBuffer);
  #endif /* 0 */

  RingBuffer_debugPrintInfo();

  return 0;
}

#ifdef __cplusplus
  }
#endif

/* end of file */
