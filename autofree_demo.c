/***********************************************************************\
*
* $Source$
* $Revision: 1102 $
* $Author: torsten $
* Contents: auto-free demo
* Systems: all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <assert.h>

#include "global.h"
#include "autofree.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

/*---------------------------------------------------------------------*/

int main(int argc, char *argv[])
 {
  AutoFreeList autoFreeList;
  void         *p;
  int          h;
  DIR          *d;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  AutoFree_init(&autoFreeList);

  // allocate and add some resources
  p = malloc(1000);
  AUTOFREE_ADD(&autoFreeList,p,void*,{ free(resource); });

  h = open("/dev/null",O_RDONLY);
  AUTOFREE_ADD(&autoFreeList,h,int,{ close(resource); });

  d = opendir(".");
  AUTOFREE_ADD(&autoFreeList,d,DIR*,{ closedir(resource); });

#if 0
  // test: duplicate insert
  AUTOFREE_ADD(&autoFreeList,p,void*,{ free(resource); });
#endif

  // free all resources
  AutoFree_freeAll(&autoFreeList);

  AutoFree_done(&autoFreeList);

  return 0;
 }

/* end of file */
