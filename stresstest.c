/***********************************************************************\
*
* $Revision$
* $Date$
* $Author$
* Contents: stress test
* Systems: all
*
\***********************************************************************/


/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "common/global.h"

#include "common/lists.h"
#include "common/strings.h"
#include "common/stringlists.h"
#include "common/arrays.h"
#include "common/ringbuffers.h"

#include "common/threads.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/
#define MAX_THREADS 256

/***************************** Datatypes *******************************/
typedef struct StressNode
{
  LIST_NODE_HEADER(struct StressNode);

  char s[100];
} StressNode;

typedef struct
{
  LIST_HEADER(StressNode);
} StressList;

/***************************** Variables *******************************/
LOCAL bool quitFlag;

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

LOCAL void freeStressNode(StressNode *stressNode, void *userData)
{
  UNUSED_VARIABLE(userData);

  LIST_DELETE_NODE(stressNode);
}

LOCAL void listThreadCode(void)
{
  StressList stressList;
  StressNode *stressNode;

  while (!quitFlag)
  {
    List_init(&stressList);

    stressNode = LIST_NEW_NODE(StressNode);
    List_append(&stressList,stressNode);
    stressNode = LIST_NEW_NODE(StressNode);
    List_append(&stressList,stressNode);
    stressNode = LIST_NEW_NODE(StressNode);
    List_append(&stressList,stressNode);

    List_done(&stressList,(ListNodeFreeFunction)freeStressNode,NULL);

    Thread_yield();
  }
}

int main(int argc, const char* args[])
{
  uint   threadCount;
  Thread threads[MAX_THREADS];
  uint   z;


  if (argc < 2)
  {
    fprintf(stderr,"Usage: %s <thread count>\n",args[0]);
    exit(1);
  }

  threadCount = (uint)atoi(args[1]);

  quitFlag = FALSE;
  for (z = 0; z < threadCount; z++)
  {
    Thread_init(&threads[z],"list",0,listThreadCode,NULL);
  }

  quitFlag = TRUE;
  for (z = 0; z < threadCount; z++)
  {
    Thread_join(&threads[z]);
  }

  return 0;
}

#ifdef __cplusplus
  }
#endif

/* end of file */
