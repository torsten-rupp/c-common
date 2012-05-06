#include <stdlib.h>
#include <assert.h>

#include "global.h"
#include "stringlists.h"
#include "strings.h"

int main(int argc, char *argv[])
{
  StringList stringList;
  String     s,t;
  StringNode *stringNode;

  StringList_init(&stringList);

  s = String_new();

  String_setCString(s,"Hello");
  StringList_append(&stringList,s);

  String_setChar(s,' ');
  StringList_append(&stringList,s);

  String_setCString(s,"World!");
  StringList_append(&stringList,s);

  STRINGLIST_ITERATE(&stringList,stringNode,t)
  {
    printf("%s\n",String_cString(t));
  }

  String_delete(s);
  StringList_done(&stringList);

  return 0;
}
