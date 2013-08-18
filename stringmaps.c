/***********************************************************************\
*
* $Revision: 1092 $
* $Date: 2013-07-28 05:34:57 +0200 (Sun, 28 Jul 2013) $
* $Author: torsten $
* Contents:
* Systems: all
*
\***********************************************************************/

#define __STRINGMAPS_IMPLEMENATION__

/****************************** Includes *******************************/
#include <config.h>  // use <...> to support separated build directory

#include <stdlib.h>
#include <stdio.h>
#if defined(HAVE_PCRE)
  #include <pcreposix.h>
#elif defined(HAVE_REGEX_H)
  #include <regex.h>
#else
  #error No regular expression library available!
#endif /* HAVE_PCRE || HAVE_REGEX_H */
#include <assert.h>

#include "lists.h"
#include "strings.h"

#include "stringmaps.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/
#define STRINGMAP_START_SIZE 16   // string map start size
#define STRINGMAP_DELTA_SIZE 16   // string map delta increasing size

const StringMapValue STRINGMAP_VALUE_NONE = {0};

/***************************** Datatypes *******************************/

struct __StringMap
{
  uint           size;
  StringMapEntry *stringMapEntries;
};

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

/***********************************************************************\
* Name   : calculateHash
* Purpose: calculate hash
* Input  : data   - data
*          length - length of data
* Output : -
* Return : hash value
* Notes  : -
\***********************************************************************/

LOCAL uint calculateHash(const char *name)
{
  uint n;
  byte hashBytes[4];
  uint i;

  assert(name != NULL);

  n = strlen(name);

  hashBytes[0] = (n > 0) ? name[0] : 0;
  hashBytes[1] = (n > 1) ? name[1] : 0;
  hashBytes[2] = (n > 2) ? name[2] : 0;
  hashBytes[3] = (n > 3) ? name[3] : 0;
  for (i = 4; i < n; i++)
  {
    hashBytes[i%4] ^= name[i];
  }

  return (uint)(hashBytes[3] << 24) |
         (uint)(hashBytes[2] << 16) |
         (uint)(hashBytes[1] <<  8) |
         (uint)(hashBytes[0] <<  0);
}

/***********************************************************************\
* Name   : addStringMapEntry
* Purpose: add string entry to string map
* Input  : __fileName__ - file naem (debug only)
*          __lineNb__   - line number (debug only)
*          stringMap    - string map
*          name         - name
* Output : -
* Return : string map entry or NULL
* Notes  : -
\***********************************************************************/

#ifdef NDEBUG
LOCAL StringMapEntry *addStringMapEntry(StringMap *stringMap, const char *name)
#else /* not NDEBUG */
LOCAL StringMapEntry *addStringMapEntry(const char *__fileName__, ulong __lineNb__, struct __StringMap *stringMap, const char *name)
#endif /* NDEBUG */
{
  uint           i;
  uint           n;
  StringMapEntry *newStringMapEntries;

  assert(stringMap != NULL);

  i = calculateHash(name)%stringMap->size;
  n = 0;
  while (   (stringMap->stringMapEntries[i].name != NULL)
         && (n < stringMap->size)
        )
  {
    i = (i+1)%stringMap->size;
    n++;
  }

  if (n >= stringMap->size)
  {
    newStringMapEntries = (StringMapEntry*)realloc(stringMap->stringMapEntries,sizeof(StringMapEntry)*(stringMap->size+STRINGMAP_DELTA_SIZE));
    if (newStringMapEntries == NULL)
    {
      return NULL;
    }
    for (i = stringMap->size; i < stringMap->size+STRINGMAP_DELTA_SIZE; i++)
    {
      stringMap->stringMapEntries[i].name = NULL;
    }
    stringMap->size += STRINGMAP_DELTA_SIZE;
  }

  if (n < stringMap->size)
  {
    stringMap->stringMapEntries[i].name = strdup(name);
    #ifndef NDEBUG
      stringMap->stringMapEntries[i].fileName  = __fileName__;
      stringMap->stringMapEntries[i].lineNb    = __lineNb__;
    #endif /* NDEBUG */

    return &stringMap->stringMapEntries[i];
  }
  else
  {
    return NULL;
  }
}

/***********************************************************************\
* Name   : findStringMapEntry
* Purpose: find string entry in string map
* Input  : stringMap - string map
*          name      - name
* Output : -
* Return : string map entry or NULL
* Notes  : -
\***********************************************************************/

LOCAL StringMapEntry *findStringMapEntry(const struct __StringMap *stringMap, const char *name)
{
  uint i;
  uint n;

  assert(stringMap != NULL);

  i = calculateHash(name)%stringMap->size;
  n = 0;
  while (   ((stringMap->stringMapEntries[i].name == NULL) || (strcmp(stringMap->stringMapEntries[i].name,name) != 0))
         && (n < stringMap->size)
        )
  {
    i = (i+1)%stringMap->size;
    n++;
  }

  return (n < stringMap->size) ? &stringMap->stringMapEntries[i] : NULL;
}

/*---------------------------------------------------------------------*/

#ifdef NDEBUG
StringMap StringMap_new(uint size)
#else /* not NDEBUG */
StringMap __StringMap_new(const char *__fileName__, ulong __lineNb__)
#endif /* NDEBUG */
{
  struct __StringMap *stringMap;
  uint               i;

  stringMap = (struct __StringMap *)malloc(sizeof(struct __StringMap));
  if (stringMap == NULL)
  {
    return NULL;
  }

  stringMap->size             = STRINGMAP_START_SIZE;
  stringMap->stringMapEntries = malloc(sizeof(StringMapEntry)*STRINGMAP_START_SIZE);
  if (stringMap == NULL)
  {
    free(stringMap);
    return NULL;
  }
  for (i = 0; i < STRINGMAP_START_SIZE; i++)
  {
    stringMap->stringMapEntries[i].name = NULL;
  }

  #ifdef NDEBUG
    DEBUG_ADD_RESOURCE_TRACE("stringMap",stringMap);
  #else /* not NDEBUG */
    DEBUG_ADD_RESOURCE_TRACEX(__fileName__,__lineNb__,"stringMap",stringMap);
  #endif /* NDEBUG */

  return stringMap;
}

#ifdef NDEBUG
StringMap StringMap_duplicate(const StringMap stringMap)
#else /* not NDEBUG */
StringMap __StringMap_duplicate(const char *__fileName__, ulong __lineNb__, const StringMap stringMap)
#endif /* NDEBUG */
{
  struct __StringMap *newStringMap;

  assert(stringMap != NULL);

  #ifdef NDEBUG
    newStringMap = StringMap_new(stringMap->size);
  #else /* not NDEBUG */
    newStringMap = __StringMap_new(__fileName__,__lineNb__);
  #endif /* NDEBUG */
  if (newStringMap == NULL)
  {
    return NULL;
  }

  StringMap_copy(newStringMap,stringMap);

  return newStringMap;
}

void StringMap_copy(StringMap stringMap, const StringMap fromStringMap)
{
  uint i;

  assert(stringMap != NULL);
  assert(fromStringMap != NULL);

  stringMap->size = fromStringMap->size;
  for (i = 0; i < stringMap->size; i++)
  {
    if (fromStringMap->stringMapEntries[i].name != NULL)
    {
      stringMap->stringMapEntries[i].name  = strdup(fromStringMap->stringMapEntries[i].name);
      stringMap->stringMapEntries[i].value = fromStringMap->stringMapEntries[i].value;
    }
    else
    {
      stringMap->stringMapEntries[i].name = NULL;
    }
  }
}

#ifdef NDEBUG
void StringMap_delete(StringMap stringMap)
#else /* not NDEBUG */
void __StringMap_delete(const char *__fileName__, ulong __lineNb__, StringMap stringMap)
#endif /* NDEBUG */
{
  uint i;

  assert(stringMap != NULL);

  #ifdef NDEBUG
    DEBUG_REMOVE_RESOURCE_TRACE(stringMap);
  #else /* not NDEBUG */
    DEBUG_REMOVE_RESOURCE_TRACEX(__fileName__,__lineNb__,stringMap);
  #endif /* NDEBUG */

  for (i = 0; i < stringMap->size; i++)
  {
    if (stringMap->stringMapEntries[i].name != NULL)
    {
      switch (stringMap->stringMapEntries[i].type)
      {
        case STRINGMAP_TYPE_INT:
        case STRINGMAP_TYPE_INT64:
        case STRINGMAP_TYPE_UINT:
        case STRINGMAP_TYPE_UINT64:
        case STRINGMAP_TYPE_DOUBLE:
        case STRINGMAP_TYPE_BOOL:
        case STRINGMAP_TYPE_CHAR:
        case STRINGMAP_TYPE_VOID:
          break;
        case STRINGMAP_TYPE_CSTRING:
          free(stringMap->stringMapEntries[i].value.s);
          break;
        case STRINGMAP_TYPE_STRING:
          String_delete(stringMap->stringMapEntries[i].value.string);
          break;
      }
      free(stringMap->stringMapEntries[i].name);
    }
  }
  free(stringMap->stringMapEntries);
  free(stringMap);
}

StringMap StringMap_clear(StringMap stringMap)
{
  uint i;

  assert(stringMap != NULL);

  for (i = 0; i < stringMap->size; i++)
  {
    if (stringMap->stringMapEntries[i].name != NULL)
    {
      free(stringMap->stringMapEntries[i].name);
      stringMap->stringMapEntries[i].name = NULL;
    }
  }

  return stringMap;
}

uint StringMap_count(const StringMap stringMap)
{
  uint count;
  uint i;

  assert(stringMap != NULL);

  count = 0L;
  for (i = 0; i < stringMap->size; i++)
  {
    if (stringMap->stringMapEntries[i].name != NULL)
    {
      count++;
    }
  }

  return count;
}

const StringMapEntry *StringMap_index(const StringMap stringMap, uint index)
{
  StringMapEntry *stringMapEntry;
  uint           i;

  assert(stringMap != NULL);

  stringMapEntry = NULL;
  for (i = 0; i < stringMap->size; i++)
  {
    if (stringMap->stringMapEntries[i].name != NULL)
    {
      if (index > 0)
      {
        index--;
      }
      else
      {
        stringMapEntry = &stringMap->stringMapEntries[i];
        break;
      }
    }
  }

  return stringMapEntry;
}

const char *StringMap_indexName(const StringMap stringMap, uint index)
{
  const StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);

  stringMapEntry = StringMap_index(stringMap,index);
  return (stringMapEntry != NULL) ? stringMapEntry->name : NULL;
}

StringMapValue StringMap_indexValue(const StringMap stringMap, uint index)
{
  const StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);

  stringMapEntry = StringMap_index(stringMap,index);
  return (stringMapEntry != NULL) ? stringMapEntry->value : STRINGMAP_VALUE_NONE;
}

StringMapValue StringMap_get(const StringMap stringMap, const char *name)
{
  const StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    return stringMapEntry->value;
  }
  else
  {
    return STRINGMAP_VALUE_NONE;
  }
}

#ifdef NDEBUG
void StringMap_put(StringMap stringMap, const char *name, void *value)
#else /* not NDEBUG */
void __StringMap_put(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name, void *value)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  #ifdef NDEBUG
    stringMapEntry = addStringMapEntry(stringMap,name);
  #else /* not NDEBUG */
    stringMapEntry = addStringMapEntry(__fileName__,__lineNb__,stringMap,name);
  #endif /* NDEBUG */
  if (stringMapEntry != NULL)
  {
    stringMapEntry->type    = STRINGMAP_TYPE_STRING;
    stringMapEntry->value.p = value;
  }
}

#ifdef NDEBUG
void StringMap_putInt(StringMap stringMap, const char *name, int value)
#else /* not NDEBUG */
void __StringMap_putInt(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name, int value)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  #ifdef NDEBUG
    stringMapEntry = addStringMapEntry(stringMap,name);
  #else /* not NDEBUG */
    stringMapEntry = addStringMapEntry(__fileName__,__lineNb__,stringMap,name);
  #endif /* NDEBUG */
  if (stringMapEntry != NULL)
  {
    stringMapEntry->type    = STRINGMAP_TYPE_INT;
    stringMapEntry->value.i = value;
  }
}

#ifdef NDEBUG
void StringMap_putInt64(StringMap stringMap, const char *name, int64 value)
#else /* not NDEBUG */
void __StringMap_putInt64(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name, int64 value)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  #ifdef NDEBUG
    stringMapEntry = addStringMapEntry(stringMap,name);
  #else /* not NDEBUG */
    stringMapEntry = addStringMapEntry(__fileName__,__lineNb__,stringMap,name);
  #endif /* NDEBUG */
  if (stringMapEntry != NULL)
  {
    stringMapEntry->type    = STRINGMAP_TYPE_INT64;
    stringMapEntry->value.l = value;
  }
}

#ifdef NDEBUG
void StringMap_putDouble(StringMap stringMap, const char *name, double value)
#else /* not NDEBUG */
void __StringMap_putDouble(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name, double value)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  #ifdef NDEBUG
    stringMapEntry = addStringMapEntry(stringMap,name);
  #else /* not NDEBUG */
    stringMapEntry = addStringMapEntry(__fileName__,__lineNb__,stringMap,name);
  #endif /* NDEBUG */
  if (stringMapEntry != NULL)
  {
    stringMapEntry->type    = STRINGMAP_TYPE_DOUBLE;
    stringMapEntry->value.d = value;
  }
}

#ifdef NDEBUG
void StringMap_putBool(StringMap stringMap, const char *name, bool value)
#else /* not NDEBUG */
void __StringMap_putBool(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name, bool value)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  #ifdef NDEBUG
    stringMapEntry = addStringMapEntry(stringMap,name);
  #else /* not NDEBUG */
    stringMapEntry = addStringMapEntry(__fileName__,__lineNb__,stringMap,name);
  #endif /* NDEBUG */
  if (stringMapEntry != NULL)
  {
    stringMapEntry->type    = STRINGMAP_TYPE_BOOL;
    stringMapEntry->value.b = value;
  }
}

#ifdef NDEBUG
void StringMap_putChar(StringMap stringMap, const char *name, char value)
#else /* not NDEBUG */
void __StringMap_putChar(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name, char value)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  #ifdef NDEBUG
    stringMapEntry = addStringMapEntry(stringMap,name);
  #else /* not NDEBUG */
    stringMapEntry = addStringMapEntry(__fileName__,__lineNb__,stringMap,name);
  #endif /* NDEBUG */
  if (stringMapEntry != NULL)
  {
    stringMapEntry->type    = STRINGMAP_TYPE_CHAR;
    stringMapEntry->value.c = value;
  }
}

#ifdef NDEBUG
void StringMap_putCString(StringMap stringMap, const char *name, const char *value)
#else /* not NDEBUG */
void __StringMap_putCString(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name, const char *value)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  #ifdef NDEBUG
    stringMapEntry = addStringMapEntry(stringMap,name);
  #else /* not NDEBUG */
    stringMapEntry = addStringMapEntry(__fileName__,__lineNb__,stringMap,name);
  #endif /* NDEBUG */

  if (stringMapEntry != NULL)
  {
    stringMapEntry->type    = STRINGMAP_TYPE_CSTRING;
    stringMapEntry->value.s = strdup(value);
  }
}

#ifdef NDEBUG
void StringMap_putString(StringMap stringMap, const char *name, String value)
#else /* not NDEBUG */
void __StringMap_putString(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name, String value)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  #ifdef NDEBUG
    stringMapEntry = addStringMapEntry(stringMap,name);
  #else /* not NDEBUG */
    stringMapEntry = addStringMapEntry(__fileName__,__lineNb__,stringMap,name);
  #endif /* NDEBUG */

  if (stringMapEntry != NULL)
  {
    stringMapEntry->type         = STRINGMAP_TYPE_STRING;
    stringMapEntry->value.string = String_duplicate(value);
  }
}

bool StringMap_getInt(const StringMap stringMap, const char *name, int *value, int defaultValue)
{
  StringMapEntry *stringMapEntry;
  char           *nextData;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    (*value) = strtol(String_cString(stringMapEntry->value.string),&nextData,0);
    return ((*nextData) == '\0');
  }
  else
  {
    (*value) = defaultValue;
    return FALSE;
  }
}

bool StringMap_getInt64(const StringMap stringMap, const char *name, int64 *value, int64 defaultValue)
{
  StringMapEntry *stringMapEntry;
  char           *nextData;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    (*value) = strtoll(String_cString(stringMapEntry->value.string),&nextData,0);
    return ((*nextData) == '\0');
  }
  else
  {
    (*value) = defaultValue;
    return FALSE;
  }
}

bool StringMap_getUInt(const StringMap stringMap, const char *name, int *value, uint defaultValue)
{
  StringMapEntry *stringMapEntry;
  char           *nextData;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    (*value) = (uint)strtol(String_cString(stringMapEntry->value.string),&nextData,0);
    return ((*nextData) == '\0');
  }
  else
  {
    (*value) = defaultValue;
    return FALSE;
  }
}

bool StringMap_getUInt64(const StringMap stringMap, const char *name, int64 *value, uint64 defaultValue)
{
  StringMapEntry *stringMapEntry;
  char           *nextData;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    (*value) = (uint64)strtoll(String_cString(stringMapEntry->value.string),&nextData,0);
    return ((*nextData) == '\0');
  }
  else
  {
    (*value) = defaultValue;
    return FALSE;
  }
}

bool StringMap_getDouble(const StringMap stringMap, const char *name, double *value, double defaultValue)
{
  StringMapEntry *stringMapEntry;
  char           *nextData;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    (*value) = strtod(String_cString(stringMapEntry->value.string),&nextData);
    return ((*nextData) == '\0');
  }
  else
  {
    (*value) = defaultValue;
    return FALSE;
  }
}

bool StringMap_getBool(const StringMap stringMap, const char *name, bool *value, bool defaultValue)
{
  const char *TRUE_STRINGS[] =
  {
    "1",
    "true",
    "yes",
    "on",
  };

  StringMapEntry *stringMapEntry;
  uint           z;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    (*value) = FALSE;
    for (z = 0; z < SIZE_OF_ARRAY(TRUE_STRINGS); z++)
    {
      if (String_equalsIgnoreCaseCString(stringMapEntry->value.string,TRUE_STRINGS[z]))
      {
        (*value) = TRUE;
      }
    }
    return TRUE;
  }
  else
  {
    (*value) = defaultValue;
    return FALSE;
  }
}

bool StringMap_getEnum(const StringMap stringMap, const char *name, void *value, StringMapToEnumFunction stringMapToEnumFunction, int defaultValue)
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);
  assert(stringMapToEnumFunction != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    (*(int*)value) = stringMapToEnumFunction(String_cString(stringMapEntry->value.string));
    return TRUE;
  }
  else
  {
    (*(int*)value) = defaultValue;
    return FALSE;
  }
}

bool StringMap_getChar(const StringMap stringMap, const char *name, char *value, char defaultValue)
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    if (String_length(stringMapEntry->value.string) > 0)
    {
      (*value) = String_index(stringMapEntry->value.string,0);
      return TRUE;
    }
    (*value) = defaultValue;
    return FALSE;
  }
  else
  {
    (*value) = defaultValue;
    return FALSE;
  }
}

bool StringMap_getCString(const StringMap stringMap, const char *name, char *value, uint maxLength, const char *defaultValue)
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);
  assert(maxLength > 0);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    strncpy(value,String_cString(stringMapEntry->value.string),maxLength);
    return TRUE;
  }
  else
  {
    if (defaultValue != NULL)
    {
      strncpy(value,defaultValue,maxLength);
    }
    else
    {
      value[0] = '\0';
    }
    return FALSE;
  }
}

bool StringMap_getString(const StringMap stringMap, const char *name, String value, const String defaultValue)
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);
  assert(value != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    String_set(value,stringMapEntry->value.string);
    return TRUE;
  }
  else
  {
    if (defaultValue != NULL)
    {
      String_set(value,defaultValue);
    }
    else
    {
      String_clear(value);
    }
    return FALSE;
  }
}

#ifdef NDEBUG
void StringMap_remove(StringMap stringMap, const char *name)
#else /* not NDEBUG */
void __StringMap_remove(const char *__fileName__, ulong __lineNb__, StringMap stringMap, const char *name)
#endif /* NDEBUG */
{
  StringMapEntry *stringMapEntry;

  assert(stringMap != NULL);
  assert(name != NULL);

  stringMapEntry = findStringMapEntry(stringMap,name);
  if (stringMapEntry != NULL)
  {
    free(stringMapEntry->name); stringMapEntry->name = NULL;
    #ifndef NDEBUG
      stringMapEntry->fileName = __fileName__;
      stringMapEntry->lineNb   = __lineNb__;
    #endif /* NDEBUG */
  }
}

StringMapEntry *StringMap_find(const StringMap stringMap, const char *name)
{
  assert(stringMap != NULL);
  assert(name != NULL);

  return findStringMapEntry(stringMap,name);
}

bool StringMap_contain(const StringMap stringMap, const char *name)
{
  assert(stringMap != NULL);
  assert(name != NULL);

  return (findStringMapEntry(stringMap,name) != NULL);
}

#if 0
LOCAL const StringMapType *getMapParseType(const StringMapType *types, uint typeCount, const char *name)
{
  const StringMapType *stringMapType;
  uint                i;

  assert(types != NULL);

  stringMapType = NULL;

  for (i = 0; i < typeCount; i++)
  {
    if (strcmp(types[i].name,name) == 0)
    {
      stringMapType = &types[i];
      break;
    }
  }

  return stringMapType;
}
#endif

bool StringMap_parse(StringMap stringMap, const String string, char quoteChar, ulong index, long *nextIndex)
{
  assert(stringMap != NULL);

  STRING_CHECK_VALID(string);

  return StringMap_parseCString(stringMap,String_cString(string),quoteChar,index,nextIndex);
}

bool StringMap_parseCString(StringMap stringMap, const char *s, char quoteChar, ulong index, long *nextIndex)
{
  uint   length;
  String name;
  String value;
  int    i;

  assert(stringMap != NULL);
  assert(s != NULL);

  // parse
  length = strlen(s);
  name   = String_new();
  value  = String_new();

  index = STRING_BEGIN;
  while (index < length)
  {
    // skip spaces
    while ((index < length) && isspace(s[index]))
    {
      index++;
    }

    // get name
    String_clear(name);
    if (   (index < length)
        && (!isalpha(s[index]) && (s[index] != '_'))
       )
    {
      if (nextIndex != NULL) (*nextIndex) = index;
      String_delete(value);
      String_delete(name);
      return FALSE;
    }
    do
    {
      String_appendChar(name,s[index]);
      index++;
    }
    while (   (index < length)
           && (isalnum(s[index]) || (s[index] == '_'))
          );

    // skip spaces
    while ((index < length) && isspace(s[index]))
    {
      index++;
    }

    // check '='
    if (   (index >= length) || (s[index] != '='))
    {
      if (nextIndex != NULL) (*nextIndex) = index;
      String_delete(value);
      String_delete(name);
      return FALSE;
    }
    index++;

    // skip spaces
    while ((index < length) && isspace(s[index]))
    {
      index++;
    }

    // get value
    String_clear(value);
    while ((index < length) && !isspace(s[index]))
    {
      if (   (s[index] == '\\')
          && ((index+1) < length)
          && (s[index+1] == quoteChar)
         )
      {
        // quoted quote
        String_appendChar(value, s[index+1]);
        index += 2;
      }
      else
      {
        // check for string quote
        if (s[index] == quoteChar)
        {
          do
          {
            // skip quote-char
            index++;

            // get string
            while ((index < length) && (s[index] != quoteChar))
            {
              if (   ((index+1) < length)
                  && (s[index] == '\\')
                 )
              {
                index++;

                if      (s[index] == quoteChar)
                {
                  // quoted quote
                  String_appendChar(value,s[index]);
                }
                else
                {
                  // check if escaped character
                  i = STRING_ESCAPE_LENGTH-1;
                  while ((i >= 0) && (STRING_ESCAPE_MAP[i] != s[index]))
                  {
                    i--;
                  }

                  if (i >= 0)
                  {
                    // escaped characater
                    String_appendChar(value,STRING_ESCAPE_CHARACTERS[i]);
                  }
                  else
                  {
                    // other escaped character
                    String_appendChar(value,s[index]);
                  }
                }
              }
              else
              {
                String_appendChar(value,s[index]);
              }
              index++;
            }

            // skip quote-char
            if (index < length)
            {
              index++;
            }
          }
          while (s[index] != quoteChar);
        }
        else
        {
          String_appendChar(value,s[index]);
          index++;
        }
      }
    }

    // store value
    StringMap_putString(stringMap,name->data,value);
  }

  if (nextIndex != NULL)
  {
    (*nextIndex) = index;
  }

  // free resources
  String_delete(value);
  String_delete(name);

  return TRUE;
}

void* const *StringMap_valueArray(const StringMap stringMap)
{
  uint count;
  void **valueArray;
  uint n,i;

  assert(stringMap != NULL);

  count = StringMap_count(stringMap);

  valueArray = (void**)malloc(count*sizeof(char*));
  if (valueArray != NULL)
  {
    n = 0;
    for (i = 0; i < stringMap->size; i++)
    {
      if (stringMap->stringMapEntries[i].name != NULL)
      {
        assert(n < count);
        valueArray[n] = stringMap->stringMapEntries[i].value.p; n++;
      }
    }
  }

  return valueArray;
}

#ifndef NDEBUG
void StringMap_debugDumpInfo(FILE *handle, const StringMap stringMap)
{
  uint i;

  assert(stringMap != NULL);

  for (i = 0; i < stringMap->size; i++)
  {
    if (stringMap->stringMapEntries[i].name != NULL)
    {
      fprintf(handle,"DEBUG %u: %s = %lx\n",i,stringMap->stringMapEntries[i].name,(unsigned long)stringMap->stringMapEntries[i].value.p);
    }
  }
}

void StringMap_debugPrintInfo(const StringMap stringMap)
{
  StringMap_debugDumpInfo(stderr,stringMap);
}
#endif /* not NDEBUG */

#ifdef __cplusplus
  }
#endif

/* end of file */
