#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/lists.h"

typedef struct TestNode
{
  LIST_NODE_HEADER(struct TestNode);
} TestNode;

typedef struct
{
  LIST_HEADER(TestNode);
} TestList;

#if 0
void List_new(void *list,
              void *node,
              void *nextNode
             );
List *List_duplicate(const void                *fromList,
                     const void                *fromListFromNode,
                     const void                *fromListToNode,
                     ListNodeDuplicateFunction listNodeDuplicateFunction,
                     void                      *listNodeDuplicateUserData
                    );
void List_delete(void                 *list,
                 ListNodeFreeFunction listNodeFreeFunction,
                 void                 *listNodeFreeUserData
                );
#endif

CTEST(lists,init_done_node)
{
  Node     *node;
  TestNode *testNode;

  node = List_newNode(sizeof(TestNode));
  ASSERT_TRUE(node != NULL);
  List_deleteNode(node);

  testNode = LIST_NEW_NODE(TestNode);
  ASSERT_TRUE(testNode != NULL);
  LIST_DELETE_NODE(testNode);
}

CTEST(lists,init_done)
{
  TestNode *testNode;
  TestList testList1,testList2;

  testNode = LIST_NEW_NODE(TestNode);

  List_init(&testList1,CALLBACK_(NULL,NULL),CALLBACK_(NULL,NULL));
  List_done(&testList1);

  List_init(&testList1,CALLBACK_(NULL,NULL),CALLBACK_(NULL,NULL));
  List_initDuplicate(&testList2,&testList1,NULL,NULL,CALLBACK_INLINE(void*,(const void *fromNode, void *userData),{ UNUSED_VARIABLE(fromNode); UNUSED_VARIABLE(userData); return NULL; },NULL),CALLBACK_(NULL,NULL));
  List_done(&testList2);
  List_done(&testList1);

LIST_DELETE_NODE(testNode);
}

CTEST(lists,new_delete)
{
  TestList *testList1,*testList2;

  testList1 = (TestList*)List_new(CALLBACK_(NULL,NULL),CALLBACK_(NULL,NULL));
  testList2 = (TestList*)List_duplicate(testList1,NULL,NULL,CALLBACK_INLINE(void*,(const void *fromNode, void *userData),{ UNUSED_VARIABLE(fromNode); UNUSED_VARIABLE(userData); return NULL; },NULL),CALLBACK_(NULL,NULL));
  List_delete(testList2);
  List_delete(testList1);
}

CTEST(lists,duplicate)
{
}

#if 0
#endif

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
