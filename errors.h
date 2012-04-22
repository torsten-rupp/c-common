/***********************************************************************\
*
* $Source$
* $Revision$
* $Author$
* Contents: error handling functions
* Systems : all
*
\***********************************************************************/

#ifndef __ERRORS__
#define __ERRORS__

/****************************** Includes *******************************/
#include <stdlib.h>
#include <assert.h>

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

void setError(uint error, const char *text);

void formatError(uint error, const char *format, ...);

uint getLastError(void);

const char *getLastErrorString(void);


#ifdef __cplusplus
  }
#endif

#endif /* __ERRORS__ */

/* end of file */
