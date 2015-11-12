/***********************************************************************\
*
* $Source$
* $Revision$
* $Author$
* Contents:
* Systems :
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>

#include "global.h"
#include "dictionaries.h"

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

LOCAL uint64 getTimestamp(void)
{
  struct timeval tv;

  if (gettimeofday(&tv,NULL) == 0)
  {
//fprintf(stderr,"%s,%d: %ld %ld\n",__FILE__,__LINE__,tv.tv_sec,tv.tv_usec);
    return (uint64)tv.tv_usec+(uint64)tv.tv_sec*1000000LL;
  }
  else
  {
    return 0LL;
  }
}

/*---------------------------------------------------------------------*/

int main(int argc, char *argv[])
 {
  const char         *fileName;
  FILE               *file;
  ulong              n;
  uint64             t0,t1,dt;
  Dictionary         dictionary;
  char               line[1024];
  DictionaryIterator dictionaryIterator;
  const void         *data;
  ulong              length;

  if (argc <= 1)
  {
    printf("Usage: dictionary_demo <filename>\n");
    return 1;
  }
  fileName = argv[1];

  Dictionary_init(&dictionary,
                  CALLBACK_NULL,
                  CALLBACK_NULL
                 );

  file = fopen(fileName,"r");
  t0 = getTimestamp();
  n = 0;
  while (!feof(file))
  {
    fgets(line,sizeof(line),file);
    n++;
//    if ((n%10000) == 0) printf("Store %lu\n",n);

    Dictionary_add(&dictionary,line,strlen(line)+1,NULL,0,DICTIONARY_BYTE_COPY);
  }
  t1 = getTimestamp();
  dt = t1-t0;
  fclose(file);
  fprintf(stderr,"Create time: %llus %llums %lluus\n",dt/1000000LL,(dt%1000000LL)/1000LL,dt%1000LL);

  file = fopen(fileName,"r");
  t0 = getTimestamp();
  n = 0;
  while (!feof(file))
  {
    fgets(line,sizeof(line),file);
    n++;

    if (!Dictionary_contains(&dictionary,line,strlen(line)+1))
    {
      HALT_INTERNAL_ERROR("Table does not contain line #%lu: '%s'\n",n,line);
    }
  }
  t1 = getTimestamp();
  dt = t1-t0;
  fclose(file);
  fprintf(stderr,"Check contain time: %llus %llums %lluus\n",dt/1000000LL,(dt%1000000LL)/1000LL,dt%1000LL);

  Dictionary_initIterator(&dictionaryIterator,&dictionary);
  while (Dictionary_getNext(&dictionaryIterator,&data,&length,NULL,0))
  {
//    printf("%s",(char*)data);
  }
  Dictionary_doneIterator(&dictionaryIterator);

  Dictionary_printStatistic(&dictionary);

  Dictionary_done(&dictionary);

  return(0);
 }

#ifdef __cplusplus
  }
#endif

/* end of file */
