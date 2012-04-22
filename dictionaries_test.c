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
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <utime.h>
#include <sys/statvfs.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
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

/***********************************************************************\
* Name       : PrintUsage
* Purpose    : print "usage" help
* Input      : -
* Output     : -
* Return     : -
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

void printUsage(const char *ProgramName)
 {
  fprintf(stderr,"...\n");
  fprintf(stderr,"Usage: %s\n",ProgramName);
  fprintf(stderr,"Options:\n");
 }

uint64 getTimestamp(void)
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

void readFiles(Dictionary *dictionary, const char *path)
{
  DIR           *dir;
  struct dirent *entry;
  struct stat   fileStat;
  char          fileName[1024];

  dir = opendir(path);
  if (dir != NULL)
  {
    entry = readdir(dir);
    while (entry != NULL)
    {
      if (   (strcmp(entry->d_name,"." ) != 0)
          && (strcmp(entry->d_name,"..") != 0)
         )
      {
        strcpy(fileName,path);
        strcat(fileName,"/");
        strcat(fileName,entry->d_name);
//fprintf(stderr,"%s,%d: %s\n",__FILE__,__LINE__,entry->d_name);
        Dictionary_add(dictionary,
                       fileName,
                       strlen(fileName),
                       fileName,
                       strlen(fileName)
                      );
        
        if (lstat(fileName,&fileStat) == 0)
        {
          if (S_ISDIR(fileStat.st_mode))
          {
fprintf(stderr,"%s,%d: %s\n",__FILE__,__LINE__,fileName);
            readFiles(dictionary,fileName);
          }
        }
      }
      entry = readdir(dir);
    }
    closedir(dir);
  }
}

/*---------------------------------------------------------------------*/

int main(int argc, char *argv[])
 {
  Dictionary         dictionary;
  DictionaryIterator dictionaryIterator;
  const char         *fileName;
  FILE               *file;
  const void         *data;
  ulong              length;
  uint               n;
  uint               maxLength;

  Dictionary_init(&dictionary,NULL,NULL);

  // read all files from /home/torsten
  readFiles(&dictionary,"/home/torsten/tmp");

  maxLength = 0;
  Dictionary_initIterator(&dictionaryIterator,&dictionary);
  while (Dictionary_getNext(&dictionaryIterator,&data,&length,NULL,0))
  {
    if (length > maxLength) maxLength = length;
  }
  Dictionary_doneIterator(&dictionaryIterator);
fprintf(stderr,"%s,%d: maxLength=%u\n",__FILE__,__LINE__,maxLength);

  Dictionary_printStatistic(&dictionary);

  Dictionary_done(&dictionary,NULL,NULL);

  return(0);
 }

#ifdef __cplusplus
  }
#endif

/* end of file */
