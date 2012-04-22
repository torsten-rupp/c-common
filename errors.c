/***********************************************************************\
*
* $Source$
* $Revision$
* $Author$
* Contents: error handling functions
* Systems : all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "global.h"
#include "errors.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

#define MAX_ERROR_TEXT_LENGTH 256

#define MASK_ERRNO             0x0FFF

#define MASK_SYSTEM_ERROR      0x0000
#define MASK_APPLICATION_ERROR 0x8000

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/

LOCAL char lastErrorText[MAX_ERROR_TEXT_LENGTH] = {'\0'};

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

void setError(uint error, const char *text)
{
  assert(text != NULL);

  errno = (MASK_APPLICATION_ERROR | error);
  strncpy(lastErrorText,text,MAX_ERROR_TEXT_LENGTH-1);lastErrorText[MAX_ERROR_TEXT_LENGTH-1]='\0';
}

void formatError(uint error, const char *format, ...)
{
  va_list arguments;

  assert(format != NULL);

  va_start(arguments,format);
  vsnprintf(lastErrorText,MAX_ERROR_TEXT_LENGTH,format,arguments);
  va_end(arguments);
}

uint getLastError(void)
{
  return errno;
}

const char *getLastErrorString(void)
{
  return ((errno & MASK_APPLICATION_ERROR) != 0)?lastErrorText:strerror(errno);
}

#ifdef __cplusplus
  }
#endif

/* end of file */
