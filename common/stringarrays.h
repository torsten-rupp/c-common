/***********************************************************************\
*
* $Source: /home/torsten/cvs/bar/arrays.h,v $
* $Revision: 1369 $
* $Author: torsten $
* Contents: dynamic string array functions
* Systems: all
*
\***********************************************************************/

#ifndef __STRINGARRAYS__
#define __STRINGARRAYS__

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "global.h"
#include "strings.h"
#include "arrays.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/* comparison, iteration functions */
typedef int(*StringArrayElementCompareFunction)(void *userData, const String string1, const String string2);
typedef char(*StringArrayElementIterateFunction)(void *userData, const String string);

/***************************** Variables *******************************/

/****************************** Macros *********************************/

#ifndef NDEBUG
  #define StringArray_new(length) __StringArray_new(__FILE__,__LINE__,length)
#endif /* not NDEBUG */

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

/***********************************************************************\
* Name   : StringArray_new
* Purpose: create new string array
* Input  : length - start length of array
* Output : -
* Return : array or NULL
* Notes  : -
\***********************************************************************/

#ifdef NDEBUG
Array *StringArray_new(ulong length);
#else /* not NDEBUG */
Array *__StringArray_new(const char *fileName, ulong lineNb, ulong length);
#endif /* NDEBUG */

/***********************************************************************\
* Name   : StringArray_delete
* Purpose: delete string array
* Input  : array - string array to delete
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

void StringArray_delete(Array *array);
#if defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)
void StringArray_delete(Array *array)
{
  assert(array != NULL);

  Array_delete(array);
}
#endif // defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)

/***********************************************************************\
* Name   : StringArray_clear
* Purpose: clear string array
* Input  : array - string array to clear
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

INLINE void StringArray_clear(Array *array);
#if defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)
INLINE void StringArray_clear(Array *array)
{
  assert(array != NULL);

  Array_clear(array);
}
#endif // defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)

/***********************************************************************\
* Name   : StringArray_length
* Purpose: get string array length
* Input  : array - string array
* Output : -
* Return : number of strings in string array
* Notes  : -
\***********************************************************************/

INLINE ulong StringArray_length(Array *array);
#if defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)
INLINE ulong StringArray_length(Array *array)
{
  assert(array != NULL);

  return Array_length(array);
}
#endif // defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)

/***********************************************************************\
* Name   : StringArray_put
* Purpose: put string into string array
* Input  : array  - string array
*          index  - index of element
*          string - string (will be duplicated)
* Output : -
* Return : TRUE if element stored in array, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool StringArray_put(Array *array, ulong index, const String string);

/***********************************************************************\
* Name   : StringArray_get
* Purpose: get string from string array
* Input  : array  - string array
*          index  - index of element
*          string - variable for data (can be NULL)
* Output : string - string
* Return : string variable or reference to element in array
* Notes  : if no string variable is supplied (NULL) a reference to the
*          string is returned
\***********************************************************************/

String StringArray_get(Array *array, ulong index, String string);

/***********************************************************************\
* Name   : StringArray_insert
* Purpose: insert string into string array
* Input  : array     - string array
*          nextIndex - index of next element or ARRAY_END
*          string    - string to insert
* Output : -
* Return : TRUE if element inserted, FALSE otherweise
* Notes  : -
\***********************************************************************/

bool StringArray_insert(Array *array, long nextIndex, const String string);

/***********************************************************************\
* Name   : StringArray_append
* Purpose: append string to string array
* Input  : array  - string array
*          string - string
* Output : -
* Return : TRUE if element appended, FALSE otherweise
* Notes  : -
\***********************************************************************/

bool StringArray_append(Array *array, const String string);

/***********************************************************************\
* Name   : StringArray_remove
* Purpose: remove string from string array
* Input  : array - string array
*          index - index of element to remove
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

INLINE void StringArray_remove(Array *array, ulong index);
#if defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)
INLINE void StringArray_remove(Array *array, ulong index)
{
  Array_remove(array,index);
}
#endif // defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)

/***********************************************************************\
* Name   : StringArray_toCArray
* Purpose: get C-array data pointer
* Input  : array - string array
* Output : -
* Return : C-array with strings
* Notes  : -
\***********************************************************************/

INLINE const String *StringArray_cArray(const Array *array);
#if defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)
INLINE const String *StringArray_cArray(const Array *array)
{
  return (const String*)Array_cArray(array);
}
#endif // defined(NDEBUG) || defined(__STRINGARRAYS_IMPLEMENATION__)

#ifdef __cplusplus
  }
#endif

#endif /* __STRINGARRAYS__ */

/* end of file */
