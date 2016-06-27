/***********************************************************************\
*
* $Source: /home/torsten/cvs/bar/arrays.c,v $
* $Revision$
* $Author$
* Contents: dynamic string array functions
* Systems: all
*
\***********************************************************************/
#define __STRINGARRAYS_IMPLEMENATION__

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "global.h"
#include "arrays.h"
#include "string.h"

#include "stringarrays.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

LOCAL void freeStringArrayElement(String *string, void *dummy)
{
  assert(string != NULL);

  UNUSED_VARIABLE(dummy);

  String_delete(*string);
}

#ifdef NDEBUG
Array *StringArray_new(ulong length)
#else /* not NDEBUG */
Array *__StringArray_new(const char *fileName, ulong lineNb, ulong length)
#endif /* NDEBUG */
{
  #ifdef NDEBUG
    return Array_new(sizeof(String),
                     length
                     CALLBACK((ArrayFreeFunction)freeStringArrayElement,NULL),
                     CALLBACK_NULL
                    );
  #else /* not NDEBUG */
    return __Array_new(fileName,
                       lineNb,
                       sizeof(String),
                       length,
                       CALLBACK((ArrayFreeFunction)freeStringArrayElement,NULL),
                       CALLBACK_NULL
                      );
  #endif /* NDEBUG */
}

bool StringArray_put(Array *array, ulong index, const String string)
{
  String newString;

  assert(array != NULL);

  newString = String_duplicate(string);
  if (newString == NULL)
  {
    return FALSE;
  }

  if (!Array_put(array,index,&newString))
  {
    String_delete(newString);
    return FALSE;
  }

  return TRUE;
}

String StringArray_get(Array *array, ulong index, String string)
{
  String arrayString;

  if (Array_get(array,index,&arrayString) != NULL)
  {
    if (string != NULL)
    {
      return String_set(string,arrayString);
    }
    else
    {
      return String_duplicate(arrayString);
    }
  }
  else
  {
    return NULL;
  }
}

bool StringArray_insert(Array *array, long nextIndex, const String string)
{
  String newString;

  assert(array != NULL);

  newString = String_duplicate(string);
  if (newString == NULL)
  {
    return FALSE;
  }

  if (!Array_insert(array,nextIndex,&newString))
  {
    String_delete(newString);
    return FALSE;
  }

  return TRUE;
}

bool StringArray_append(Array *array, const String string)
{
  String newString;

  assert(array != NULL);

  newString = String_duplicate(string);
  if (newString == NULL)
  {
    return FALSE;
  }

  if (!Array_append(array,&newString))
  {
    String_delete(newString);
    return FALSE;
  }

  return TRUE;
}

#ifdef __cplusplus
  }
#endif

/* end of file */
