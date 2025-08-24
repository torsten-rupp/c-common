/***********************************************************************\
*
* Contents: tiny memory pool allocator demo
* Systems : all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common/tinymemory.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/
#define TRUE  1
#define FALSE 0

/***************************** Datatypes *******************************/
typedef int bool;

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

static void printUsage(const char *ProgramName)
 {
  fprintf(stderr,"...\n");
  fprintf(stderr,"Usage: %s\n",ProgramName);
  fprintf(stderr,"Options:\n");
 }

/*---------------------------------------------------------------------*/

int main(int argc, char *argv[])
 {
  int  z;
  bool endOfOptionsFlag;

  char       memory[32*1024];
  TinyMemory tinyMemory;
  void       *p0,*p1,*p2;

  endOfOptionsFlag = FALSE;
  z = 1;
  while (z < argc)
  {
    if (!endOfOptionsFlag)
    {
      if      ((strcmp(argv[z],"--help") == 0) || (strcmp(argv[z],"-h") == 0))
      {
        printUsage(argv[0]);
        return(0);
      }
      else if ((strcmp(argv[z],"--help") == 0) || (strcmp(argv[z],"-h") == 0))
      {
      }
      else if (strcmp(argv[z],"--") == 0)
      {
      }
      else if (strncmp(argv[z],"-",1) == 0)
      {
        fprintf(stderr,"Unknown option '%s'!\n",argv[z]);
        return 1;
      }
      else
      {
      }
    }
    else
    {
    }
    z++;
  }

  tinyMemory = TinyMemory_init(memory,sizeof(memory));
  assert(tinyMemory != NULL);

  p0 = TinyMemory_alloc(tinyMemory,256);
  p1 = TinyMemory_alloc(tinyMemory,127);
  p2 = TinyMemory_alloc(tinyMemory,12);

  TinyMemory_free(tinyMemory,p0);
  TinyMemory_free(tinyMemory,p1);
  TinyMemory_free(tinyMemory,p2);


  p0 = TinyMemory_alloc(tinyMemory,256);
  p1 = TinyMemory_realloc(tinyMemory,p0,300);
  TinyMemory_free(tinyMemory,p1);

  TinyMemory_done(tinyMemory);

  return(0);
 }

#ifdef __cplusplus
  }
#endif

/* end of file */
