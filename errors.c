
#define __ERROR_IMPLEMENTATION__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>

#include "common/global.h"

#include "errors.h"

// use NONE to avoid warning in strn*-functions which do not accept NULL (this case must be checked before calling strn*
static const char *NONE = NULL;

typedef struct
{
  int  id;
  char text[2048];
  #ifndef NDEBUG
    const char   *fileName;
    unsigned int lineNb;
  #endif /* not NDEBUG */
} ErrorData;

static ErrorData errorData[63];   // last error data
static uint      errorDataCount = 0;                       // last error data count (=max. when all data entries are used; recycle oldest entry if required)
static uint      errorDataId    = 0;                       // total number of error data

#ifndef NDEBUG
int _Error_dataToIndex(const char *fileName, ulong lineNb, const char *format, ...)
#else
int _Error_dataToIndex(const char *format, ...)
#endif
{
  va_list arguments;
  char    text[2048];
  int     index;
  int     minId;
  uint    z,i;

  if (format != NULL)
  {
    // format error text
    va_start(arguments,format);
    vsnprintf(text,sizeof(text),format,arguments);
    va_end(arguments);
  }
  else
  {
    stringClear(text);
  }

  // get new error data id
  errorDataId++;

  // get error data index
  index = -1;
  z = 0;
  while ((z < errorDataCount) && (index == -1))
  {
    if (stringEquals(errorData[z].text,text))
    {
      index = z;
    }
    z++;
  }
  if (index == -1)
  {
    if (errorDataCount < 63)
    {
      // use next entry
      index = errorDataCount;
      errorDataCount++;
    }
    else
    {
      // recycle oldest entry (entry with smallest id)
      index = 0;
      minId = INT_MAX;
      for (z = 0; z < 63; z++)
      {
        if (errorData[z].id < minId)
        {
          index = z;
          minId = errorData[z].id;
        }
      }
    }
  }

  // init error data
  errorData[index].id = errorDataId;
  z = 0;
  i = 0;
  while ((z < strlen(text)) && (i < 2048-1))
  {
    if (!iscntrl(text[z])) { errorData[index].text[i] = text[z]; i++; }
    z++;
  }
  errorData[index].text[i] = '\0';
  #ifndef NDEBUG
    errorData[index].fileName = fileName;
    errorData[index].lineNb   = lineNb;
  #endif /* not NDEBUG */

  return index+1;
}

#define ERROR_GET_CODE(error)        ((((intptr_t)(error)) & 0x000003FF) >> 0)
#define ERROR_GET_CODE_TEXT(error)   Error_getCodeText(error)
#define ERROR_GET_DATA_INDEX(error)  ((((intptr_t)(error)) & 0x0000FC00) >> 10)
#ifndef NDEBUG
#define ERROR_GET_FILENAME(error)    ((ERROR_GET_DATA_INDEX(error) > 0) ? errorData[ERROR_GET_DATA_INDEX(error)-1].fileName : NONE)
#define ERROR_GET_LINENB(error)      ((ERROR_GET_DATA_INDEX(error) > 0) ? errorData[ERROR_GET_DATA_INDEX(error)-1].lineNb : 0)
#define ERROR_GET_LINENB_TEXT(error) Error_getLineNbText(error)
#else
#define ERROR_GET_FILENAME(error)    NONE
#define ERROR_GET_LINENB(error)      0
#define ERROR_GET_LINENB_TEXT(error) NONE
#endif
#define ERROR_GET_DATA(error)        ((ERROR_GET_DATA_INDEX(error) > 0) ? errorData[ERROR_GET_DATA_INDEX(error)-1].text : NONE)
#define ERROR_GET_ERRNO(error)       ((int)(((intptr_t)(error)) & 0xFFFF0000) >> 16)
#define ERROR_GET_ERRNO_TEXT(error)  Error_getErrnoText(error)

#define ERROR_CODE        ERROR_GET_CODE(error)
#define ERROR_FILENAME    ERROR_GET_FILENAME(error)
#define ERROR_LINENB      ERROR_GET_LINENB(error)
#define ERROR_LINENB_TEXT ERROR_GET_LINENB_TEXT(error)
#define ERROR_DATA        ERROR_GET_DATA(error)
#define ERROR_ERRNO       ERROR_GET_ERRNO(error)
#define ERROR_ERRNO_TEXT  ERROR_GET_ERRNO_TEXT(error)

unsigned int Error_getCode(Errors error)
{
  return ERROR_GET_CODE(error);
}

const char *Error_getCodeText(Errors error)
{
  static char codeText[2+3+1];

  stringFormat(codeText,sizeof(codeText),"0x%03x",ERROR_GET_CODE(error));

  return codeText;
}

const char *Error_getData(Errors error)
{
  return ERROR_GET_DATA(error);
}

const char *Error_getFileName(Errors error)
{
  #ifndef NDEBUG
    return ERROR_GET_FILENAME(error);
  #else
    UNUSED_VARIABLE(error);

    return NONE;
  #endif
}

const char *Error_getLineNbText(Errors error)
{
  #ifndef NDEBUG
    static char lineNbText[16+1];

    stringFormat(lineNbText,sizeof(lineNbText),"%d",ERROR_GET_LINENB(error));

    return lineNbText;
  #else
    UNUSED_VARIABLE(error);

    return NULL;
  #endif
}

const char *Error_getLocationText(Errors error)
{
  #ifndef NDEBUG
    static char locationText[PATH_MAX+2+16+1];

    stringFormat(locationText,sizeof(locationText),"%s, %d",ERROR_GET_FILENAME(error),ERROR_GET_LINENB(error));

    return locationText;
  #else
    UNUSED_VARIABLE(error);

    return NULL;
  #endif
}

int Error_getErrno(Errors error)
{
  return ERROR_GET_ERRNO(error);
}

const char *Error_getErrnoText(Errors error)
{
  static char errnoText[16+1];

  stringFormat(errnoText,sizeof(errnoText),"%d",ERROR_GET_ERRNO(error));

  return errnoText;
}

const char *Error_getText(Errors error)
{
  static char errorText[2048];

  stringClear(errorText);
  switch (ERROR_GET_CODE(error))
  {
    case ERROR_CODE_NONE: stringSet(errorText,sizeof(errorText),"none"); break;
    case ERROR_CODE_INSUFFICIENT_MEMORY:
      stringSet(errorText,sizeof(errorText),"insufficient memory");
      break;
    case ERROR_CODE_INIT:
      stringSet(errorText,sizeof(errorText),"init");
      break;
    case ERROR_CODE_INVALID_ARGUMENT:
      stringSet(errorText,sizeof(errorText),"invalid argument");
      break;
    case ERROR_CODE_CONFIG:
      stringSet(errorText,sizeof(errorText),"config error");
      break;
    case ERROR_CODE_ABORTED:
      stringSet(errorText,sizeof(errorText),"aborted");
      break;
    case ERROR_CODE_FUNCTION_NOT_SUPPORTED:
      stringSet(errorText,sizeof(errorText),"function not supported");
      break;
    case ERROR_CODE_STILL_NOT_IMPLEMENTED:
      stringSet(errorText,sizeof(errorText),"function still not implemented");
      break;
    case ERROR_CODE_TESTCODE:
      stringSet(errorText,sizeof(errorText),"test code");
      break;
    case ERROR_CODE_INVALID_PATTERN:
      stringSet(errorText,sizeof(errorText),"init pattern matching");
      break;
    case ERROR_CODE_INIT_TLS:
      stringSet(errorText,sizeof(errorText),"init TLS (SSL)");
      break;
    case ERROR_CODE_NO_TLS_CA:
      stringSet(errorText,sizeof(errorText),"no TLS (SSL) certificate authority file 'bar-ca.pem'");
      break;
    case ERROR_CODE_NO_TLS_CERTIFICATE:
      stringSet(errorText,sizeof(errorText),"no TLS (SSL) certificate file 'bar-server-cert.pem'");
      break;
    case ERROR_CODE_NO_TLS_KEY:
      stringSet(errorText,sizeof(errorText),"no or unreadable TLS (SSL) key file 'bar-server-key.pem'");
      break;
    case ERROR_CODE_INVALID_TLS_CA:
      stringSet(errorText,sizeof(errorText),"invalid TLS (SSL) certificate authority");
      break;
    case ERROR_CODE_INVALID_TLS_CERTIFICATE:
      stringSet(errorText,sizeof(errorText),"invalid TLS (SSL) certificate");
      break;
    case ERROR_CODE_TLS_HANDSHAKE:
      stringSet(errorText,sizeof(errorText),"TLS (SSL) handshake failure");
      break;
    case ERROR_CODE_INVALID_SSH_SPEFICIER:
      stringSet(errorText,sizeof(errorText),"invalid SSH specifier");
      break;
    case ERROR_CODE_SSH_SESSION_FAIL:
      stringSet(errorText,sizeof(errorText),"initialize ssh session fail");
      break;
    case ERROR_CODE_SSH_AUTHENTIFICATION:
      stringSet(errorText,sizeof(errorText),"invalid ssh password");
      break;
    case ERROR_CODE_FTP_SESSION_FAIL:
      stringSet(errorText,sizeof(errorText),"initialize FTP session fail");
      break;
    case ERROR_CODE_FTP_AUTHENTIFICATION:
      stringSet(errorText,sizeof(errorText),"invalid FTP user/password");
      break;
    case ERROR_CODE_INIT_COMPRESS:
      stringSet(errorText,sizeof(errorText),"init compress");
      break;
    case ERROR_CODE_COMPRESS_ERROR:
      stringSet(errorText,sizeof(errorText),"compress");
      break;
    case ERROR_CODE_DEFLATE_ERROR:
      stringSet(errorText,sizeof(errorText),"deflate");
      break;
    case ERROR_CODE_INFLATE_ERROR:
      stringSet(errorText,sizeof(errorText),"inflate");
      break;
    case ERROR_CODE_COMPRESS_EOF:
      stringSet(errorText,sizeof(errorText),"compress end of file");
      break;
    case ERROR_CODE_UNSUPPORTED_BLOCK_SIZE:
      stringSet(errorText,sizeof(errorText),"unsupported block size");
      break;
    case ERROR_CODE_INIT_CRYPT:
      stringSet(errorText,sizeof(errorText),"init crypt");
      break;
    case ERROR_CODE_NO_CRYPT_PASSWORD:
      stringSet(errorText,sizeof(errorText),"no password given for cipher");
      break;
    case ERROR_CODE_INVALID_PASSWORD:
      stringSet(errorText,sizeof(errorText),"invalid password");
      break;
    case ERROR_CODE_INIT_CIPHER:
      stringSet(errorText,sizeof(errorText),"init cipher");
      break;
    case ERROR_CODE_ENCRYPT_FAIL:
      stringSet(errorText,sizeof(errorText),"encrypt");
      break;
    case ERROR_CODE_DECRYPT_FAIL:
      stringSet(errorText,sizeof(errorText),"decrypt");
      break;
    case ERROR_CODE_CREATE_FILE:
    case ERROR_CODE_OPEN_FILE:
    case ERROR_CODE_OPEN_DIRECTORY:
    case ERROR_CODE_IO_ERROR:
      {
        strncpy(errorText,strerror(ERROR_CODE),sizeof(errorText)-1); errorText[sizeof(errorText)-1] = '\0';
      }
      break;
    case ERROR_CODE_PARSE_DEVICE_LIST:
      stringSet(errorText,sizeof(errorText),"error parsing device list");
      break;
    case ERROR_CODE_FILE_EXITS:
      stringSet(errorText,sizeof(errorText),"file already exists");
      break;
    case ERROR_CODE_FILE_NOT_FOUND:
      stringSet(errorText,sizeof(errorText),"file not found");
      break;
    case ERROR_CODE_END_OF_ARCHIVE:
      stringSet(errorText,sizeof(errorText),"end of archive");
      break;
    case ERROR_CODE_NO_FILE_ENTRY:
      stringSet(errorText,sizeof(errorText),"no file entry");
      break;
    case ERROR_CODE_NO_FILE_DATA:
      stringSet(errorText,sizeof(errorText),"no data entry");
      break;
    case ERROR_CODE_NO_DIRECTORY_ENTRY:
      stringSet(errorText,sizeof(errorText),"no directory entry");
      break;
    case ERROR_CODE_NO_LINK_ENTRY:
      stringSet(errorText,sizeof(errorText),"no link entry");
      break;
    case ERROR_CODE_NO_SPECIAL_ENTRY:
      stringSet(errorText,sizeof(errorText),"no special entry");
      break;
    case ERROR_CODE_END_OF_DATA:
      stringSet(errorText,sizeof(errorText),"end of data");
      break;
    case ERROR_CODE_CRC_ERROR:
      stringSet(errorText,sizeof(errorText),"CRC error");
      break;
    case ERROR_CODE_FILE_INCOMPLETE:
      stringSet(errorText,sizeof(errorText),"file is incomplete");
      break;
    case ERROR_CODE_WRONG_FILE_TYPE:
      stringSet(errorText,sizeof(errorText),"wrong file type");
      break;
    case ERROR_CODE_FILES_DIFFER:
      stringSet(errorText,sizeof(errorText),"files differ");
      break;
    case ERROR_CODE_CORRUPT_DATA:
      stringSet(errorText,sizeof(errorText),"corrupt data or invalid password");
      break;
    case ERROR_CODE_NOT_AN_INCREMENTAL_FILE:
      stringSet(errorText,sizeof(errorText),"invalid incremental file");
      break;
    case ERROR_CODE_WRONG_INCREMENTAL_FILE_VERSION:
      stringSet(errorText,sizeof(errorText),"wrong incremental file version");
      break;
    case ERROR_CODE_CORRUPT_INCREMENTAL_FILE:
      stringSet(errorText,sizeof(errorText),"corrupt incremental file");
      break;
    case ERROR_CODE_HOST_NOT_FOUND:
      stringSet(errorText,sizeof(errorText),"host not found");
      break;
    case ERROR_CODE_CONNECT_FAIL:
      {
        strncpy(errorText,strerror(ERROR_CODE),sizeof(errorText)-1); errorText[sizeof(errorText)-1] = '\0';
      }
      break;
    case ERROR_CODE_NO_LOGIN_NAME:
      stringSet(errorText,sizeof(errorText),"no login name given");
      break;
    case ERROR_CODE_NO_PASSWORD:
      stringSet(errorText,sizeof(errorText),"no password given");
      break;
    case ERROR_CODE_NETWORK_SEND:
      stringSet(errorText,sizeof(errorText),"sending data fail");
      break;
    case ERROR_CODE_NETWORK_RECEIVE:
      stringSet(errorText,sizeof(errorText),"receiving data fail");
      break;
    case ERROR_CODE_NETWORK_EXECUTE_FAIL:
      stringSet(errorText,sizeof(errorText),"execute command fail");
      break;
    case ERROR_CODE_INVALID_DEVICE_SPECIFIER:
      stringSet(errorText,sizeof(errorText),"invalid device specifier");
      break;
    case ERROR_CODE_LOAD_VOLUME_FAIL:
      stringSet(errorText,sizeof(errorText),"load volume fail");
      break;
    case ERROR_CODE_FORK_FAIL:
      stringSet(errorText,sizeof(errorText),"fork for execute external program fail");
      break;
    case ERROR_CODE_EXEC_FAIL:
      stringSet(errorText,sizeof(errorText),"execute external program fail");
      break;

  }
  if (stringIsEmpty(errorText)) stringSet(errorText,sizeof(errorText),"unknown");
  #ifndef NDEBUG
    if (ERROR_FILENAME != NULL)
    {
      stringAppend(errorText,sizeof(errorText)," at ");
      stringAppend(errorText,sizeof(errorText),ERROR_FILENAME);
      stringAppend(errorText,sizeof(errorText),", ");
      stringAppend(errorText,sizeof(errorText),ERROR_LINENB_TEXT);
    }
  #endif /* not NDEBUG */

  return errorText;
}
