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
  String s,t,w;
  int    i,j;
  long   nextIndex;

  /* allocate and free a string */
  s = String_new();
  String_delete(s);

  s = String_newCString("Hello world!");
  String_delete(s);

  /* assign string */
  s = String_new();
  String_setCString(s,"Hello world, I'm here!");
  String_delete(s);

  /* print string */
  s = String_newCString("Hello world!\n");
  printf("%s",String_cString(s));
  printf("\n");
  String_delete(s);

  /* format string */
  s = String_new();
  t = String_new();
  String_setCString(s,"Hello");
  String_setCString(t,"world");
  printf("Format result:\n");
  String_format(s,
                "%d %ld %lld %f %s %S %'s %'S\n",
                123,
                456L,
                789LL,
                0.123,
                String_cString(s),
                s,
                String_cString(t),
                t
               );  
  printf("s=%s\n",String_cString(s));
  printf("\n");
  String_delete(t);
  String_delete(s);

  /* parse string */
  s = String_new();
  t = String_new();
  w = String_new();
  String_setCString(s,"Hello 4711 we are 08.15");
  if (String_parse(s,STRING_BEGIN,"%S %d % S",NULL,t,&i,w))
  {
    printf("Parse result 1:\n");
    printf("t=%s\n",String_cString(t));
    printf("i=%d\n",i);
    printf("w=%s\n",String_cString(w));
    printf("\n");
  }
  if (String_parse(s,STRING_BEGIN,"Hello ",&nextIndex))
  {
    printf("Parse result 2:\n");
    printf("next index=%lu\n",nextIndex);
    printf("\n");
  }
  String_setCString(s,"foo 1");
  if (String_parse(s,STRING_BEGIN,"foo %d %d",NULL,&i,&j))
  {
    printf("Parse result 3a:\n");
    printf("i=%d\n",i);
    printf("j=%d\n",j);
    printf("\n");
  }
  if (String_parse(s,STRING_BEGIN,"foo %d",NULL,&i))
  {
    printf("Parse result 3b:\n");
    printf("i=%d\n",i);
    printf("\n");
  }
  String_setCString(s,"foo/");
  if (String_parse(s,STRING_BEGIN,"%S/%S",&nextIndex,t,w))
  {
    printf("Parse result 4:\n");
    printf("next index=%ld\n",nextIndex);
    printf("\n");
  }
  String_delete(w);
  String_delete(t);
  String_delete(s);

  /* match string */
  s = String_new();
  t = String_new();
  w = String_new();
  String_setCString(s,"Hello 4711 we are 08.15");
  if (String_matchCString(s,STRING_BEGIN,".* ([0-7]+) .*are ([[:digit:]]+).*",NULL,t,w))
  {
    printf("Match result 1:\n");
    printf("t=%s\n",String_cString(t));
    printf("w=%s\n",String_cString(w));
  }
  printf("\n");
  String_delete(w);
  String_delete(t);
  String_delete(s);

  s = String_new();
  t = String_new();
  w = String_new();
  String_setCString(s,"it_foo.com:345");
  if (String_matchCString(s,STRING_BEGIN,"[[:alnum:]_]+:[[:digit:]]+",NULL,NULL))
  {
    printf("Match result 2:\n");
    printf("match %s\n",String_cString(s));
    printf("\n");
  }
  printf("\n");

  String_delete(w);
  String_delete(t);
  String_delete(s);

  /* misc functions */
  

  /* uncomment to see debug functions */

  #if 0
  /* debug functions: lost string */
  s = String_new();
  #endif /* 0 */

  #if 0
  /* debug function: duplicate free */
  s = String_new();
  String_delete(s);
  String_delete(s);
  #endif /* 0 */

  #if 0
  /* debug function: invalid string */
  printf(String_cString(s));
  String_delete(s);
  #endif /* 0 */

  String_debugPrintInfo();

  return 0;
}

#ifdef __cplusplus
  }
#endif

/* end of file */
