#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/strings.h"

CTEST(strings,new_delete)
{
  String s;
  char   buffer[4+1];

  s = String_new();
  ASSERT_STR("",String_cString(s));
  String_delete(s);

  s = String_newCString("test");
  ASSERT_STR("test",String_cString(s));
  String_delete(s);

  s = String_newChar('t');
  ASSERT_STR("t",String_cString(s));
  String_delete(s);

  strcpy(buffer,"test");
  s = String_newBuffer(buffer,4);
  ASSERT_STR("test",String_cString(s));
  String_delete(s);
}

CTEST(strings,duplicate)
{
  String s1,s2;

  s1 = String_new();
  s2 = String_new();

  s1 = String_newCString("test");
  ASSERT_STR("test",String_cString(s1));
  s2 = String_duplicate(s1);
  ASSERT_STR("test",String_cString(s2));

  String_delete(s2);
  String_delete(s1);
}

CTEST(strings,copy)
{
  String s1,s2;

  s1 = String_new();
  s2 = String_new();

  s1 = String_newCString("test");
  ASSERT_STR("test",String_cString(s1));
  s2 = String_copy(String_new(),s1);
  ASSERT_STR("test",String_cString(s2));

  String_delete(s2);
  String_delete(s1);
}

CTEST(strings,clear)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  String_clear(s);
  ASSERT_STR("",String_cString(s));
  String_delete(s);
}

CTEST(strings,erase)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  String_erase(s);
  ASSERT_STR("",String_cString(s));
  String_delete(s);
}

CTEST(strings,set)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();
  ASSERT_STR("",String_cString(s));

  t = String_newCString("test");
  String_set(s,t);
  ASSERT_STR("test",String_cString(s));
  String_delete(t);

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  String_setChar(s,'t');
  ASSERT_STR("t",String_cString(s));

  strcpy(buffer,"test");
  String_setBuffer(s,buffer,4);
  ASSERT_STR("test",String_cString(s));

  String_delete(s);
}

LOCAL void vformat(String s, char *format, ...)
{
  va_list arguments;

  va_start(arguments,format);
  String_vformat(s,format,arguments);
  va_end(arguments);
}

LOCAL void appendVFormat(String s, char *format, ...)
{
  va_list arguments;

  va_start(arguments,format);
  String_appendVFormat(s,format,arguments);
  va_end(arguments);
}

CTEST(strings,format)
{
  String  s;

  s = String_new();
  ASSERT_STR("",String_cString(s));

  String_format(s,"test %02d %2.2lf %s",12,34.56,"test");
  ASSERT_STR("test 12 34.56 test",String_cString(s));

  String_appendFormat(s,"test %02d %2.2lf %s",12,34.56,"test");
  ASSERT_STR("test 12 34.56 testtest 12 34.56 test",String_cString(s));

  vformat(s,"test %02d %2.2lf %s",12,34.56,"test");
  ASSERT_STR("test 12 34.56 test",String_cString(s));

  appendVFormat(s,"test %02d %2.2lf %s",12,34.56,"test");
  ASSERT_STR("test 12 34.56 testtest 12 34.56 test",String_cString(s));

  String_delete(s);
}

CTEST(strings,append)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();
  ASSERT_STR("",String_cString(s));

  t = String_newCString("test");
  String_append(s,t);
  ASSERT_STR("test",String_cString(s));
  String_delete(t);

  t = String_newCString("test");
  String_appendSub(s,t,1,2);
  ASSERT_STR("testes",String_cString(s));
  String_delete(t);

  String_appendCString(s,"test");
  ASSERT_STR("testestest",String_cString(s));

  String_appendChar(s,'t');
  ASSERT_STR("testestestt",String_cString(s));

  String_appendCharUTF8(s,'t');
  ASSERT_STR("testestesttt",String_cString(s));

  strcpy(buffer,"test");
  String_appendBuffer(s,buffer,4);
  ASSERT_STR("testestesttttest",String_cString(s));

  String_delete(s);
}

CTEST(strings,insert)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("test");
  String_insert(s,1,t);
  ASSERT_STR("ttestest",String_cString(s));
  String_delete(t);

  t = String_newCString("test");
  String_insertSub(s,1,t,1,2);
  ASSERT_STR("testestest",String_cString(s));
  String_delete(t);

  String_insertCString(s,1,"test");
  ASSERT_STR("ttestestestest",String_cString(s));

  String_insertChar(s,1,'t');
  ASSERT_STR("tttestestestest",String_cString(s));

  strcpy(buffer,"test");
  String_insertBuffer(s,1,buffer,4);
  ASSERT_STR("ttestttestestestest",String_cString(s));

  String_delete(s);
}

CTEST(strings,remove)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  String_remove(s,0,2);
  ASSERT_STR("st",String_cString(s));

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  String_remove(s,1,2);
  ASSERT_STR("tt",String_cString(s));

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  String_remove(s,2,2);
  ASSERT_STR("te",String_cString(s));

  String_delete(s);
}

CTEST(strings,truncate)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  String_truncate(s,0,2);
  ASSERT_STR("te",String_cString(s));

  String_delete(s);
}

CTEST(strings,replace)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("test");
  String_replace(s,1,2,t);
  ASSERT_STR("ttestt",String_cString(s));
  String_delete(t);

  String_replaceCString(s,1,2,"test");
  ASSERT_STR("tteststt",String_cString(s));

  String_replaceChar(s,1,2,'t');
  ASSERT_STR("ttststt",String_cString(s));

  strcpy(buffer,"test");
  String_replaceBuffer(s,1,2,buffer,4);
  ASSERT_STR("ttesttstt",String_cString(s));

  String_delete(s);
}

CTEST(strings,replaceAll)
{
  String s;
  String t0,t1;

  s = String_new();

  String_setCString(s,"testtest");
  ASSERT_STR("testtest",String_cString(s));

  t0 = String_newCString("test");
  t1 = String_newCString("baab");
  String_replaceAll(s,STRING_BEGIN,t0,t1);
  ASSERT_STR("baabbaab",String_cString(s));
  String_delete(t1);
  String_delete(t0);

  String_replaceAllCString(s,STRING_BEGIN,"aa","c");
  ASSERT_STR("bcbbcb",String_cString(s));

  String_replaceAllChar(s,STRING_BEGIN,'c','d');
  ASSERT_STR("bdbbdb",String_cString(s));

  String_delete(s);
}

CTEST(strings,map)
{
  String s;
  String t0[2],t1[2];

  s = String_new();

  String_setCString(s,"testtest'test'");
  ASSERT_STR("testtest'test'",String_cString(s));

  t0[0] = String_newCString("t");
  t1[0] = String_newCString("00");
  t0[1] = String_newCString("e");
  t1[1] = String_newCString("11");
  String_map(s,STRING_BEGIN,t0,t1,2,NULL);
  ASSERT_STR("0011s000011s00'0011s00'",String_cString(s));
  String_delete(t1[1]);
  String_delete(t1[0]);
  String_delete(t0[1]);
  String_delete(t0[0]);

  String_mapCString(s,STRING_BEGIN,(const char*[]){"0","1"},(const char*[]){"2","3"},2,NULL);
  ASSERT_STR("2233s222233s22'2233s22'",String_cString(s));

  String_mapChar(s,STRING_BEGIN,"s3","X4",2,NULL);
  ASSERT_STR("2244X222244X22'2244X22'",String_cString(s));

  String_delete(s);
}

CTEST(strings,sub)
{
  String s;
  String t;
  char   cString[4+1];
  char   buffer[4+1];

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("test");
  String_sub(t,s,1,2);
  ASSERT_STR("es",String_cString(t));
  String_delete(t);

  String_subCString(cString,s,1,2);
  ASSERT_STR("es",cString);

  strcpy(buffer,"test");
  String_subBuffer(buffer,s,1,2); buffer[2] = NUL;
  ASSERT_STR("es",buffer);

  String_delete(s);
}

CTEST(strings,join)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("test");
  String_join(s,t,',');
  ASSERT_STR("test,test",String_cString(s));
  String_delete(t);

  String_joinCString(s,"test",',');
  ASSERT_STR("test,test,test",String_cString(s));

  String_joinChar(s,'X',',');
  ASSERT_STR("test,test,test,X",String_cString(s));

  strcpy(buffer,"test");
  String_joinBuffer(s,buffer,4,',');
  ASSERT_STR("test,test,test,X,test",String_cString(s));

  String_delete(s);
}

CTEST(strings,length)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  ASSERT_EQUAL(String_length(s),4);

  String_delete(s);
}

CTEST(strings,isSet)
{
  String s;

  s = String_new();

  ASSERT_TRUE(String_isSet(s));

  String_delete(s);
}

CTEST(strings,isEmpty)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  ASSERT_TRUE(!String_isEmpty(s));

  String_delete(s);
}

CTEST(strings,index)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  ASSERT_EQUAL(String_index(s,STRING_BEGIN),'t');
  ASSERT_EQUAL(String_atUTF8(s,STRING_BEGIN+1,NULL),'e');

  String_delete(s);
}

CTEST(strings,cString)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  String_delete(s);
}

CTEST(strings,compare)
{
  String s,t;

  s = String_new();
  t = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  String_setCString(t,"test");
  ASSERT_STR("test",String_cString(t));

  ASSERT_EQUAL(String_compare(s,t,
                              CALLBACK_INLINE(int,(char ch1, char ch2, void *userData),
                                              {
                                                ASSERT_EQUAL(userData,NULL);

                                                if      (ch1 < ch2) return -1;
                                                else if (ch1 < ch2) return  1;
                                                else                return  0;
                                              },NULL
                                             )
                             ),
               0
              );

  String_delete(t);
  String_delete(s);
}

CTEST(strings,equals)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("test");
  ASSERT_TRUE(String_equals(s,t));
  String_delete(t);

  ASSERT_TRUE(String_equalsCString(s,"test"));

  String_setCString(s,"x");
  ASSERT_TRUE(String_equalsChar(s,'x'));

  String_setCString(s,"test");
  strcpy(buffer,"test");
  ASSERT_TRUE(String_equalsBuffer(s,buffer,4));

  t = String_newCString("TEST");
  ASSERT_TRUE(String_equalsIgnoreCase(s,t));
  String_delete(t);

  ASSERT_TRUE(String_equalsIgnoreCaseCString(s,"TEST"));

  String_setCString(s,"x");
  ASSERT_TRUE(String_equalsIgnoreCaseChar(s,'X'));

  String_setCString(s,"test");
  strcpy(buffer,"TEST");
  ASSERT_TRUE(String_equalsIgnoreCaseBuffer(s,buffer,4));

  String_delete(s);
}

CTEST(strings,subEquals)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("es");
  ASSERT_TRUE(String_subEquals(s,t,1,2));
  String_delete(t);

  ASSERT_TRUE(String_subEqualsCString(s,"es",1,2));

  ASSERT_TRUE(String_subEqualsChar(s,'e',1));

  strcpy(buffer,"es");
  ASSERT_TRUE(String_subEqualsBuffer(s,buffer,4,1,2));

  t = String_newCString("ES");
  ASSERT_TRUE(String_subEqualsIgnoreCase(s,t,1,2));
  String_delete(t);

  ASSERT_TRUE(String_subEqualsIgnoreCaseCString(s,"ES",1,2));

  ASSERT_TRUE(String_subEqualsIgnoreCaseChar(s,'E',1));

  strcpy(buffer,"ES");
  ASSERT_TRUE(String_subEqualsIgnoreCaseBuffer(s,buffer,4,1,2));

  String_delete(s);
}

CTEST(strings,startsWith)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("te");
  ASSERT_TRUE(String_startsWith(s,t));
  String_delete(t);

  ASSERT_TRUE(String_startsWithCString(s,"te"));

  ASSERT_TRUE(String_startsWithChar(s,'t'));

  strcpy(buffer,"te");
  ASSERT_TRUE(String_startsWithBuffer(s,buffer,2));

  String_delete(s);
}

CTEST(strings,endsWith)
{
  String s;
  String t;
  char   buffer[4+1];

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("st");
  ASSERT_TRUE(String_endsWith(s,t));
  String_delete(t);

  ASSERT_TRUE(String_endsWithCString(s,"st"));

  ASSERT_TRUE(String_endsWithChar(s,'t'));

  strcpy(buffer,"st");
  ASSERT_TRUE(String_endsWithBuffer(s,buffer,2));

  String_delete(s);
}

CTEST(strings,find)
{
  String s;
  String t;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  t = String_newCString("st");
  ASSERT_EQUAL(String_find(s,1,t),2);
  String_setCString(s,"ST");
  ASSERT_EQUAL(String_find(s,1,t),-1);
  String_delete(t);

  String_setCString(s,"test");
  ASSERT_EQUAL(String_findCString(s,1,"st"),2);
  ASSERT_EQUAL(String_findCString(s,1,"ST"),-1);

  ASSERT_EQUAL(String_findChar(s,1,'t'),3);
  ASSERT_EQUAL(String_findChar(s,1,'T'),-1);

  t = String_newCString("st");
  ASSERT_EQUAL(String_findLast(s,STRING_END,t),2);
  String_setCString(s,"ST");
  ASSERT_EQUAL(String_findLast(s,STRING_END,t),-1);
  String_delete(t);

  String_setCString(s,"test");
  ASSERT_EQUAL(String_findLastCString(s,STRING_END,"st"),2);
  ASSERT_EQUAL(String_findLastCString(s,STRING_END,"ST"),-1);

  ASSERT_EQUAL(String_findLastChar(s,STRING_END,'t'),3);
  ASSERT_EQUAL(String_findLastChar(s,STRING_END,'T'),-1);

  String_delete(s);
}

CTEST(strings,interate)
{
  String         s;
  StringIterator stringIterator;
  uint           i;
  char           ch;
  Codepoint      codepoint;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  ASSERT_STR("aXaa",
             String_cString(String_iterate(s,
                                           CALLBACK_INLINE(const char*,(char ch, void *userData),
                                                           {
                                                             ASSERT_EQUAL(userData,NULL);

                                                             return (ch == 'e') ? "X" : "a";
                                                           },NULL
                                                          )
                                          )
                           )
            );

  String_setCString(s,"test");

  stringIterator = String_iterateBegin(s);
  i = 0;
  while (stringIterator < String_iterateEnd(s))
  {
    ch = String_iterateNext(s,&stringIterator);
    switch (i)
    {
      case 0: ASSERT_EQUAL(ch,'t'); break;
      case 1: ASSERT_EQUAL(ch,'e'); break;
      case 2: ASSERT_EQUAL(ch,'s'); break;
      case 3: ASSERT_EQUAL(ch,'t'); break;
    }
    i++;
  }

  stringIterator = String_iterateBegin(s);
  i = 0;
  while (stringIterator < String_iterateEnd(s))
  {
    codepoint = String_iterateNextUTF8(s,&stringIterator);
    switch (i)
    {
      case 0: ASSERT_EQUAL(codepoint,'t'); break;
      case 1: ASSERT_EQUAL(codepoint,'e'); break;
      case 2: ASSERT_EQUAL(codepoint,'s'); break;
      case 3: ASSERT_EQUAL(codepoint,'t'); break;
    }
    i++;
  }

  String_delete(s);
}

CTEST(strings,lowerUpper)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));

  String_setCString(s,"TEST");
  ASSERT_STR("test",String_cString(String_toLower(s)));

  String_setCString(s,"test");
  ASSERT_STR("TEST",String_cString(String_toUpper(s)));

  String_delete(s);
}

CTEST(strings,trim)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("es",String_cString(String_trim(s,"t")));

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("est",String_cString(String_trimBegin(s,"t")));

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("tes",String_cString(String_trimEnd(s,"t")));

  String_delete(s);
}

CTEST(strings,escape)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("t'est",
             String_cString(String_escape(s,
                                          '\'',
                                          "e",
                                          NULL,
                                          NULL,
                                          0
                                         )
                           )
            );
  ASSERT_STR("test",
             String_cString(String_unescape(s,
                                            '\'',
                                            NULL,
                                            NULL,
                                            0
                                           )
                           )
            );

  ASSERT_STR("'a'es'a",
             String_cString(String_escape(s,
                                          '\'',
                                          "e",
                                          "t",
                                          "a",
                                          1
                                         )
                           )
            );
  ASSERT_STR("test",
             String_cString(String_unescape(s,
                                            '\'',
                                            "a",
                                            "t",
                                            1
                                           )
                           )
            );

  String_delete(s);
}

CTEST(strings,quote)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("'test'",
             String_cString(String_quote(s,
                                         '\'',
                                         NULL
                                        )
                           )
            );

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("test",
             String_cString(String_quote(s,
                                         '\'',
                                         "X"
                                        )
                           )
            );
  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("'test'",
             String_cString(String_quote(s,
                                         '\'',
                                         "e"
                                        )
                           )
            );

  String_setCString(s,"t'est");
  ASSERT_STR("t'est",String_cString(s));
  ASSERT_STR("'t\\'est'",
             String_cString(String_quote(s,
                                         '\'',
                                         NULL
                                        )
                           )
            );
  ASSERT_STR("t'est",
             String_cString(String_unquote(s,
                                           "'"
                                          )
                           )
            );

  String_delete(s);
}

CTEST(strings,pad)
{
  String s;

  s = String_new();

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("test    ",
             String_cString(String_padRight(s,
                                            8,
                                            ' '
                                           )
                           )
            );

  String_setCString(s,"test");
  ASSERT_STR("test",String_cString(s));
  ASSERT_STR("    test",
             String_cString(String_padLeft(s,
                                           8,
                                           ' '
                                          )
                           )
            );

  String_delete(s);
}

CTEST(strings,fill)
{
  String s;

  s = String_new();

  ASSERT_STR("tttt",
             String_cString(String_fillChar(s,
                                            4,
                                            't'
                                           )
                           )
            );

  String_delete(s);
}

CTEST(strings,tokenizer)
{
  String          s;
  StringTokenizer stringTokenizer;
  ConstString     token;
  long            tokenIndex;

  s = String_new();

  String_setCString(s,"test 'abcd ' '' \t1234");
  ASSERT_STR("test 'abcd ' '' \t1234",String_cString(s));
  String_initTokenizer(&stringTokenizer,
                       s,
                       STRING_BEGIN,
                       " \t",
                       "'",
                       FALSE
                      );
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("test",String_cString(token));
  ASSERT_EQUAL(tokenIndex,0);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("abcd ",String_cString(token));
  ASSERT_EQUAL(tokenIndex,5);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("",String_cString(token));
  ASSERT_EQUAL(tokenIndex,13);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("",String_cString(token));
  ASSERT_EQUAL(tokenIndex,16);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("1234",String_cString(token));
  ASSERT_EQUAL(tokenIndex,17);
  String_doneTokenizer(&stringTokenizer);

  String_initTokenizer(&stringTokenizer,
                       s,
                       STRING_BEGIN,
                       " \t",
                       "'",
                       TRUE
                      );
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("test",String_cString(token));
  ASSERT_EQUAL(tokenIndex,0);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("abcd ",String_cString(token));
  ASSERT_EQUAL(tokenIndex,5);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("1234",String_cString(token));
  ASSERT_EQUAL(tokenIndex,17);
  String_doneTokenizer(&stringTokenizer);

  String_delete(s);

  String_initTokenizerCString(&stringTokenizer,
                              "test 'abcd ' '' \t1234",
                              " \t",
                              "'",
                              FALSE
                             );
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("test",String_cString(token));
  ASSERT_EQUAL(tokenIndex,0);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("abcd ",String_cString(token));
  ASSERT_EQUAL(tokenIndex,5);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("",String_cString(token));
  ASSERT_EQUAL(tokenIndex,13);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("",String_cString(token));
  ASSERT_EQUAL(tokenIndex,16);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("1234",String_cString(token));
  ASSERT_EQUAL(tokenIndex,17);
  String_doneTokenizer(&stringTokenizer);

  String_initTokenizerCString(&stringTokenizer,
                              "test 'abcd ' '' \t1234",
                              " \t",
                              "'",
                              TRUE
                             );
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("test",String_cString(token));
  ASSERT_EQUAL(tokenIndex,0);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("abcd ",String_cString(token));
  ASSERT_EQUAL(tokenIndex,5);
  ASSERT_TRUE(String_getNextToken(&stringTokenizer,&token,&tokenIndex));
  ASSERT_STR("1234",String_cString(token));
  ASSERT_EQUAL(tokenIndex,17);
  String_doneTokenizer(&stringTokenizer);
}

CTEST(strings,scan)
{
  String string;
  int    i;
  uint   u;
  bool   b;
  long   l;
  float  f;
  double d;
  char   s[4+1];

  string = String_new();

  String_setCString(string,"1234");
  ASSERT_TRUE(String_scan(string,STRING_BEGIN,"%d",&i));
  ASSERT_EQUAL(1234,i);

  String_setCString(string,"1234");
  ASSERT_TRUE(String_scan(string,STRING_BEGIN,"%u",&u));
  ASSERT_EQUAL(1234,u);

  String_setCString(string,"no");
  ASSERT_TRUE(String_scan(string,STRING_BEGIN,"%y",&b));
  ASSERT_FALSE(b);
  String_setCString(string,"yes");
  ASSERT_TRUE(String_scan(string,STRING_BEGIN,"%y",&b));
  ASSERT_TRUE(b);

  String_setCString(string,"12345678901234");
  ASSERT_TRUE(String_scan(string,STRING_BEGIN,"%ld",&l));
  ASSERT_EQUAL(12345678901234,l);

  String_setCString(string,"1234.5678");
  ASSERT_TRUE(String_scan(string,STRING_BEGIN,"%f",&f));
  ASSERT_EQUAL(1234.5678,f);
  String_setCString(string,"1234.5678");
  ASSERT_TRUE(String_scan(string,STRING_BEGIN,"%lf",&d));
  ASSERT_EQUAL(1234.5678,d);

  String_setCString(string,"test");
  ASSERT_TRUE(String_scan(string,STRING_BEGIN,"%5s",s));
  ASSERT_STR(s,"test");

  String_delete(string);

  ASSERT_TRUE(String_scanCString("1234","%d",&i));
  ASSERT_EQUAL(1234,i);

  ASSERT_TRUE(String_scanCString("1234","%u",&u));
  ASSERT_EQUAL(1234,u);

  ASSERT_TRUE(String_scanCString("no","%y",&b));
  ASSERT_FALSE(b);
  ASSERT_TRUE(String_scanCString("yes","%y",&b));
  ASSERT_TRUE(b);

  ASSERT_TRUE(String_scanCString("12345678901234","%ld",&l));
  ASSERT_EQUAL(12345678901234,l);

  ASSERT_TRUE(String_scanCString("1234.5678","%f",&f));
  ASSERT_EQUAL(1234.5678,f);
  ASSERT_TRUE(String_scanCString("1234.5678","%lf",&d));
  ASSERT_EQUAL(1234.5678,d);

  ASSERT_TRUE(String_scanCString("test","%5s",s));
  ASSERT_STR("test",s);
}

CTEST(strings,parse)
{
  String string;
  int    i;
  uint   u;
  bool   b;
  long   l;
  float  f;
  double d;
  char   s[4+1];
  long   nextIndex;
  String t;

  string = String_new();

  String_setCString(string,"1234");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%d",&nextIndex,&i));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234,i);

  String_setCString(string,"1234");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%u",&nextIndex,&u));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234,u);

  String_setCString(string,"12345678901234");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%ld",&nextIndex,&l));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(12345678901234,l);

  String_setCString(string,"no");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%y",&nextIndex,&b));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_FALSE(b);
  String_setCString(string,"yes");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%y",&nextIndex,&b));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_TRUE(b);

  String_setCString(string,"1234.5678");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%f",&nextIndex,&f));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234.5678,f);
  String_setCString(string,"1234.5678");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%lf",&nextIndex,&d));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234.5678,d);

  String_setCString(string,"test");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%5s",&nextIndex,s));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_STR("test",s);

  t = String_new();
  String_setCString(string,"' test '");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"%'S",&nextIndex,t));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_STR(" test " ,String_cString(t));
  String_delete(t);

  t = String_new();
  String_setCString(string,"test foo abcd");
  ASSERT_TRUE(String_parse(string,STRING_BEGIN,"% S",&nextIndex,t));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_STR("test foo abcd" ,String_cString(t));
  String_delete(t);

  t = String_new();
  String_setCString(string,"1234 1234 12345678901234 yes 1234.5678 1234.5678 test abcd foo xyz");
  ASSERT_TRUE(String_parse(string,
                           STRING_BEGIN,
                           "%d %u %ld %y %f %lf %5s %1s % S",
                           &nextIndex,
                           &i,
                           &u,
                           &l,
                           &b,
                           &f,
                           &d,
                           s,
                           NULL,
                           t
                          )
             );
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234,i);
  ASSERT_EQUAL(1234,u);
  ASSERT_TRUE(b);
  ASSERT_EQUAL(12345678901234,l);
  ASSERT_EQUAL(1234.5678,f);
  ASSERT_EQUAL(1234.5678,d);
  ASSERT_STR("test" ,s);
  ASSERT_STR("foo xyz" ,String_cString(t));
  String_delete(t);

  String_delete(string);

  ASSERT_TRUE(String_parseCString("1234","%d",&nextIndex,&i));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234,i);

  ASSERT_TRUE(String_parseCString("1234","%u",&nextIndex,&u));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234,u);

  ASSERT_TRUE(String_parseCString("12345678901234","%ld",&nextIndex,&l));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(12345678901234,l);

  ASSERT_TRUE(String_parseCString("no","%y",&nextIndex,&b));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_FALSE(b);
  ASSERT_TRUE(String_parseCString("yes","%y",&nextIndex,&b));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_TRUE(b);

  ASSERT_TRUE(String_parseCString("1234.5678","%f",&nextIndex,&f));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234.5678,f);
  ASSERT_TRUE(String_parseCString("1234.5678","%lf",&nextIndex,&d));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234.5678,d);

  ASSERT_TRUE(String_parseCString("test","%5s",&nextIndex,s));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_STR("test",s);

  t = String_new();
  ASSERT_TRUE(String_parseCString("' test '","%'S",&nextIndex,t));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_STR(" test " ,String_cString(t));
  String_delete(t);

  t = String_new();
  ASSERT_TRUE(String_parseCString("test foo abcd","% S",&nextIndex,t));
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_STR("test foo abcd" ,String_cString(t));
  String_delete(t);

  t = String_new();
  ASSERT_TRUE(String_parseCString("1234 1234 12345678901234 yes 1234.5678 1234.5678 test abcd foo xyz",
                                  "%d %u %ld %y %f %lf %5s %1s % S",
                                  &nextIndex,
                                  &i,
                                  &u,
                                  &l,
                                  &b,
                                  &f,
                                  &d,
                                  s,
                                  NULL,
                                  t
                                 )
             );
  ASSERT_EQUAL(STRING_END,nextIndex);
  ASSERT_EQUAL(1234,i);
  ASSERT_EQUAL(1234,u);
  ASSERT_TRUE(b);
  ASSERT_EQUAL(12345678901234,l);
  ASSERT_EQUAL(1234.5678,f);
  ASSERT_EQUAL(1234.5678,d);
  ASSERT_STR("test" ,s);
  ASSERT_STR("foo xyz" ,String_cString(t));
  String_delete(t);
}

CTEST(strings,match)
{
  String string;
  String t;
  long   nextIndex;

  string = String_new();

  t = String_newCString("^t(es)t$");
  String_setCString(string,"test");
  ASSERT_TRUE(String_match(string,STRING_BEGIN,t,&nextIndex,string,NULL));
  ASSERT_EQUAL(4,nextIndex);
  ASSERT_STR("test",String_cString(string));
  String_delete(t);

  t = String_newCString("^t(es)t$");
  String_setCString(string,"test");
  ASSERT_TRUE(String_match(string,STRING_BEGIN,t,&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(4,nextIndex);
  ASSERT_STR("es",String_cString(string));
  String_delete(t);

  t = String_newCString("^t(a*)t$");
  String_setCString(string,"tt");
  ASSERT_TRUE(String_match(string,STRING_BEGIN,t,&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(2,nextIndex);
  ASSERT_STR("",String_cString(string));
  String_setCString(string,"tat");
  ASSERT_TRUE(String_match(string,STRING_BEGIN,t,&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(3,nextIndex);
  ASSERT_STR("a",String_cString(string));
  String_setCString(string,"taaaat");
  ASSERT_TRUE(String_match(string,STRING_BEGIN,t,&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(6,nextIndex);
  ASSERT_STR("aaaa",String_cString(string));
  String_delete(t);

  t = String_newCString("^t([0-9]+)t$");
  String_setCString(string,"t21t");
  ASSERT_TRUE(String_match(string,STRING_BEGIN,t,&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(4,nextIndex);
  ASSERT_STR("21",String_cString(string));
  String_delete(t);


  String_setCString(string,"test");
  ASSERT_TRUE(String_matchCString(string,STRING_BEGIN,"^t(es)t$",&nextIndex,string,NULL));
  ASSERT_EQUAL(4,nextIndex);
  ASSERT_STR("test",String_cString(string));

  String_setCString(string,"test");
  ASSERT_TRUE(String_matchCString(string,STRING_BEGIN,"^t(es)t$",&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(4,nextIndex);
  ASSERT_STR("es",String_cString(string));

  String_setCString(string,"tt");
  ASSERT_TRUE(String_matchCString(string,STRING_BEGIN,"^t(a*)t$",&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(2,nextIndex);
  ASSERT_STR("",String_cString(string));
  String_setCString(string,"tat");
  ASSERT_TRUE(String_matchCString(string,STRING_BEGIN,"^t(a*)t$",&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(3,nextIndex);
  ASSERT_STR("a",String_cString(string));
  String_setCString(string,"taaaat");
  ASSERT_TRUE(String_matchCString(string,STRING_BEGIN,"^t(a*)t$",&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(6,nextIndex);
  ASSERT_STR("aaaa",String_cString(string));

  String_setCString(string,"t21t");
  ASSERT_TRUE(String_matchCString(string,STRING_BEGIN,"^t([0-9]+)t$",&nextIndex,STRING_NO_ASSIGN,string,NULL));
  ASSERT_EQUAL(4,nextIndex);
  ASSERT_STR("21",String_cString(string));

  String_delete(string);
}

CTEST(strings,toInteger)
{
  const StringUnit STRING_UNITS[] = {{"X",10},{"Y",100}};

  String string;
  long   nextIndex;

  string = String_new();

  String_setCString(string,"1234");
  ASSERT_EQUAL(1234,String_toInteger(string,STRING_BEGIN,&nextIndex,NULL,0));
  String_setCString(string,"0x1234");
  ASSERT_EQUAL(0x1234,String_toInteger(string,STRING_BEGIN,&nextIndex,NULL,0));

  String_setCString(string,"1234X");
  ASSERT_EQUAL(1234*10,String_toInteger(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));
  String_setCString(string,"0x1234X");
  ASSERT_EQUAL(0x1234*10,String_toInteger(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));
  String_setCString(string,"1234Y");
  ASSERT_EQUAL(1234*100,String_toInteger(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));
  String_setCString(string,"0x1234Y");
  ASSERT_EQUAL(0x1234*100,String_toInteger(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));

  String_delete(string);
}

CTEST(strings,toInteger64)
{
  const StringUnit STRING_UNITS[] = {{"X",10},{"Y",100}};

  String string;
  long   nextIndex;

  string = String_new();

  String_setCString(string,"1234");
  ASSERT_EQUAL(1234,String_toInteger64(string,STRING_BEGIN,&nextIndex,NULL,0));
  String_setCString(string,"0x1234");
  ASSERT_EQUAL(0x1234,String_toInteger64(string,STRING_BEGIN,&nextIndex,NULL,0));

  String_setCString(string,"1234X");
  ASSERT_EQUAL(1234*10,String_toInteger64(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));
  String_setCString(string,"0x1234X");
  ASSERT_EQUAL(0x1234*10,String_toInteger64(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));
  String_setCString(string,"1234Y");
  ASSERT_EQUAL(1234*100,String_toInteger64(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));
  String_setCString(string,"0x1234Y");
  ASSERT_EQUAL(0x1234*100,String_toInteger64(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));

  String_delete(string);
}

CTEST(strings,toDouble)
{
  const StringUnit STRING_UNITS[] = {{"X",10},{"Y",100}};

  String string;
  long   nextIndex;

  string = String_new();

  String_setCString(string,"1234.5678");
  ASSERT_EQUAL(1234.5678,String_toDouble(string,STRING_BEGIN,&nextIndex,NULL,0));

  String_setCString(string,"1234.5678X");
  ASSERT_EQUAL(1234.5678*10,String_toDouble(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));
  String_setCString(string,"1234.5678Y");
  ASSERT_EQUAL(1234.5678*100,String_toDouble(string,STRING_BEGIN,&nextIndex,STRING_UNITS,2));

  String_delete(string);
}

CTEST(strings,toBoolean)
{
  const char *NO_STRINGS [] = {"A"};
  const char *YES_STRINGS[] = {"B"};

  String string;
  long   nextIndex;

  string = String_new();

  String_setCString(string,"0");
  ASSERT_FALSE(String_toBoolean(string,STRING_BEGIN,&nextIndex,NULL,0,NULL,0));
  String_setCString(string,"1");
  ASSERT_TRUE(String_toBoolean(string,STRING_BEGIN,&nextIndex,NULL,0,NULL,0));

  String_setCString(string,"false");
  ASSERT_FALSE(String_toBoolean(string,STRING_BEGIN,&nextIndex,NULL,0,NULL,0));
  String_setCString(string,"true");
  ASSERT_TRUE(String_toBoolean(string,STRING_BEGIN,&nextIndex,NULL,0,NULL,0));

  String_setCString(string,"no");
  ASSERT_FALSE(String_toBoolean(string,STRING_BEGIN,&nextIndex,NULL,0,NULL,0));
  String_setCString(string,"yes");
  ASSERT_TRUE(String_toBoolean(string,STRING_BEGIN,&nextIndex,NULL,0,NULL,0));

  String_setCString(string,"A");
  ASSERT_FALSE(String_toBoolean(string,STRING_BEGIN,&nextIndex,YES_STRINGS,1,NO_STRINGS,1));
  String_setCString(string,"B");
  ASSERT_TRUE(String_toBoolean(string,STRING_BEGIN,&nextIndex,YES_STRINGS,1,NO_STRINGS,1));

  String_delete(string);
}

CTEST(strings,toString)
{
  String string;
  String t;
  long   nextIndex;

  string = String_new();

  t = String_newCString("test");
  String_clear(string);
  ASSERT_STR("test",String_cString(String_toString(string,t,STRING_BEGIN,&nextIndex,NULL)));
  ASSERT_EQUAL(4,nextIndex);
  String_delete(t);

  t = String_newCString("' test '");
  String_clear(string);
  ASSERT_STR("'",String_cString(String_toString(string,t,STRING_BEGIN,&nextIndex,NULL)));
  ASSERT_EQUAL(1,nextIndex);
  String_delete(t);

  t = String_newCString("' test '");
  String_clear(string);
  ASSERT_STR(" test ",String_cString(String_toString(string,t,STRING_BEGIN,&nextIndex,"'")));
  ASSERT_EQUAL(8,nextIndex);
  String_delete(t);

  String_delete(string);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
