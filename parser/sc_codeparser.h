/***********************************************************************\
*
* Contents: code parser functions
* Systems : all
*
\***********************************************************************/

#ifndef __SC_CODEPARSER__
#define __SC_CODEPARSER__

/****************************** Includes *******************************/
#include <stdlib.h>

#include "global.h"
#include "sc_ir.h"

#include "sc_codeparser_code.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/
extern bool scCodeParser_debug;

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

/***********************************************************************\
* Name   : scCodeParser_parseFile
* Purpose: parse file
* Input  : filename  - filename
*          quietFlag - TRUE to surpress output
* Output : -
* Return : TRUE if no error occurred, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool scCodeParser_parseFile(const char *filename, IRList *irList, bool quietFlag);

/***********************************************************************\
* Name   : scCodeParser_resolveReferences
* Purpose: resolve references
* Input  : irList - intermediate representation list
* Output : -
* Return : TRUE if no error occurred, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool scCodeParser_resolveReferences(IRList *irList);

/***********************************************************************\
* Name   : scCodeParser_verify
* Purpose: verify
* Input  : irList - intermediate representation list
* Output : -
* Return : TRUE if no error occurred, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool scCodeParser_verify(IRList *irList);

#ifdef __cplusplus
  }
#endif

#endif /* __SC_CODEPARSER__ */

/* end of file */
