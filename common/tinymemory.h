/***********************************************************************\
*
* Contents: 
* Systems :
*
\***********************************************************************/

#ifndef __TINYMEMORY__
#define __TINYMEMORY__

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/
typedef struct __TinyMemory *TinyMemory;

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

/***********************************************************************\
* Name   : tinyMemory_init
* Purpose: initialize tiny memory
* Input  : p    - memory
*          size - memory size
* Output : -
* Return : tiny memory or NULL on error
* Notes  : -
\***********************************************************************/

TinyMemory tinyMemory_init(void *p, ulong size);

/***********************************************************************\
* Name   : tinyMemory_done
* Purpose: deinitialize tiny memory
* Input  : tinyMemory - tiny memory
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

void tinyMemory_done(TinyMemory tinyMemory);

/***********************************************************************\
* Name   : tinyMemory_alloc
* Purpose: allocate memory block
* Input  : tinyMemory - tiny memory
*          size       - size of memory block
* Output : -
* Return : memory block or NULL on insufficient memory
* Notes  : -
\***********************************************************************/

void *tinyMemory_alloc(TinyMemory tinyMemory, ulong size);

/***********************************************************************\
* Name   : tinyMemory_realloc
* Purpose: reallocate memory block
* Input  : tinyMemory - tiny memory
*          p          - memory block
*          newSize    - new size of memory block
* Output : -
* Return : reallocated memory block or NULL on insufficient memory
* Notes  : -
\***********************************************************************/

void *tinyMemory_realloc(TinyMemory tinyMemory, void *p, ulong newSize);

/***********************************************************************\
* Name   : tinyMemory_free
* Purpose: free memory block
* Input  : tinyMemory - tiny memory
*          p          - memory block
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

void tinyMemory_free(TinyMemory tinyMemory, void *p);

#ifdef __cplusplus
  }
#endif

#endif /* __TINYMEMORY__ */

/* end of file */
