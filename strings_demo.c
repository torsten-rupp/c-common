/***********************************************************************\
*
* $Source$
* $Revision$
* $Author$
* Contents: dynamic strings demo
* Systems: *nix
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "global.h"
#include "strings.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

/*---------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
  String          s,t,w;
  int             i,j;
  long            nextIndex;
  StringTokenizer stringTokenizer;
  StaticString    (v,16);

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  // allocate and free a string
  s = String_new();
  String_delete(s);

  s = String_newCString("Hello world!");
  String_delete(s);

  // assign string, append string
  s = String_new();
  String_setCString(s,"Hello world, ");
  String_appendCString(s,"I'm here!");
  String_delete(s);

  // print string
  s = String_newCString("Hello world!\n");
  printf("%s\n",String_cString(s));
  String_delete(s);
  printf("\n");

  // string length, sub-string, find string
  s = String_new();
  String_setCString(s,"Hello world, I'm here!");
  printf("string length '%s': %lu characters\n",String_cString(s),String_length(s));
  t = String_sub(String_new(),s,0,5);
  printf("sub-string %s",String_cString(t));
  printf("find string 'world': index %ld\n",String_findCString(s,STRING_BEGIN,"world"));
  String_delete(t);
  String_delete(s);
  printf("\n");

  // format string
  s = String_new();
  t = String_new();
  w = String_new();
  String_setCString(s,"Hello");
  String_setCString(t,"world");
  String_setCString(w,"\t'Die Verwandlung'\nFranz Kafka\r!");
  printf("Format result:\n");
  String_format(s,
                "%d %ld %lld %f %s %S %'s %'S %'S emp=#%S#",
                123,
                456L,
                789LL,
                0.123,
                String_cString(s),
                s,
                String_cString(t),
                t,
                w,
                STRING_EMPTY
               );
  printf("s=%s\n",String_cString(s));
  String_delete(w);
  String_delete(t);
  String_delete(s);
  printf("\n");

  // parse string
  s = String_new();
  t = String_new();
  w = String_new();
  String_setCString(s,"Hello 4711 we are 08.15");
  printf("String: %s\n",String_cString(s));
  if (String_parse(s,STRING_BEGIN,"%S %d % S",NULL,t,&i,w))
  {
    printf("Parse result 1:\n");
    printf("string=%s\n",String_cString(t));
    printf("int=%d\n",i);
    printf("rest=%s\n",String_cString(w));
  }
  if (String_parse(s,STRING_BEGIN,"Hello ",&nextIndex))
  {
    printf("Parse result 2:\n");
    printf("next index after 'Hello '=%lu\n",nextIndex);
  }
  String_setCString(s,"foo 1");
  printf("String: %s\n",String_cString(s));
  if (String_parse(s,STRING_BEGIN,"foo %d %d",NULL,&i,&j))
  {
    printf("Parse result 3a:\n");
    printf("i=%d\n",i);
    printf("j=%d\n",j);
  }
  if (String_parse(s,STRING_BEGIN,"foo %d",NULL,&i))
  {
    printf("Parse result 3b:\n");
    printf("i=%d\n",i);
  }
  String_setCString(s,"foo/");
  printf("String: %s\n",String_cString(s));
  if (String_parse(s,STRING_BEGIN,"%S/%S",&nextIndex,t,w))
  {
    printf("Parse result 4:\n");
    printf("next index=%ld=%s\n",nextIndex,(nextIndex == STRING_END) ? "none" : "some");
  }
  String_delete(w);
  String_delete(t);
  String_delete(s);
  printf("\n");

  // match string
  s = String_new();
  t = String_new();
  w = String_new();
  String_setCString(s,"Hello 4711 we are 08.15");
  printf("String: %s\n",String_cString(s));
  if (String_matchCString(s,STRING_BEGIN,".* ([0-7]+) .*are ([[:digit:]]+).*",NULL,NULL,t,w))
  {
    printf("Match result 1:\n");
    printf("group 1 [0-7]+=%s\n",String_cString(t));
    printf("group 2 [[:digit:]]+=%s\n",String_cString(w));
  }
  printf("\n");
  String_delete(w);
  String_delete(t);
  String_delete(s);

  s = String_new();
  t = String_new();
  w = String_new();
  String_setCString(s,"it_foo.com:345");
  printf("String: %s\n",String_cString(s));
  if (String_matchCString(s,STRING_BEGIN,"[[:alnum:]_]+:[[:digit:]]+",NULL,NULL))
  {
    printf("Match result 2:\n");
    printf("match=%s\n",String_cString(s));
  }
  String_delete(w);
  String_delete(t);
  String_delete(s);
  printf("\n");

  // tokenizer
  s = String_new();
  String_setCString(s,"Hello world,, I'm here!");
  printf("String: %s\n",String_cString(s));
  String_initTokenizer(&stringTokenizer,s,STRING_BEGIN," ,",NULL,TRUE);
  while (String_getNextToken(&stringTokenizer,&t,NULL))
  {
    printf("token=%s\n",String_cString(t));
  }
  String_doneTokenizer(&stringTokenizer);

  String_setCString(s,"'Die Verwandlung' by 'Franz Kafka'!");
  printf("String: %s\n",String_cString(s));
  String_initTokenizer(&stringTokenizer,s,STRING_BEGIN," ,","'",TRUE);
  while (String_getNextToken(&stringTokenizer,&t,NULL))
  {
    printf("token=%s\n",String_cString(t));
  }
  String_doneTokenizer(&stringTokenizer);
  String_delete(s);
  printf("\n");

  // misc functions
  s = String_new();
  String_setCString(s,"  Hello 'World!'  ");
  printf("String: #%s#\n",String_cString(s));
  String_trimRight(s,STRING_WHITE_SPACES);
  printf("trim right=#%s#\n",String_cString(s));
  String_trimLeft(s,STRING_WHITE_SPACES);
  printf("trim left=#%s#\n",String_cString(s));
  String_toLower(s);
  printf("lower=#%s#\n",String_cString(s));
  String_toUpper(s);
  printf("upper=#%s#\n",String_cString(s));
  String_escape(s,"'",'\\',NULL,NULL,0);
  printf("escaped '=#%s#\n",String_cString(s));
  String_unescape(s,'\\',NULL,NULL,0);
  printf("ununescaped '=#%s#\n",String_cString(s));
  String_quote(s,'\'');
  printf("quoted=#%s#\n",String_cString(s));
  String_unquote(s,"'");
  printf("unquoted=#%s#\n",String_cString(s));
  String_delete(s);

  // uncomment to see debug functions

  #if 0
  // debug functions: lost string
  s = String_new();
  #endif /* 0 */

  #if 0
  // debug function: duplicate free
  s = String_new();
  String_delete(s);
  String_delete(s);
  #endif /* 0 */

  #if 0
  // debug function: invalid string
  printf(String_cString(s));
  String_delete(s);
  #endif /* 0 */

  #if 0
  // debug functions: delete static string
  String_delete(v);
  #endif /* 0 */

  #if 0
  // debug functions: exceed static string
  String_setCString(v,"Hello World: how are you?");
  printf("static string=#%s#\n",String_cString(v));
  #endif /* 0 */

  String_debugPrintInfo();

  return 0;
}

#ifdef __cplusplus
  }
#endif

/* end of file */
