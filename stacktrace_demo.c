/***********************************************************************\
*
* Contents: stack trace demo
* Systems: all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common/global.h"
#include "common/stacktraces.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

/*---------------------------------------------------------------------*/

LOCAL void function1(int n)
{
  fprintf(stderr,"%s:%d: n=%d\n",__FILE__,__LINE__,n);
  debugPrintStackTrace();
}

LOCAL void function2(int n)
{
  function1(n);
}

LOCAL void function3(int n)
{
  function2(n);
}

int main(int argc, char *argv[])
 {
   UNUSED_VARIABLE(argc);
   UNUSED_VARIABLE(argv);

   function3(123);

   return(0);
 }

/* end of file */
