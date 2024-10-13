#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/global.h"
#include "common/cmdoptions.h"
#include "common/strings.h"

typedef enum
{
  ENUM_A,
  ENUM_B
} Enumeration;

int         integer;
int64       integer64;
double      double_;
float       float_;
bool        boolean;
bool        flag;
int         increment;
uint        select_;
const char  *cstring;
Enumeration enumeration;
String      string;
int         special;
int         deprecated;

const CommandLineOptionSelect SELECT[] = CMD_VALUE_SELECT_ARRAY
(
  {"a",1,"a"},
  {"b",2,"b"},
);

LOCAL bool cmdOptionParseSpecial(void *userData, void *variable, const char *name, const char *value, const void *defaultValue, char errorMessage[], uint errorMessageSize)
{
  UNUSED_VARIABLE(userData);
  UNUSED_VARIABLE(name);
  UNUSED_VARIABLE(value);
  UNUSED_VARIABLE(defaultValue);
  UNUSED_VARIABLE(errorMessage);
  UNUSED_VARIABLE(errorMessageSize);

  (*(int*)variable) = 1;

  return TRUE;
}

LOCAL bool cmdOptionParseDeprecated(void *userData, void *variable, const char *name, const char *value, const void *defaultValue, char errorMessage[], uint errorMessageSize)
{
  UNUSED_VARIABLE(userData);
  UNUSED_VARIABLE(variable);
  UNUSED_VARIABLE(name);
  UNUSED_VARIABLE(value);
  UNUSED_VARIABLE(defaultValue);
  UNUSED_VARIABLE(errorMessage);
  UNUSED_VARIABLE(errorMessageSize);

  deprecated = 1;

  return TRUE;
}

CommandLineOption COMMAND_LINE_OPTIONS[] = CMD_VALUE_ARRAY
(
  CMD_OPTION_INTEGER      ("integer",      'i',0,1,integer,0,123,NULL,"integer"),
  CMD_OPTION_INTEGER64    ("integer64",    'I',0,1,integer64,0,123,NULL,"integer64"),
  CMD_OPTION_DOUBLE       ("double",       'd',0,1,double_,0.0,123.0,NULL,"double"),
  CMD_OPTION_BOOLEAN      ("boolean",      'b',0,1,boolean,"boolean"),
  CMD_OPTION_FLAG         ("flag",         'f',0,1,flag,FALSE,"flag"),
  CMD_OPTION_INCREMENT    ("increment",    'k',0,1,increment,0,2,"increment"),
  CMD_OPTION_SELECT       ("select",       's',0,1,select_,SELECT,"select","argument","default"),
  CMD_OPTION_CSTRING      ("cstring",      'c',0,1,cstring,"","cstring"),
  CMD_OPTION_ENUM         ("enumeration-A",'A',0,1,enumeration,ENUM_A,"enum A"),
  CMD_OPTION_ENUM         ("enumeration-B",'B',0,1,enumeration,ENUM_B,"enum B"),
  CMD_OPTION_STRING       ("string",       'S',0,1,string,"string",""),
  CMD_OPTION_SPECIAL      ("special",      'p',0,1,&special,cmdOptionParseSpecial,NULL,1,"special",""),
  CMD_OPTION_DEPRECATED   ("deprecated",   'D',0,1,&deprecated,cmdOptionParseDeprecated,NULL,1,"deprecated"),
);

CTEST(cmdoptions,init)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,integer)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(0,integer);

  const char *argv[] = {"","--integer=123","-i","123"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(123,integer);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,integer64)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(0,integer64);

  const char *argv[] = {"","--integer64=123","-I","123"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(123,integer64);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,double_)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(0,double_);

  const char *argv[] = {"","--double=123","-d","123"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(123,double_);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,boolean)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(FALSE,boolean);

  const char *argv[] = {"","--boolean","-b"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(TRUE,boolean);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

#if 0
CTEST(cmdoptions,flag)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(FALSE,flag);

  const char *argv[] = {"","--flag","-f"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(TRUE,flag);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}
#endif

CTEST(cmdoptions,increment)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(0,increment);

  const char *argv[] = {"","--increment","-k"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(2,increment);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,select)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(0,select_);

  const char *argv[] = {"","--select=b","-s","b"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(2,select_);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,cstring)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_STR(NULL,cstring);

  const char *argv[] = {"","--cstring=foo","-c","foo"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_STR("foo",cstring);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,enumeration)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(ENUM_A,enumeration);

  const char *argv[] = {"","--enumeration-B","-B"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(ENUM_B,enumeration);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,string)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_STR("",String_cString(string));

  const char *argv[] = {"","--string=foo","-S","foo"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_STR("foo",String_cString(string));
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,special)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(0,special);

  const char *argv[] = {"","--special=123","-p","123"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,stderr,"ERROR: ","Warning: ");
  ASSERT_EQUAL(1,special);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

CTEST(cmdoptions,deprecated)
{
  ASSERT_TRUE(CmdOption_init(COMMAND_LINE_OPTIONS));

  ASSERT_EQUAL(0,deprecated);

  const char *argv[] = {"","--deprecated=123","-D","123"};
  int        argc    = SIZE_OF_ARRAY(argv);
  CmdOption_parse(argv,&argc,COMMAND_LINE_OPTIONS,CMD_PRIORITY_ANY,CMD_PRIORITY_ANY,NULL,NULL,NULL);
  ASSERT_EQUAL(1,deprecated);
  ASSERT_EQUAL(argc,1);

  CmdOption_done(COMMAND_LINE_OPTIONS);
}

int main(int argc, const char *argv[])
{
  string = String_new();
  int exitcode = ctest_main(argc, argv);
  String_delete(string);

  return exitcode;
}
