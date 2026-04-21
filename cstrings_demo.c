/***********************************************************************\
*
* Contents: C-strings demo
* Systems: *nix
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "common/global.h"
#include "common/cstrings.h"

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
  char             s[100],t[100],w[100];
  CStringTokenizer cStringTokenizer;
  const char       *token;
  const char       *u;
  const char       *v1,*v2;
  size_t           n1;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  stringSet(s,sizeof(s),"Hello world!");

  // string length, sub-string, find string
  stringSet(s,sizeof(s),"Hello world, I'm here!");
  printf("string length '%s': %lu characters\n",s,stringLength(s));

  // format string
  stringSet(t,sizeof(t),"world");
  stringSet(w,sizeof(w),"\t'Die Verwandlung'\nFranz Kafka!");
  printf("Format result:\n");
  stringFormat(s,
               sizeof(s),
               "%d %ld %lld %f %s %'s",
               123,
               456L,
               789LL,
               0.123,
               t,
               w
              );
  printf("s=%s\n",s);

  // match string
  stringSet(s,sizeof(s),"Hello 4711 we are 08.15");
  printf("String: %s\n",s);
  if (stringMatch(s,".* ([0-7]+) .*are ([[:digit:]]+).*",STRING_NO_ASSIGN,STRING_NO_ASSIGN,&v1,&n1,&v2,STRING_NO_ASSIGN,NULL))
  {
    printf("Match result 1:\n");
    printf("group 1 [0-7]+=");fwrite(v1,1,n1,stdout);printf("\n");
    printf("group 2 [[:digit:]]+=%s\n",v2);
  }
  printf("\n");

  stringSet(s,sizeof(s),"it_foo.com:345");
  printf("String: %s\n",s);
  if (stringMatch(s,"[[:alnum:]_]+:[[:digit:]]+",&u,NULL,NULL))
  {
    printf("Match result 2:\n");
    printf("match=%s\n",u);
  }
  printf("\n");

  // tokenizer
  stringSet(s,sizeof(s),"Hello world,, I'm here!");
  printf("String: %s\n",s);
  stringTokenizerInit(&cStringTokenizer,s," ,");
  while (stringGetNextToken(&cStringTokenizer,&token))
  {
    printf("token=%s\n",token);
  }
  stringTokenizerDone(&cStringTokenizer);

  stringSet(s,sizeof(s),"'Die Verwandlung' by 'Franz Kafka'!");
  printf("String: %s\n",s);
  stringTokenizerInit(&cStringTokenizer,s," ,");
  while (stringGetNextToken(&cStringTokenizer,&token))
  {
    printf("token=%s\n",token);
  }
  stringTokenizerDone(&cStringTokenizer);
  printf("\n");

  // misc functions
  stringSet(s,sizeof(s),"  Hello 'World!'  ");
  printf("String: #%s#\n",s);
  u = stringTrimEnd(s);
  printf("trim right=#%s#\n",u);

  stringSet(s,sizeof(s),"  Hello 'World!'  ");
  printf("String: #%s#\n",s);
  u = stringTrimBegin(s);
  printf("trim left=#%s#\n",u);

  stringSet(s,sizeof(s),"  Hello 'World!'  ");
  printf("String: #%s#\n",s);
  u = stringTrim(s);
  printf("trim left/right=#%s#\n",u);

  return 0;
}

#ifdef __cplusplus
  }
#endif

/* end of file */
