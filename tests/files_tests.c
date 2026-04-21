#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/files.h"

CTEST(files,iteratePath)
{
  String path   = String_new();
  String string = String_new();
  size_t n;
  bool   flag;
  bool   condition;

#if 0
  String_setCString(path,"abc/def/123");
  n    = 0;
  flag = FALSE;
  FILE_PATH_ITERATE(path,string,FALSE)
  {
    switch (n)
    {
      case 0: ASSERT_STR("abc",String_cString(string)); break;
      case 1: ASSERT_STR("def",String_cString(string)); break;
      case 2: ASSERT_STR("123",String_cString(string)); break;
    }
    n++;
    flag = TRUE;
  }
  ASSERT_TRUE(flag);

  String_setCString(path,"abc/def/123");
  n    = 0;
  flag = FALSE;
  FILE_PATH_ITERATE(path,string,TRUE)
  {
    switch (n)
    {
      case 0: ASSERT_STR("abc",String_cString(string)); break;
      case 1: ASSERT_STR("abc/def",String_cString(string)); break;
      case 2: ASSERT_STR("abc/def/123",String_cString(string)); break;
    }
    n++;
    flag = TRUE;
  }
  ASSERT_TRUE(flag);
#endif

  String_setCString(path,"/abc/def/123");
  n    = 0;
  flag = FALSE;
  FILE_PATH_ITERATE(path,string,FALSE)
  {
    switch (n)
    {
      case 0: ASSERT_STR("/",String_cString(string)); break;
      case 1: ASSERT_STR("abc",String_cString(string)); break;
      case 2: ASSERT_STR("def",String_cString(string)); break;
      case 3: ASSERT_STR("123",String_cString(string)); break;
    }
    n++;
    flag = TRUE;
  }
  ASSERT_TRUE(flag);

  String_setCString(path,"/abc/def/123");
  n    = 0;
  flag = FALSE;
  FILE_PATH_ITERATE(path,string,TRUE)
  {
    switch (n)
    {
      case 0: ASSERT_STR("/",String_cString(string)); break;
      case 1: ASSERT_STR("/abc",String_cString(string)); break;
      case 2: ASSERT_STR("/abc/def",String_cString(string)); break;
      case 3: ASSERT_STR("/abc/def/123",String_cString(string)); break;
    }
    n++;
    flag = TRUE;
  }
  ASSERT_TRUE(flag);

  flag = TRUE;
  String_setCString(path,"");
  FILE_PATH_ITERATE(path,string,FALSE)
  {
    flag = FALSE;
  }
  ASSERT_TRUE(flag);

  String_setCString(path,"abc/def");
  condition = TRUE;
  n         = 0;
  FILE_PATH_ITERATEX(path,string,FALSE,condition)
  {
    condition = FALSE;
    n++;
  }
  ASSERT_EQUAL(1,n);

  String_delete(string);
  String_delete(path);
}

CTEST(files,iteratePathCString)
{
  const char *cstring;
  size_t     n;
  bool       flag;
  bool       condition;

  n    = 0;
  flag = FALSE;
  FILE_PATH_ITERATE_CSTRING("abc/def/123",cstring,FALSE)
  {
    switch (n)
    {
      case 0: ASSERT_STR("abc",cstring); break;
      case 1: ASSERT_STR("def",cstring); break;
      case 2: ASSERT_STR("123",cstring); break;
    }
    n++;
    flag = TRUE;
  }
  ASSERT_TRUE(flag);

  n    = 0;
  flag = FALSE;
  FILE_PATH_ITERATE_CSTRING("abc/def/123",cstring,TRUE)
  {
    switch (n)
    {
      case 0: ASSERT_STR("abc",cstring); break;
      case 1: ASSERT_STR("abc/def",cstring); break;
      case 2: ASSERT_STR("abc/def/123",cstring); break;
    }
    n++;
    flag = TRUE;
  }
  ASSERT_TRUE(flag);

  n    = 0;
  flag = FALSE;
  FILE_PATH_ITERATE_CSTRING("/abc/def/123",cstring,FALSE)
  {
    switch (n)
    {
      case 0: ASSERT_STR("/",cstring); break;
      case 1: ASSERT_STR("abc",cstring); break;
      case 2: ASSERT_STR("def",cstring); break;
      case 3: ASSERT_STR("123",cstring); break;
    }
    n++;
    flag = TRUE;
  }
  ASSERT_TRUE(flag);

  n    = 0;
  flag = FALSE;
  FILE_PATH_ITERATE_CSTRING("/abc/def/123",cstring,TRUE)
  {
    switch (n)
    {
      case 0: ASSERT_STR("/",cstring); break;
      case 1: ASSERT_STR("/abc",cstring); break;
      case 2: ASSERT_STR("/abc/def",cstring); break;
      case 3: ASSERT_STR("/abc/def/123",cstring); break;
    }
    n++;
    flag = TRUE;
  }
  ASSERT_TRUE(flag);

  flag = TRUE;
  FILE_PATH_ITERATE_CSTRING("",cstring,FALSE)
  {
    flag = FALSE;
  }
  ASSERT_TRUE(flag);

  condition = TRUE;
  n         = 0;
  FILE_PATH_ITERATEX_CSTRING("abc/def",cstring,FALSE,condition)
  {
    condition = FALSE;
    n++;
  }
  ASSERT_EQUAL(1,n);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
