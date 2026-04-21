/***********************************************************************\
*
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

#include "common/global.h"
#include "common/autofree.h"

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
  fprintf(stderr,"%s, %d: p1=%p\n",__FILE__,__LINE__,p);
  AUTOFREE_ADD(&autoFreeList,p,{ fprintf(stderr,"%s, %d: called auto free: %p\n",__FILE__,__LINE__,p); free(p); });

  h = open("/dev/null",O_RDONLY);
  AUTOFREE_ADD(&autoFreeList,&h,{ fprintf(stderr,"%s, %d: called auto free: %d\n",__FILE__,__LINE__,h); close(h); });

  d = opendir(".");
  AUTOFREE_ADD(&autoFreeList,d,{ fprintf(stderr,"%s, %d: called auto free: %p\n",__FILE__,__LINE__,d); closedir(d); });

#if 0
  // test: duplicate insert
  AUTOFREE_ADD(&autoFreeList,p,{ fprintf(stderr,"%s, %d: called auto free: %p\n",__FILE__,__LINE__,p); free(p); });
#endif

  // free all resources
  AutoFree_freeAll(&autoFreeList);

  AutoFree_done(&autoFreeList);

  return 0;
 }

/* end of file */
