#include <stdlib.h>
#include <assert.h>

#include "common/global.h"
#include "common/stringlists.h"
#include "common/strings.h"

int main(int argc, char *argv[])
{
  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  StringList  stringList;
  String      s;
  ConstString t;

  StringList_init(&stringList);

  s = String_new();

  String_setCString(s,"Hello");
  StringList_append(&stringList,s);

  String_setChar(s,' ');
  StringList_append(&stringList,s);

  String_setCString(s,"World!");
  StringList_append(&stringList,s);

  STRINGLIST_ITERATE(&stringList,t)
  {
    printf("%s\n",String_cString(t));
  }

  String_delete(s);
  StringList_done(&stringList);

  return 0;
}
