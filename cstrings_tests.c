#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/cstrings.h"

CTEST(cstrings,stringSet)
{
  char s[16];

  stringSet(s,sizeof(s),"0123456789");
  ASSERT_STR("0123456789",s);

  stringSet(s,sizeof(s),"01234567890123456789");
  ASSERT_STR("012345678901234",s);
}

CTEST(cstrings,stringSetBuffer)
{
  const char *BUFFER = "012345678901234";

  char s[16];

  stringSetBuffer(s,sizeof(s),BUFFER,5);
  ASSERT_STR("01234",s);

  stringSetBuffer(s,sizeof(s),BUFFER,15);
  ASSERT_STR("012345678901234",s);

  stringSetBuffer(s,sizeof(s),BUFFER,20);
  ASSERT_STR("012345678901234",s);
}

CTEST(cstrings,stringClear)
{
  char s[16];

  stringSet(s,sizeof(s),"0123456789");
  stringClear(s);
  ASSERT_STR("",s);
}

CTEST(cstrings,stringLength)
{
  char s[16];

  stringSet(s,sizeof(s),"0123456789");
  ASSERT_EQUAL(stringLength(s),10);
  stringSet(s,sizeof(s),"01234567890123456789");
  ASSERT_EQUAL(stringLength(s),15);
}

CTEST(cstrings,stringCompare)
{
  char s[16];

  stringSet(s,sizeof(s),"0123456789");
  ASSERT_TRUE(stringCompare(s,"01234567890") < 0);
  ASSERT_TRUE(stringCompare(s,"0123456789") == 0);
  ASSERT_TRUE(stringCompare(s,"012345678") >= 1);

  ASSERT_TRUE(stringCompare(NULL,"a") > 0);
  ASSERT_TRUE(stringCompare("a",NULL) < 0);
  ASSERT_TRUE(stringCompare(NULL,NULL) == 0);
}

CTEST(cstrings,stringEquals)
{
  char s[16];

  ASSERT_TRUE(stringEquals(NULL,NULL));
  ASSERT_TRUE(stringEquals("1234","1234"));

  stringSet(s,sizeof(s),"abc");
  ASSERT_TRUE(stringEquals(s,"abc"));
  ASSERT_FALSE(stringEquals(s,"ab"));
  ASSERT_FALSE(stringEquals(s,"abcd"));
  ASSERT_FALSE(stringEquals(s,"aXc"));
}

CTEST(cstrings,stringEqualsPrefix)
{
  char s[16];

  ASSERT_TRUE(stringEqualsPrefix(NULL,NULL,1));
  ASSERT_TRUE(stringEqualsPrefix("1234","1234",1));

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_TRUE(stringEqualsPrefix(s,"01234",5));
  ASSERT_FALSE(stringEqualsPrefix(s,"012345",6));
}

CTEST(cstrings,stringEqualsIgnoreCase)
{
  char s[16];

  ASSERT_TRUE(stringEqualsPrefix(NULL,NULL,1));
  ASSERT_TRUE(stringEqualsPrefix("1234","1234",1));

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_TRUE(stringEqualsIgnoreCase(s,"01234abcde"));
  ASSERT_TRUE(stringEqualsIgnoreCase(s,"01234ABCDE"));
}

CTEST(cstrings,stringEqualsPrefixIgnoreCase)
{
  char s[16];

  ASSERT_TRUE(stringEqualsPrefixIgnoreCase(NULL,NULL,1));
  ASSERT_TRUE(stringEqualsPrefixIgnoreCase("1234","1234",1));

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_TRUE(stringEqualsPrefixIgnoreCase(s,"01234abcde",6));
  ASSERT_TRUE(stringEqualsPrefixIgnoreCase(s,"01234ABCDE",6));
  ASSERT_FALSE(stringEqualsPrefixIgnoreCase(s,"012345ABCDE",7));
}

CTEST(cstrings,stringStartsWith)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_TRUE(stringStartsWith(s,"01234"));
  ASSERT_FALSE(stringStartsWith(s,"012345"));
}

CTEST(cstrings,stringStartsWithIgnoreCase)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_TRUE(stringStartsWithIgnoreCase(s,"01234a"));
  ASSERT_TRUE(stringStartsWithIgnoreCase(s,"01234A"));
  ASSERT_FALSE(stringStartsWithIgnoreCase(s,"0123456"));
}

CTEST(cstrings,stringEndsWith)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_TRUE(stringEndsWith(s,"abcde"));
  ASSERT_FALSE(stringEndsWith(s,"abcdef"));
}

CTEST(cstrings,stringEndsWithIgnoreCase)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_TRUE(stringEndsWithIgnoreCase(s,"abcde"));
  ASSERT_TRUE(stringEndsWithIgnoreCase(s,"ABCDE"));
  ASSERT_FALSE(stringEndsWithIgnoreCase(s,"ABCDEF"));
}

CTEST(cstrings,stringIsEmpty)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_FALSE(stringIsEmpty(s));
  stringClear(s);
  ASSERT_TRUE(stringIsEmpty(s));
}

static bool checkPad(const char *pad)
{
  for (size_t i = 0; i < 16; i++)
  {
    if ((int8_t)pad[i] != (int8_t)0xFE) return FALSE;
  }

  return TRUE;
}

CTEST(cstrings,stringReplace)
{
  struct
  {
    char fence0[16];
    char s[10];
    char fence1[16];
  } __attribute((packed)) s10;

  memFill(s10.fence0,16,0xFE);
  ASSERT_TRUE(checkPad(s10.fence0));
  memFill(s10.fence1,16,0xFE);
  ASSERT_TRUE(checkPad(s10.fence1));

  // short string
  stringSet(s10.s,sizeof(s10.s),"abcdefgh");
  ASSERT_STR("abcdefgh",s10.s);
  stringReplace(s10.s,sizeof(s10.s), 2, 2, "XYZ");
  ASSERT_STR("abXYZefgh",s10.s);
  ASSERT_TRUE(checkPad(s10.fence0));
  ASSERT_TRUE(checkPad(s10.fence1));

  stringSet(s10.s,sizeof(s10.s),"abcdefgh");
  ASSERT_STR("abcdefgh",s10.s);
  stringReplace(s10.s,sizeof(s10.s), 2, 2, "XYZWVU");
  ASSERT_STR("abXYZWVUe",s10.s);
  ASSERT_TRUE(checkPad(s10.fence0));
  ASSERT_TRUE(checkPad(s10.fence1));

  struct
  {
    char fence0[16];
    char s[12];
    char fence1[16];
  } __attribute((packed)) s12;

  memFill(s12.fence0,16,0xFE);
  ASSERT_TRUE(checkPad(s12.fence0));
  memFill(s12.fence1,16,0xFE);
  ASSERT_TRUE(checkPad(s12.fence1));

  stringSet(s12.s,sizeof(s12.s),"Hello World");
  ASSERT_STR("Hello World",s12.s);
  stringReplace(s12.s,sizeof(s12.s),7,1,"12345678");
  ASSERT_STR("Hello W1234",s12.s);
  ASSERT_TRUE(checkPad(s12.fence0));
  ASSERT_TRUE(checkPad(s12.fence1));

  stringSet(s12.s,sizeof(s12.s),"Hello World");
  ASSERT_STR("Hello World",s12.s);
  stringReplace(s12.s,sizeof(s12.s),7,8,"12345678");
  ASSERT_STR("Hello W1234",s12.s);
  ASSERT_TRUE(checkPad(s12.fence0));
  ASSERT_TRUE(checkPad(s12.fence1));

  stringSet(s12.s,sizeof(s12.s),"Hello World");
  ASSERT_STR("Hello World",s12.s);
  stringReplace(s12.s, sizeof(s12.s), 2, 10, "X");
  ASSERT_STR("HeX",s12.s);
  ASSERT_TRUE(checkPad(s12.fence0));
  ASSERT_TRUE(checkPad(s12.fence1));

  stringSet(s12.s,sizeof(s12.s),"Hello World");
  ASSERT_STR("Hello World",s12.s);
  stringReplace(s12.s, sizeof(s12.s), 2, 10, "XYZ");
  ASSERT_STR("HeXYZ",s12.s);
  ASSERT_TRUE(checkPad(s12.fence0));
  ASSERT_TRUE(checkPad(s12.fence1));

  struct
  {
    char fence0[16];
    char s[16];
    char fence1[16];
  } __attribute((packed)) s16;

  memFill(s16.fence0,16,0xFE);
  ASSERT_TRUE(checkPad(s16.fence0));
  memFill(s16.fence1,16,0xFE);
  ASSERT_TRUE(checkPad(s16.fence1));

  // replace with ""/NULL
  stringSet(s16.s,sizeof(s16.s),"01234abcde");
  ASSERT_STR("01234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),0,0,"");
  ASSERT_STR("01234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),2,2,NULL);
  ASSERT_STR("014abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),0,8,NULL);
  ASSERT_STR("",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  // replace n-by-n
  stringSet(s16.s,sizeof(s16.s),"01234abcde");
  ASSERT_STR("01234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),0,2,"XX");
  ASSERT_STR("XX234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),1,2,"YY");
  ASSERT_STR("XYY34abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),0,2,"ZZ");
  ASSERT_STR("ZZY34abcde",s16.s);

  // extend string
  stringSet(s16.s,sizeof(s16.s),"01234abcde");
  ASSERT_STR("01234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),0,1,"AB");
  ASSERT_STR("AB1234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),1,1,"uv");
  ASSERT_STR("Auv1234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),11,1,"XY");
  ASSERT_STR("Auv1234abcdXY",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  // shrink string
  stringSet(s16.s,sizeof(s16.s),"01234abcde");
  ASSERT_STR("01234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),1,4,"A");
  ASSERT_STR("0Aabcde",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  stringSet(s16.s,sizeof(s16.s),"01234abcde");
  ASSERT_STR("01234abcde",s16.s);
  stringReplace(s16.s,sizeof(s16.s),1,4,"ABCDEF");
  ASSERT_STR("0ABCDEFabcde",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  // replace out-of-bounds
  stringSet(s16.s,sizeof(s16.s),"01234");
  ASSERT_STR("01234",s16.s);
  stringReplace(s16.s,sizeof(s16.s),6,1,"A");
  ASSERT_STR("01234",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  // string too long
  stringSet(s16.s,sizeof(s16.s),"01234");
  ASSERT_STR("01234",s16.s);
  stringReplace(s16.s,sizeof(s16.s),1,1,"ABCDEFGHIJKLMNO");
  ASSERT_STR("0ABCDEFGHIJKLMN",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  stringSet(s16.s,sizeof(s16.s),"0123456789");
  ASSERT_STR("0123456789",s16.s);
  stringReplace(s16.s,sizeof(s16.s),1,1,"ABCDEFGHIJKLMNO");
  ASSERT_STR("0ABCDEFGHIJKLMN",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  stringSet(s16.s,sizeof(s16.s),"0123456789");
  ASSERT_STR("0123456789",s16.s);
  stringReplace(s16.s,sizeof(s16.s),4,1,"ABCD");
  ASSERT_STR("0123ABCD56789",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  stringSet(s16.s,sizeof(s16.s),"0123456789");
  ASSERT_STR("0123456789",s16.s);
  stringReplace(s16.s,sizeof(s16.s),6,10,"abcd");
  ASSERT_STR("012345abcd",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  stringSet(s16.s,sizeof(s16.s),"0123456789");
  ASSERT_STR("0123456789",s16.s);
  stringReplace(s16.s,sizeof(s16.s),6,10,"abcdefghijk");
  ASSERT_STR("012345abcdefghi",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  stringSet(s16.s,sizeof(s16.s),"0123456789");
  ASSERT_STR("0123456789",s16.s);
  stringReplace(s16.s,sizeof(s16.s),10,3,"abcdefgh");
  ASSERT_STR("0123456789abcde",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));


  stringSet(s16.s,sizeof(s16.s),"abcdefgh");
  ASSERT_STR("abcdefgh",s16.s);
  stringReplace(s16.s,sizeof(s16.s), 2, 2, "XYZ");
  ASSERT_STR("abXYZefgh",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));

  stringSet(s16.s,sizeof(s16.s),"abcdefgh");
  ASSERT_STR("abcdefgh",s16.s);
  stringReplace(s16.s,sizeof(s16.s), 2, 2, "XYZWVU");
  ASSERT_STR("abXYZWVUefgh",s16.s);
  ASSERT_TRUE(checkPad(s16.fence0));
  ASSERT_TRUE(checkPad(s16.fence1));
}

CTEST(cstrings,stringIntLength)
{
  ASSERT_EQUAL(1,stringIntLength(0));
  ASSERT_EQUAL(1,stringIntLength(9));
  ASSERT_EQUAL(2,stringIntLength(10));
  ASSERT_EQUAL(3,stringIntLength(-10));
}

CTEST(cstrings,stringInt64Length)
{
  ASSERT_EQUAL(1,stringIntLength(0LL));
  ASSERT_EQUAL(1,stringIntLength(9LL));
  ASSERT_EQUAL(2,stringIntLength(10LL));
  ASSERT_EQUAL(3,stringIntLength(-10LL));
}

CTEST(cstrings,stringFormat)
{
  char s[16];

  stringFormat(s,sizeof(s),"abc%d",123);
  ASSERT_STR("abc123",s);
  stringFormat(s,sizeof(s),"01234abcdeXXX%d",123);
  ASSERT_STR("01234abcdeXXX12",s);
}

static void test_stringVFormat(const char *format, ...)
{
  char s[16];

  va_list arguments;

  va_start(arguments,format);
  stringVFormat(s,sizeof(s),"abc%d",arguments);
  ASSERT_STR("abc123",s);
  va_end(arguments);

  va_start(arguments,format);
  stringVFormat(s,sizeof(s),"01234abcdeXXX%d",arguments);
  ASSERT_STR("01234abcdeXXX12",s);
  va_end(arguments);
}

CTEST(cstrings,stringVFormat)
{
  test_stringVFormat("",123);
}

CTEST(cstrings,stringFormatLength)
{
  ASSERT_EQUAL(6,stringFormatLength("abc%d",123));
  ASSERT_EQUAL(16,stringFormatLength("01234abcdeXXX%d",123));
}

static void test_stringVFormatLength(const char *format, ...)
{
  va_list arguments;

  va_start(arguments,format);
  ASSERT_EQUAL(6,stringVFormatLength("abc%d",arguments));
  va_end(arguments);

  va_start(arguments,format);
  ASSERT_EQUAL(16,stringVFormatLength("01234abcdeXXX%d",arguments));
  va_end(arguments);
}

CTEST(cstrings,stringVFormatLength)
{
  test_stringVFormatLength("",123);
}

CTEST(cstrings,stringAppend)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_STR("01234abcde",s);
  stringAppend(s,sizeof(s),"XXX");
  ASSERT_STR("01234abcdeXXX",s);
  stringAppend(s,sizeof(s),"0123456789");
  ASSERT_STR("01234abcdeXXX01",s);
}

CTEST(cstrings,stringAppendChar)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_STR("01234abcde",s);
  stringAppendChar(s,sizeof(s),'X');
  ASSERT_STR("01234abcdeX",s);
}

CTEST(cstrings,stringAppendBuffer)
{
  const char *BUFFER = "01234";

  char s[16];

  stringSet(s,sizeof(s),"abcde");
  ASSERT_STR("abcde",s);
  stringAppendBuffer(s,sizeof(s),BUFFER,5);
  ASSERT_STR("abcde01234",s);

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_STR("01234abcde",s);
  stringAppendBuffer(s,sizeof(s),BUFFER,5);
  ASSERT_STR("01234abcde01234",s);

  stringSet(s,sizeof(s),"01234abcde");
  ASSERT_STR("01234abcde",s);
  stringAppendBuffer(s,sizeof(s),BUFFER,3);
  ASSERT_STR("01234abcde012",s);
  stringAppendBuffer(s,sizeof(s),BUFFER,3);
  ASSERT_STR("01234abcde01201",s);
}

CTEST(cstrings,stringAppendFormat)
{
  char s[16];

  stringSet(s,sizeof(s),"abc");
  ASSERT_STR("abc",s);
  stringAppendFormat(s,sizeof(s),"%d",123);
  ASSERT_STR("abc123",s);

  stringSet(s,sizeof(s),"01234abcdeXXX");
  ASSERT_STR("01234abcdeXXX",s);
  stringAppendFormat(s,sizeof(s),"%d",123);
  ASSERT_STR("01234abcdeXXX12",s);
}

static void test_stringAppendVFormat(const char *format, ...)
{
  char s[16];

  va_list arguments;

  stringSet(s,sizeof(s),"abc");
  ASSERT_STR("abc",s);
  va_start(arguments,format);
  stringAppendVFormat(s,sizeof(s),"%d",arguments);
  ASSERT_STR("abc123",s);
  va_end(arguments);

  stringSet(s,sizeof(s),"01234abcdeXXX");
  ASSERT_STR("01234abcdeXXX",s);
  va_start(arguments,format);
  stringAppendVFormat(s,sizeof(s),"%d",arguments);
  ASSERT_STR("01234abcdeXXX12",s);
  va_end(arguments);
}

CTEST(cstrings,stringAppendVFormat)
{
  test_stringAppendVFormat("",123);
}

CTEST(cstrings,stringFill)
{
  char s[16];

  stringClear(s);
  ASSERT_STR("",s);
  stringFill(s,sizeof(s),1,'0');
  ASSERT_STR("0",s);
  stringFill(s,sizeof(s),1,'1');
  ASSERT_STR("1",s);
  stringFill(s,sizeof(s),16,'2');
  ASSERT_STR("222222222222222",s);
}

CTEST(cstrings,stringFillAppend)
{
  char s[16];

  stringClear(s);
  ASSERT_STR("",s);
  stringFillAppend(s,sizeof(s),1,'0');
  ASSERT_STR("0",s);
  stringFillAppend(s,sizeof(s),1,'1');
  ASSERT_STR("01",s);
  stringFillAppend(s,sizeof(s),16,'2');
  ASSERT_STR("012222222222222",s);
}

CTEST(cstrings,stringTrim)
{
  char s[16];

  ASSERT_NULL(stringTrim(NULL));

  stringSet(s,sizeof(s),"  01234  ");
  ASSERT_STR("  01234  ",s);
  ASSERT_STR("01234",stringTrim(s));
}

CTEST(cstrings,stringTrimBegin)
{
  char s[16];

  ASSERT_NULL(stringTrimBegin(NULL));

  stringSet(s,sizeof(s),"  01234  ");
  ASSERT_STR("  01234  ",s);
  ASSERT_STR("01234  ",stringTrimBegin(s));
}

CTEST(cstrings,stringTrimEnd)
{
  char s[16];

  ASSERT_NULL(stringTrimEnd(NULL));

  stringSet(s,sizeof(s),"  01234  ");
  ASSERT_STR("  01234  ",s);
  ASSERT_STR("  01234",stringTrimEnd(s));
}

CTEST(cstrings,stringNewDelete)
{
  char *s;

  s = stringNew(5+1);
  ASSERT_NOT_NULL(s);
  stringSet(s,5+1,"01234");
  ASSERT_STR("01234",s);
  stringDelete(s);
}

CTEST(cstrings,stringNewBuffer)
{
  const char *BUFFER = "01234";

  char *s;

  s = stringNewBuffer(BUFFER,5);
  ASSERT_NOT_NULL(s);
  ASSERT_STR("01234",s);
  stringDelete(s);
}

CTEST(cstrings,stringDuplicate)
{
  char s[16],*t;

  stringSet(s,sizeof(s),"01234");
  t = stringDuplicate(s);
  ASSERT_NOT_NULL(t);
  ASSERT_STR("01234",t);
  stringDelete(t);
}

CTEST(cstrings,stringAt)
{
  char s[16];

  stringSet(s,sizeof(s),"01234");
  ASSERT_STR("01234",s);
  ASSERT_EQUAL('1',stringAt(s,1));
  ASSERT_FALSE(stringAt(s,1) == '2');
}

CTEST(cstrings,stringIsValidUTF8CodepointN)
{
  size_t n;

  ASSERT_TRUE(stringIsValidUTF8CodepointN("A",1,0,NULL));
  ASSERT_TRUE(stringIsValidUTF8CodepointN("A",1,0,&n));
  ASSERT_EQUAL(1,n);
  ASSERT_TRUE(stringIsValidUTF8CodepointN("ä",2,0,NULL));
  ASSERT_TRUE(stringIsValidUTF8CodepointN("ä",2,0,&n));
  ASSERT_EQUAL(2,n);
  ASSERT_TRUE(stringIsValidUTF8CodepointN("ää",4,0,NULL));
  ASSERT_TRUE(stringIsValidUTF8CodepointN("ää",4,2,&n));
  ASSERT_EQUAL(4,n);
  ASSERT_TRUE(stringIsValidUTF8CodepointN("€",3,0,NULL));
  ASSERT_TRUE(stringIsValidUTF8CodepointN("€",3,0,&n));
  ASSERT_EQUAL(3,n);
  ASSERT_TRUE(stringIsValidUTF8CodepointN("€€",6,3,NULL));
  ASSERT_TRUE(stringIsValidUTF8CodepointN("€€",6,3,&n));
  ASSERT_EQUAL(6,n);
  ASSERT_TRUE(stringIsValidUTF8CodepointN("𝄞",4,0,NULL));
  ASSERT_TRUE(stringIsValidUTF8CodepointN("𝄞",4,0,&n));
  ASSERT_EQUAL(4,n);
  ASSERT_TRUE(stringIsValidUTF8CodepointN("𝄞𝄞",8,4,NULL));
  ASSERT_TRUE(stringIsValidUTF8CodepointN("𝄞𝄞",8,4,&n));
  ASSERT_EQUAL(8,n);
  ASSERT_FALSE(stringIsValidUTF8CodepointN("\xC3\xA4",2,2,NULL));
  ASSERT_FALSE(stringIsValidUTF8CodepointN("\xB3\xA4",2,0,NULL));
}

CTEST(cstrings,stringIsValidUTF8Codepoint)
{
  size_t n;

  ASSERT_TRUE(stringIsValidUTF8Codepoint("A",0,NULL));
  ASSERT_TRUE(stringIsValidUTF8Codepoint("A",0,&n));
  ASSERT_EQUAL(1,n);
  ASSERT_TRUE(stringIsValidUTF8Codepoint("ä",0,NULL));
  ASSERT_TRUE(stringIsValidUTF8Codepoint("ä",0,&n));
  ASSERT_EQUAL(2,n);
  ASSERT_TRUE(stringIsValidUTF8Codepoint("€",0,NULL));
  ASSERT_TRUE(stringIsValidUTF8Codepoint("€",0,&n));
  ASSERT_EQUAL(3,n);
  ASSERT_TRUE(stringIsValidUTF8Codepoint("𝄞",0,NULL));
  ASSERT_TRUE(stringIsValidUTF8Codepoint("𝄞",0,&n));
  ASSERT_EQUAL(4,n);
  ASSERT_FALSE(stringIsValidUTF8Codepoint("\xC3\xA4",2,NULL));
  ASSERT_FALSE(stringIsValidUTF8Codepoint("\xB3\xA4",0,NULL));
}

CTEST(cstrings,stringIsValidUTF8)
{
  ASSERT_TRUE(stringIsValidUTF8("A",0));
  ASSERT_TRUE(stringIsValidUTF8("AA",1));
  ASSERT_TRUE(stringIsValidUTF8("ä",0));
  ASSERT_TRUE(stringIsValidUTF8("ää",2));
  ASSERT_TRUE(stringIsValidUTF8("€",0));
  ASSERT_TRUE(stringIsValidUTF8("€€",3));
  ASSERT_TRUE(stringIsValidUTF8("𝄞",0));
  ASSERT_TRUE(stringIsValidUTF8("𝄞𝄞",4));
  ASSERT_FALSE(stringIsValidUTF8("\xB3\xA4",0));
  ASSERT_FALSE(stringIsValidUTF8("\xB3\xB3\xA4",1));
}

CTEST(cstrings,stringMakeValidUTF8)
{
  char s[16];

  stringSet(s,sizeof(s),"A");
  ASSERT_EQUAL(1,stringLengthCodepointsUTF8(s));
  stringMakeValidUTF8(s,0);
  ASSERT_TRUE(stringIsValidUTF8(s,0));
  stringSet(s,sizeof(s),"AA");
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8(s));
  stringMakeValidUTF8(s,1);
  ASSERT_TRUE(stringIsValidUTF8(s,1));

  stringSet(s,sizeof(s),"ä");
  ASSERT_EQUAL(1,stringLengthCodepointsUTF8(s));
  stringMakeValidUTF8(s,0);
  ASSERT_TRUE(stringIsValidUTF8(s,0));
  stringSet(s,sizeof(s),"ää");
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8(s));
  stringMakeValidUTF8(s,2);
  ASSERT_TRUE(stringIsValidUTF8(s,2));

  stringSet(s,sizeof(s),"ä\xB3ä");
  stringMakeValidUTF8(s,0);
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8(s));
  ASSERT_TRUE(stringIsValidUTF8(s,0));

  stringSet(s,sizeof(s),"€");
  ASSERT_EQUAL(1,stringLengthCodepointsUTF8(s));
  stringMakeValidUTF8(s,0);
  ASSERT_TRUE(stringIsValidUTF8(s,0));
  stringSet(s,sizeof(s),"€€");
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8(s));
  stringMakeValidUTF8(s,3);
  ASSERT_TRUE(stringIsValidUTF8(s,3));

  stringSet(s,sizeof(s),"€\xB3€");
  stringMakeValidUTF8(s,0);
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8(s));
  ASSERT_TRUE(stringIsValidUTF8(s,0));

  stringSet(s,sizeof(s),"𝄞");
  ASSERT_EQUAL(1,stringLengthCodepointsUTF8(s));
  stringMakeValidUTF8(s,0);
  ASSERT_TRUE(stringIsValidUTF8(s,0));
  stringSet(s,sizeof(s),"𝄞𝄞");
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8(s));
  stringMakeValidUTF8(s,4);
  ASSERT_TRUE(stringIsValidUTF8(s,4));

  stringSet(s,sizeof(s),"𝄞\xB3𝄞");
  stringMakeValidUTF8(s,0);
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8(s));
  ASSERT_TRUE(stringIsValidUTF8(s,0));
}

CTEST(cstrings,stringNextUTF8N)
{
  size_t n = 0;
  n = stringNextUTF8N("Aä€𝄞",1+2+3+4,n);
  ASSERT_EQUAL(1,n);
  n = stringNextUTF8N("Aä€𝄞",1+2+3+4,n);
  ASSERT_EQUAL(3,n);
  n = stringNextUTF8N("Aä€𝄞",1+2+3+4,n);
  ASSERT_EQUAL(6,n);
  n = stringNextUTF8N("Aä€𝄞",1+2+3+4,n);
  ASSERT_EQUAL(10,n);
}

CTEST(cstrings,charUTF8Length)
{
  ASSERT_EQUAL(1,charUTF8Length(stringAtUTF8("A",0,NULL)));
  ASSERT_EQUAL(2,charUTF8Length(stringAtUTF8("ä",0,NULL)));
  ASSERT_EQUAL(3,charUTF8Length(stringAtUTF8("€",0,NULL)));
  ASSERT_EQUAL(4,charUTF8Length(stringAtUTF8("𝄞",0,NULL)));
}

CTEST(cstrings,isCharUTF8)
{
  ASSERT_FALSE(isCharUTF8(stringAtUTF8("A",0,NULL)));
  ASSERT_TRUE(isCharUTF8(stringAtUTF8("ä",0,NULL)));
  ASSERT_TRUE(isCharUTF8(stringAtUTF8("€",0,NULL)));
  ASSERT_TRUE(isCharUTF8(stringAtUTF8("𝄞",0,NULL)));
}

CTEST(cstrings,charUTF8)
{
  ASSERT_STR("A",charUTF8(stringAtUTF8("A",0,NULL)));
  ASSERT_STR("ä",charUTF8(stringAtUTF8("ä",0,NULL)));
  ASSERT_STR("€",charUTF8(stringAtUTF8("€",0,NULL)));
  ASSERT_STR("𝄞",charUTF8(stringAtUTF8("𝄞",0,NULL)));
}

CTEST(cstrings,stringAtUTF8N)
{
  ASSERT_STR("A",charUTF8(stringAtUTF8N("A",1,0,NULL)));
  ASSERT_STR("A",charUTF8(stringAtUTF8N("AA",2,1,NULL)));
  ASSERT_STR("ä",charUTF8(stringAtUTF8N("ä",2,0,NULL)));
  ASSERT_STR("ä",charUTF8(stringAtUTF8N("ää",4,2,NULL)));
  ASSERT_STR("€",charUTF8(stringAtUTF8N("€",3,0,NULL)));
  ASSERT_STR("€",charUTF8(stringAtUTF8N("€€",6,3,NULL)));
  ASSERT_STR("𝄞",charUTF8(stringAtUTF8N("𝄞",4,0,NULL)));
  ASSERT_STR("𝄞",charUTF8(stringAtUTF8N("𝄞𝄞",8,4,NULL)));
}

CTEST(cstrings,stringAtUTF8)
{
  ASSERT_STR("A",charUTF8(stringAtUTF8("A",0,NULL)));
  ASSERT_STR("A",charUTF8(stringAtUTF8("AA",1,NULL)));
  ASSERT_STR("ä",charUTF8(stringAtUTF8("ä",0,NULL)));
  ASSERT_STR("ä",charUTF8(stringAtUTF8("ää",2,NULL)));
  ASSERT_STR("€",charUTF8(stringAtUTF8("€",0,NULL)));
  ASSERT_STR("€",charUTF8(stringAtUTF8("€€",3,NULL)));
  ASSERT_STR("𝄞",charUTF8(stringAtUTF8("𝄞",0,NULL)));
  ASSERT_STR("𝄞",charUTF8(stringAtUTF8("𝄞𝄞",4,NULL)));
}

static void test_stringVFormatLengthCodepointsUTF8(size_t n, const char *format, ...)
{
  va_list arguments;

  va_start(arguments,format);
  ASSERT_EQUAL(n,stringVFormatLengthCodepointsUTF8(format,arguments));
  va_end(arguments);
}

CTEST(cstrings,stringVFormatLengthCodepointsUTF8)
{
  test_stringVFormatLengthCodepointsUTF8(4,"Aä€𝄞");
  test_stringVFormatLengthCodepointsUTF8(8,"A%cä%c€%c𝄞%c",'x','x','x','x');
}

CTEST(cstrings,stringFormatLengthCodepointsUTF8)
{
  ASSERT_EQUAL(4,stringFormatLengthCodepointsUTF8("Aä€𝄞"));
  ASSERT_EQUAL(8,stringFormatLengthCodepointsUTF8("A%cä%c€%c𝄞%c",'x','x','x','x'));
}

CTEST(cstrings,stringLengthCodepointsUTF8)
{
  ASSERT_EQUAL(1,stringLengthCodepointsUTF8("A"));
  ASSERT_EQUAL(1,stringLengthCodepointsUTF8("ä"));
  ASSERT_EQUAL(1,stringLengthCodepointsUTF8("€"));
  ASSERT_EQUAL(1,stringLengthCodepointsUTF8("𝄞"));

  ASSERT_EQUAL(2,stringLengthCodepointsUTF8("AA"));
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8("ää"));
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8("€€"));
  ASSERT_EQUAL(2,stringLengthCodepointsUTF8("𝄞𝄞"));

  ASSERT_EQUAL(4,stringLengthCodepointsUTF8("Aä€𝄞"));
}

CTEST(cstrings,stringFind)
{
  char s[16];

  stringSet(s,sizeof(s),"0123456789abcde");
  ASSERT_STR("0123456789abcde",s);
  ASSERT_EQUAL(1,stringFind(s,"123"));
  ASSERT_EQUAL(-1,stringFind(s,"hello"));
}

CTEST(cstrings,stringFindChar)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde01234");
  ASSERT_STR("01234abcde01234",s);
  ASSERT_EQUAL(3,stringFindChar(s,'3'));
  ASSERT_EQUAL(-1,stringFindChar(s,'X'));
}

CTEST(cstrings,stringFindReverseChar)
{
  char s[16];

  stringSet(s,sizeof(s),"01234abcde01234");
  ASSERT_STR("01234abcde01234",s);
  ASSERT_EQUAL(13,stringFindReverseChar(s,'3'));
  ASSERT_EQUAL(-1,stringFindReverseChar(s,'X'));
}

CTEST(cstrings,stringSub)
{
  char s[16];

  stringSub(s,sizeof(s),"01234abcde01234",2,3);
  ASSERT_STR("234",s);
  stringSub(s,sizeof(s),"01234abcde01234",13,3);
  ASSERT_STR("34",s);
}

CTEST(cstrings,stringIterator)
{
  CStringIterator stringIterator;

  stringIteratorInit(&stringIterator,"01234abcde01234");
  ASSERT_FALSE(stringIteratorEnd(&stringIterator));
  ASSERT_EQUAL('0',stringIteratorAtX(&stringIterator,0));
  ASSERT_EQUAL('1',stringIteratorAtX(&stringIterator,1));
  stringIteratorNextX(&stringIterator,1);
  ASSERT_EQUAL('1',stringIteratorAtX(&stringIterator,0));
  stringIteratorNextX(&stringIterator,2);
  ASSERT_EQUAL('3',stringIteratorAtX(&stringIterator,0));
  ASSERT_EQUAL('3',stringIteratorGetNext(&stringIterator));
  ASSERT_EQUAL('4',stringIteratorGetNext(&stringIterator));
  while (!stringIteratorEnd(&stringIterator))
  {
    stringIteratorNext(&stringIterator);
  }
  ASSERT_TRUE(stringIteratorEnd(&stringIterator));
  stringIteratorDone(&stringIterator);
}

CTEST(cstrings,stringTokenizer)
{
  CStringTokenizer stringTokenizer;
  const char       *t;

  stringTokenizerInit(&stringTokenizer,"123 abc xyz"," ");
  ASSERT_TRUE(stringGetNextToken(&stringTokenizer,&t));
  ASSERT_STR("123",t);
  ASSERT_TRUE(stringGetNextToken(&stringTokenizer,&t));
  ASSERT_STR("abc",t);
  ASSERT_TRUE(stringGetNextToken(&stringTokenizer,&t));
  ASSERT_STR("xyz",t);
  ASSERT_FALSE(stringGetNextToken(&stringTokenizer,&t));
  stringTokenizerDone(&stringTokenizer);
}

CTEST(cstrings,stringToBool)
{
  bool b;

  ASSERT_TRUE(stringToBool("0",&b));
  ASSERT_FALSE(b);
  ASSERT_TRUE(stringToBool("false",&b));
  ASSERT_FALSE(b);
  ASSERT_TRUE(stringToBool("off",&b));
  ASSERT_FALSE(b);
  ASSERT_TRUE(stringToBool("FALSE",&b));
  ASSERT_FALSE(b);
  ASSERT_TRUE(stringToBool("OFF",&b));
  ASSERT_FALSE(b);

  ASSERT_TRUE(stringToBool("1",&b));
  ASSERT_TRUE(b);
  ASSERT_TRUE(stringToBool("true",&b));
  ASSERT_TRUE(b);
  ASSERT_TRUE(stringToBool("on",&b));
  ASSERT_TRUE(b);
  ASSERT_TRUE(stringToBool("TRUE",&b));
  ASSERT_TRUE(b);
  ASSERT_TRUE(stringToBool("ON",&b));
  ASSERT_TRUE(b);

  ASSERT_FALSE(stringToBool("hello",&b));
}

CTEST(cstrings,stringToInt)
{
  int        i;
  const char *t;

  ASSERT_TRUE(stringToInt("123",&i,NULL));
  ASSERT_EQUAL(123,i);
  ASSERT_TRUE(stringToInt("-123",&i,NULL));
  ASSERT_EQUAL(-123,i);
  ASSERT_TRUE(stringToInt("123",&i,&t));
  ASSERT_EQUAL(123,i);
  ASSERT_STR("",t);
  ASSERT_TRUE(stringToInt("123abc",&i,&t));
  ASSERT_EQUAL(123,i);
  ASSERT_STR("abc",t);
  ASSERT_FALSE(stringToInt("123abc",&i,NULL));
}

CTEST(cstrings,stringToUInt)
{
  uint       u;
  const char *t;

  ASSERT_TRUE(stringToUInt("123",&u,NULL));
  ASSERT_EQUAL(123,u);
  ASSERT_TRUE(stringToUInt("123",&u,&t));
  ASSERT_EQUAL(123,u);
  ASSERT_STR("",t);
  ASSERT_TRUE(stringToUInt("123abc",&u,&t));
  ASSERT_EQUAL(123,u);
  ASSERT_STR("abc",t);
  ASSERT_FALSE(stringToUInt("123abc",&u,NULL));
}

CTEST(cstrings,stringToInt64)
{
  int64      i64;
  const char *t;

  ASSERT_TRUE(stringToInt64("123",&i64,NULL));
  ASSERT_EQUAL(123,i64);
  ASSERT_TRUE(stringToInt64("-123",&i64,NULL));
  ASSERT_EQUAL(-123,i64);
  ASSERT_TRUE(stringToInt64("123",&i64,&t));
  ASSERT_EQUAL(123,i64);
  ASSERT_STR("",t);
  ASSERT_TRUE(stringToInt64("123abc",&i64,&t));
  ASSERT_EQUAL(123,i64);
  ASSERT_STR("abc",t);
  ASSERT_FALSE(stringToInt64("123abc",&i64,NULL));
}

CTEST(cstrings,stringToUInt64)
{
  uint64     u64;
  const char *t;

  ASSERT_TRUE(stringToUInt64("123",&u64,NULL));
  ASSERT_EQUAL(123,u64);
  ASSERT_TRUE(stringToUInt64("123",&u64,&t));
  ASSERT_EQUAL(123,u64);
  ASSERT_STR("",t);
  ASSERT_TRUE(stringToUInt64("123abc",&u64,&t));
  ASSERT_EQUAL(123,u64);
  ASSERT_STR("abc",t);
  ASSERT_FALSE(stringToUInt64("123abc",&u64,NULL));
}

CTEST(cstrings,stringToDouble)
{
  double     d;
  const char *t;

  ASSERT_TRUE(stringToDouble("123.456",&d,NULL));
  ASSERT_EQUAL(123.456,d);
  ASSERT_TRUE(stringToDouble("123.456",&d,&t));
  ASSERT_EQUAL(123.456,d);
  ASSERT_STR("",t);
  ASSERT_TRUE(stringToDouble("123.456abc",&d,&t));
  ASSERT_EQUAL(123.456,d);
  ASSERT_STR("abc",t);
  ASSERT_FALSE(stringToDouble("123.456abc",&d,NULL));
}

CTEST(cstrings,stringScan)
{
  int    i;
  int64  i64;
  double d;

  ASSERT_TRUE(stringScan("123","%d",&i,NULL));
  ASSERT_EQUAL(123,i);
  ASSERT_TRUE(stringScan("123","%lld",&i64,NULL));
  ASSERT_EQUAL(123,i64);
  ASSERT_TRUE(stringScan("123.456","%lf",&d,NULL));
  ASSERT_EQUAL(123.456,d);

  ASSERT_FALSE(stringScan("abc","%d",&i,NULL));
}

static void test_stringVScanInt(const char *string, const char *format, ...)
{
  va_list arguments;

  va_start(arguments,format);
  ASSERT_TRUE(stringVScan(string,"%d",arguments));
  va_end(arguments);
}
static void test_stringVScanInt64(const char *string, const char *format, ...)
{
  va_list arguments;

  va_start(arguments,format);
  ASSERT_TRUE(stringVScan(string,"%lld",arguments));
  va_end(arguments);
}
static void test_stringVScanDouble(const char *string, const char *format, ...)
{
  va_list arguments;

  va_start(arguments,format);
  ASSERT_TRUE(stringVScan(string,"%lf",arguments));
  va_end(arguments);
}

CTEST(cstrings,stringVScan)
{
  int     i;
  int64   i64;
  double  d;

  test_stringVScanInt("123","",&i,NULL);
  ASSERT_EQUAL(123,i);
  test_stringVScanInt64("123","",&i64,NULL);
  ASSERT_EQUAL(123,i);
  test_stringVScanDouble("123.456","",&d,NULL);
  ASSERT_EQUAL(123.456,d);
}

CTEST(cstrings,stringMatch)
{
  const char *t;
  size_t     n;

  ASSERT_TRUE(stringMatch("abc",".*",&t,&n,NULL));
  ASSERT_TRUE(stringEqualsPrefix(t,"abc",3));
  ASSERT_TRUE(stringMatch("abc","[a-z]+",&t,&n,NULL));
  ASSERT_TRUE(stringEqualsPrefix(t,"abc",3));
  ASSERT_TRUE(stringMatch("abc","a+",&t,&n,NULL));
  ASSERT_TRUE(stringEqualsPrefix(t,"a",1));
}

static void test_stringVMatch(const char *format, ...)
{
  const char *t;
  size_t     n;

  va_list arguments;

  va_start(arguments,format);
  ASSERT_TRUE(stringVMatch("abc",".*",&t,&n,arguments));
  ASSERT_TRUE(stringEqualsPrefix(t,"abc",3));
  va_end(arguments);

  va_start(arguments,format);
  ASSERT_TRUE(stringVMatch("abc","[a-z]+",&t,&n,arguments));
  ASSERT_TRUE(stringEqualsPrefix(t,"abc",3));
  va_end(arguments);

  va_start(arguments,format);
  ASSERT_TRUE(stringVMatch("abc","a+",&t,&n,arguments));
  ASSERT_TRUE(stringEqualsPrefix(t,"a",1));
  va_end(arguments);
}

CTEST(cstrings,stringVMatch)
{
  char   *t;
  size_t n;

  test_stringVMatch("",&t,&n,NULL);
}

CTEST(cstrings,stringSimpleHash)
{
  stringSimpleHash("012345");
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
