#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/fragmentlists.h"

typedef struct TestNode
{
  LIST_NODE_HEADER(struct TestNode);
} TestNode;

typedef struct
{
  LIST_HEADER(TestNode);
} TestList;

CTEST(fragmentlists,init_done)
{
  FragmentList testFragmentList;

  FragmentList_init(&testFragmentList);
  FragmentList_done(&testFragmentList);
}

CTEST(fragmentlists,init_done_node)
{
  String       name;
  FragmentNode testFragmentNode;

  name = String_newCString("test");
  FragmentList_initNode(&testFragmentNode,name,1000,NULL,0,0);
  ASSERT_STR(String_cString(testFragmentNode.name),"test");
  ASSERT_EQUAL(FragmentList_getSize(&testFragmentNode),0);
  ASSERT_EQUAL(FragmentList_getTotalSize(&testFragmentNode),1000);
  ASSERT_EQUAL(List_count(&testFragmentNode.rangeList),0);
  ASSERT_EQUAL(testFragmentNode.rangeListSum,0);
  FragmentList_doneNode(&testFragmentNode);
  String_delete(name);
}

CTEST(fragmentlists,lock_unlock_node)
{
  String       name;
  FragmentNode testFragmentNode;

  name = String_newCString("test");
  FragmentList_initNode(&testFragmentNode,name,1000,NULL,0,0);
  ASSERT_EQUAL(testFragmentNode.lockCount,0);
  FragmentList_lockNode(&testFragmentNode);
  ASSERT_EQUAL(testFragmentNode.lockCount,1);
  FragmentList_unlockNode(&testFragmentNode);
  ASSERT_EQUAL(testFragmentNode.lockCount,0);
  FragmentList_doneNode(&testFragmentNode);
  String_delete(name);
}

CTEST(fragmentlists,add_remove)
{
  FragmentList testFragmentList;
  String       name;
  FragmentNode *testFragmentNode;

  FragmentList_init(&testFragmentList);

  name = String_newCString("test");

  testFragmentNode = FragmentList_add(&testFragmentList,name,1000,NULL,0,0);
  ASSERT_NOT_NULL(testFragmentNode);
  ASSERT_STR(String_cString(testFragmentNode->name),"test");
  ASSERT_EQUAL(FragmentList_getSize(testFragmentNode),0);
  ASSERT_EQUAL(FragmentList_getTotalSize(testFragmentNode),1000);
  ASSERT_EQUAL(testFragmentNode->lockCount,0);
  ASSERT_EQUAL(List_count(&testFragmentNode->rangeList),0);
  ASSERT_EQUAL(testFragmentNode->rangeListSum,0);

  FragmentList_discard(&testFragmentList,testFragmentNode);

  String_delete(name);

  FragmentList_done(&testFragmentList);
}

CTEST(fragmentlists,add_remove_ranges)
{
  FragmentList testFragmentList;
  String       name;
  FragmentNode *testFragmentNode;

  FragmentList_init(&testFragmentList);

  name = String_newCString("test");

  testFragmentNode = FragmentList_add(&testFragmentList,name,1000,NULL,0,0);
  ASSERT_NOT_NULL(testFragmentNode);
  ASSERT_EQUAL(FragmentList_getSize(testFragmentNode),0);

  FragmentList_addRange(testFragmentNode,0,500);
  ASSERT_EQUAL(FragmentList_getSize(testFragmentNode),500);
  ASSERT_FALSE(FragmentList_isComplete(testFragmentNode));

  FragmentList_addRange(testFragmentNode,800,200);
  ASSERT_EQUAL(FragmentList_getSize(testFragmentNode),700);
  ASSERT_FALSE(FragmentList_isComplete(testFragmentNode));

  FragmentList_addRange(testFragmentNode,500,300);
  ASSERT_EQUAL(FragmentList_getSize(testFragmentNode),1000);
  ASSERT_TRUE(FragmentList_isComplete(testFragmentNode));

  FragmentList_clearRanges(testFragmentNode);
  ASSERT_EQUAL(FragmentList_getSize(testFragmentNode),0);
  ASSERT_FALSE(FragmentList_isComplete(testFragmentNode));

  FragmentList_clearRanges(testFragmentNode);
  FragmentList_addRange(testFragmentNode,500,500);
  FragmentList_print(stdout,0,testFragmentNode,TRUE);
  FragmentList_addRange(testFragmentNode,0,500);
  FragmentList_print(stdout,0,testFragmentNode,TRUE);
  ASSERT_EQUAL(FragmentList_getSize(testFragmentNode),1000);
  ASSERT_TRUE(FragmentList_isComplete(testFragmentNode));

  ASSERT_TRUE(FragmentList_rangeExists(testFragmentNode,0,500));
  ASSERT_TRUE(FragmentList_rangeExists(testFragmentNode,1,1));
  ASSERT_FALSE(FragmentList_rangeExists(testFragmentNode,1000,500));

  FragmentList_discard(&testFragmentList,testFragmentNode);

  String_delete(name);

  FragmentList_done(&testFragmentList);
}

CTEST(fragmentlists,print)
{
  FragmentList testFragmentList;
  String       name;
  FragmentNode *testFragmentNode;

  FragmentList_init(&testFragmentList);

  name = String_newCString("test");

  testFragmentNode = FragmentList_add(&testFragmentList,name,1000,NULL,0,0);
  ASSERT_NOT_NULL(testFragmentNode);
  ASSERT_EQUAL(FragmentList_getSize(testFragmentNode),0);

  FragmentList_addRange(testFragmentNode,0,500);
  FragmentList_addRange(testFragmentNode,800,200);

  FragmentList_print(stdout,0,testFragmentNode,TRUE);

  String_delete(name);

  FragmentList_done(&testFragmentList);
}

#if 0
#endif

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
