/***********************************************************************\
*
* Contents: dynamic list demo
* Systems: all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common/global.h"
#include "common/lists.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/
/* list node */
typedef struct StringNode
{
  LIST_NODE_HEADER(struct StringNode);

  char s[100];
} StringNode;

/* list */
typedef struct
{
  LIST_HEADER(StringNode);
} StringList;

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

LOCAL void freeStringNode(StringNode *stringNode, void *userData)
{
  UNUSED_VARIABLE(stringNode);
  UNUSED_VARIABLE(userData);

  /* code to free the data of a single node */
}

LOCAL int compareStringNodes(StringNode *stringNode1, StringNode *stringNode2, void *userData)
{
  UNUSED_VARIABLE(userData);

  return strcmp(stringNode1->s,stringNode2->s);
}

/*---------------------------------------------------------------------*/

int main(int argc, char *argv[])
 {
  StringList stringList;
  StringNode *stringNode;
  StringList stringList1,stringList2;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  /* create a list, traverse it, delete list */
  List_init(&stringList,NULL,NULL,(ListNodeFreeFunction)freeStringNode,NULL);

  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"Hello");
  List_append(&stringList,stringNode);

  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s," ");
  List_append(&stringList,stringNode);

  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"World");
  List_append(&stringList,stringNode);

  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"\n");
  List_append(&stringList,stringNode);

  stringNode = stringList.head;
  while (stringNode != NULL)
  {
    printf("%s",stringNode->s);
    stringNode = stringNode->next;
  }

  List_done(&stringList);

  /* create a list, print it, sort it, print it, delete list */
  List_init(&stringList,NULL,NULL,(ListNodeFreeFunction)freeStringNode,NULL);

  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"b");
  List_append(&stringList,stringNode);
  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"a");
  List_append(&stringList,stringNode);
  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"y");
  List_append(&stringList,stringNode);
  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"x");
  List_append(&stringList,stringNode);
  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"z");
  List_append(&stringList,stringNode);

  printf("not sorted:\n");
  stringNode = stringList.head;
  while (stringNode != NULL)
  {
    printf("%p: %s\n",stringNode,stringNode->s);
    stringNode = stringNode->next;
  }

  List_sort(&stringList,
            (ListNodeCompareFunction)compareStringNodes,
            NULL
           );

  printf("sorted:\n");
  stringNode = stringList.head;
  while (stringNode != NULL)
  {
    printf("%p: %s\n",stringNode,stringNode->s);
    stringNode = stringNode->next;
  }

  List_done(&stringList);

  /* debug checks */

#if 0
  /* duplicate insert */
  List_init(&stringList);

  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"Hello");

  List_append(&stringList,stringNode);
  List_append(&stringList,stringNode);

  List_done(&stringList,(ListNodeFreeFunction)freeStringNode,NULL);
#endif

#if 1
  List_init(&stringList1,NULL,NULL,(ListNodeFreeFunction)freeStringNode,NULL);
  List_init(&stringList2,NULL,NULL,(ListNodeFreeFunction)freeStringNode,NULL);

  stringNode = LIST_NEW_NODE(StringNode);
  strcpy(stringNode->s,"Hello");

  List_append(&stringList1,stringNode);
  List_append(&stringList2,stringNode);

  List_done(&stringList2);
  List_done(&stringList1);
#endif

  return(0);
 }

/* end of file */
