#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/misc.h"

// --- macros ----------------------------------------------------------

CTEST(misc,expandMacroInt)
{
  TextMacros (textMacros,1);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_INT("m",123,NULL);
  }

  String string = String_new();

  Misc_expandMacros(string,
                    "m=%m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=123",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:4",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m= 123",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:04",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=0123",String_cString(string));

  String_delete(string);
}

CTEST(misc,expandMacroUInt)
{
  TextMacros (textMacros,1);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_UINT("m",123,NULL);
  }

  String string = String_new();

  Misc_expandMacros(string,
                    "m=%m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=123",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:4",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m= 123",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:04",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=0123",String_cString(string));

  String_delete(string);
}

CTEST(misc,expandMacroInt64)
{
  TextMacros (textMacros,1);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_INT64("m",123456,NULL);
  }

  String string = String_new();

  Misc_expandMacros(string,
                    "m=%m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=123456",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:8",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=  123456",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:08",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=00123456",String_cString(string));

  String_delete(string);
}

CTEST(misc,expandMacroUInt64)
{
  TextMacros (textMacros,1);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_UINT64("m",123456,NULL);
  }

  String string = String_new();

  Misc_expandMacros(string,
                    "m=%m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=123456",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:8",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=  123456",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:08",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=00123456",String_cString(string));

  String_delete(string);
}

CTEST(misc,expandMacroDouble)
{
  TextMacros (textMacros,1);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_DOUBLE("m",123.456,NULL);
  }

  String string = String_new();

  Misc_expandMacros(string,
                    "m=%m:.3",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=123.456",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=123.456000",String_cString(string));

  String_delete(string);
}

CTEST(misc,expandMacroCString)
{
  TextMacros (textMacros,1);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_CSTRING("m","m",NULL);
  }

  String string = String_new();

  Misc_expandMacros(string,
                    "m=%m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=m",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:4",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=   m",String_cString(string));

  Misc_expandMacros(string,
                    "m=%m:-4",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=m   ",String_cString(string));

  String_delete(string);
}

CTEST(misc,expandMacroString)
{
  String m = String_newCString("m");

  TextMacros (textMacros,1);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_STRING("m",m,NULL);
  }

  String string = String_new();
  Misc_expandMacros(string,
                    "m=%m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=m",String_cString(string));
  String_delete(string);

  String_delete(m);
}

CTEST(misc,expandMacros)
{
  String m7 = String_newCString("m7");

  TextMacros (textMacros,7);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_INT    ("m1",123,    NULL);
    TEXT_MACRO_X_UINT   ("m2",123,    NULL);
    TEXT_MACRO_X_INT64  ("m3",123456, NULL);
    TEXT_MACRO_X_UINT64 ("m4",123456, NULL);
    TEXT_MACRO_X_DOUBLE ("m5",123.456,NULL);
    TEXT_MACRO_X_CSTRING("m6","m6",   NULL);
    TEXT_MACRO_X_STRING ("m7",m7,     NULL);
  }

  String string = String_new();
  Misc_expandMacros(string,
                    "m1=%m1 m2=%m2 m3=%m3 m4=%m4 m5=%m5 m6=%m6 m7=%m7",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m1=123 m2=123 m3=123456 m4=123456 m5=123.456000 m6=m6 m7=m7",String_cString(string));
  String_delete(string);

  String_delete(m7);
}

CTEST(misc,expandMacrosSpecial)
{
  String m7 = String_newCString("m7");

  TextMacros (textMacros,7);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_INT    ("m1",123,    NULL);
    TEXT_MACRO_X_UINT   ("m2",123,    NULL);
    TEXT_MACRO_X_INT64  ("m3",123456, NULL);
    TEXT_MACRO_X_UINT64 ("m4",123456, NULL);
    TEXT_MACRO_X_DOUBLE ("m5",123.456,NULL);
    TEXT_MACRO_X_CSTRING("m6","m6",   NULL);
    TEXT_MACRO_X_STRING ("m7",m7,     NULL);
  }

  String string;

  string = String_new();
  Misc_expandMacros(string,
                    "m1=%m1 m2=%m2 m3=%m3 m4=%m4 m5=%m5 m6=%m6 m7=%m7 %% %unknown",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m1=123 m2=123 m3=123456 m4=123456 m5=123.456000 m6=m6 m7=m7 %% %unknown",String_cString(string));
  String_delete(string);

  string = String_new();
  Misc_expandMacros(string,
                    "m1=%m1 m2=%m2 m3=%m3 m4=%m4 m5=%m5 m6=%m6 m7=%m7 %% %unknown",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    TRUE,
                    TRUE
                   ); 
  ASSERT_STR("m1=123 m2=123 m3=123456 m4=123456 m5=123.456000 m6=m6 m7=m7 % %unknown",String_cString(string));
  String_delete(string);

  string = String_new();
  Misc_expandMacros(string,
                    "m1=%m1 m2=%m2 m3=%m3 m4=%m4 m5=%m5 m6=%m6 m7=%m7 %% %unknown",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    TRUE,
                    FALSE
                   ); 
  ASSERT_STR("m1=123 m2=123 m3=123456 m4=123456 m5=123.456000 m6=m6 m7=m7 % ",String_cString(string));
  String_delete(string);

  String_delete(m7);
}

CTEST(misc,expandMacroCurlyBraces)
{
  TextMacros (textMacros,1);
  TEXT_MACROS_INIT(textMacros)
  {
    TEXT_MACRO_X_INT("m",123,NULL);
  }

  String string = String_new();

  Misc_expandMacros(string,
                    "m=%{m}m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=123m",String_cString(string));

  Misc_expandMacros(string,
                    "m=%{  m  }m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=123m",String_cString(string));

  Misc_expandMacros(string,
                    "m=%{m:4}m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m= 123m",String_cString(string));

  Misc_expandMacros(string,
                    "m=%{m:04}m",
                    EXPAND_MACRO_MODE_STRING,
                    textMacros.data,
                    textMacros.count,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=0123m",String_cString(string));

  String_delete(string);
}

CTEST(misc,expandMacroEmpty)
{
  String string = String_new();

  Misc_expandMacros(string,
                    "m=%:4",
                    EXPAND_MACRO_MODE_STRING,
                    NULL,
                    0,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=    ",String_cString(string));
  Misc_expandMacros(string,
                    "m=%:+4",
                    EXPAND_MACRO_MODE_STRING,
                    NULL,
                    0,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=    ",String_cString(string));
  Misc_expandMacros(string,
                    "m=%:-4",
                    EXPAND_MACRO_MODE_STRING,
                    NULL,
                    0,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=    ",String_cString(string));

  Misc_expandMacros(string,
                    "m=%:4.2",
                    EXPAND_MACRO_MODE_STRING,
                    NULL,
                    0,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=    ",String_cString(string));

  Misc_expandMacros(string,
                    "m=%  :4",
                    EXPAND_MACRO_MODE_STRING,
                    NULL,
                    0,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=    ",String_cString(string));
  Misc_expandMacros(string,
                    "m=%  :  4",
                    EXPAND_MACRO_MODE_STRING,
                    NULL,
                    0,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=    ",String_cString(string));
  Misc_expandMacros(string,
                    "m=%{  :  4  }",
                    EXPAND_MACRO_MODE_STRING,
                    NULL,
                    0,
                    FALSE,
                    TRUE
                   ); 
  ASSERT_STR("m=    ",String_cString(string));

  String_delete(string);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
