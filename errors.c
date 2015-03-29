
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>

#include "global.h"
#include "errors.h"

// use NONE to avoid warning in strn*-functions which do not accept NULL (this case must be checked before calling strn*
static const char *NONE = NULL;

typedef struct
{
  int  id;
  char text[512];
} ErrorText;

static ErrorText errorTexts[63];
static int       errorTextCount = 0;
static int       errorTextId    = 0;

int _Error_textToIndex(const char *format, ...)
{
  va_list arguments;
  char    text[512];
  int     index;
  int     minId;
  uint    z,i;

  if (format != NULL)
  {
    va_start(arguments,format);
    vsnprintf(text,sizeof(text),format,arguments);
    va_end(arguments);

    errorTextId++;
    if (errorTextCount < 63)
    {
      index = errorTextCount;
      errorTextCount++;
    }
    else
    {
      index = 0;
      minId = INT_MAX;
      for (z = 0; z < 63; z++)
      {
        if (errorTexts[z].id < minId)
        {
          index = z;
          minId = errorTexts[z].id;
        }
      }
    }
    z = 0;
    i = 0;
    while ((z < strlen(text)) && (i < 512-1))
    {
      if (!iscntrl(text[z])) { errorTexts[index].text[i] = text[z]; i++; }
      z++;
    }
    errorTexts[index].text[i] = '\0';
    errorTexts[errorTextCount].id = errorTextId;
    return index+1;
  }
  else
  {
    return 0;
  }
}


#define ERROR_GET_CODE(error)       (((error) & 0x000003FF) >> 0)
#define ERROR_GET_CODE_TEXT(error)  Error_getCodeText(error)
#define ERROR_GET_TEXTINDEX(error)  (((error) & 0x0000FC00) >> 10)
#define ERROR_GET_TEXT(error)       ((ERROR_GET_TEXTINDEX(error) > 0) ? errorTexts[ERROR_GET_TEXTINDEX(error)-1].text : NONE)
#define ERROR_GET_ERRNO(error)      ((int)((error) & 0xFFFF0000) >> 16)
#define ERROR_GET_ERRNO_TEXT(error) Error_getErrnoText(error)

#define ERROR_CODE       ERROR_GET_CODE(error)
#define ERROR_TEXT       ERROR_GET_TEXT(error)
#define ERROR_ERRNO      ERROR_GET_ERRNO(error)
#define ERROR_ERRNO_TEXT ERROR_GET_ERRNO_TEXT(error)

unsigned int Error_getCode(Errors error)
{
  return ERROR_GET_CODE(error);
}

const char *Error_getCodeText(Errors error)
{
  static char codeText[512];

  snprintf(codeText,sizeof(codeText)-1,"0x%03x",ERROR_GET_CODE(error));
  codeText[sizeof(codeText)-1] = '\0';

  return codeText;
}

const char *Error_getErrnoText(Errors error)
{
  static char errnoText[512];

  snprintf(errnoText,sizeof(errnoText)-1,"%d",ERROR_GET_ERRNO(error));
  errnoText[sizeof(errnoText)-1] = '\0';

  return errnoText;
}

const char *Error_getText(Errors error)
{
  static char errorText[512];

  strcpy(errorText,"unknown");
  switch (ERROR_GET_CODE(error))
  {
    case ERROR_NONE: return "none";
    case ERROR_INSUFFICIENT_MEMORY: return "insufficient memory";
    case ERROR_INIT: return "init";
    case ERROR_INVALID_ARGUMENT: return "invalid argument";
    case ERROR_CONFIG: return "config error";
    case ERROR_ABORTED: return "aborted";
    case ERROR_FUNCTION_NOT_SUPPORTED: return "function not supported";
    case ERROR_STILL_NOT_IMPLEMENTED: return "function still not implemented";
    case ERROR_TESTCODE: return "test code";
    case ERROR_INVALID_PATTERN: return "init pattern matching";
    case ERROR_INIT_TLS: return "init TLS (SSL)";
    case ERROR_NO_TLS_CA: return "no TLS (SSL) certificate authority file 'bar-ca.pem'";
    case ERROR_NO_TLS_CERTIFICATE: return "no TLS (SSL) certificate file 'bar-server-cert.pem'";
    case ERROR_NO_TLS_KEY: return "no or unreadable TLS (SSL) key file 'bar-server-key.pem'";
    case ERROR_INVALID_TLS_CA: return "invalid TLS (SSL) certificate authority";
    case ERROR_INVALID_TLS_CERTIFICATE: return "invalid TLS (SSL) certificate";
    case ERROR_TLS_HANDSHAKE: return "TLS (SSL) handshake failure";
    case ERROR_INVALID_SSH_SPEFICIER: return "invalid SSH specifier";
    case ERROR_SSH_SESSION_FAIL: return "initialize ssh session fail";
    case ERROR_SSH_AUTHENTIFICATION: return "invalid ssh password";
    case ERROR_FTP_SESSION_FAIL: return "initialize FTP session fail";
    case ERROR_FTP_AUTHENTIFICATION: return "invalid FTP user/password";
    case ERROR_INIT_COMPRESS: return "init compress";
    case ERROR_COMPRESS_ERROR: return "compress";
    case ERROR_DEFLATE_ERROR: return "deflate";
    case ERROR_INFLATE_ERROR: return "inflate";
    case ERROR_COMPRESS_EOF: return "compress end of file";
    case ERROR_UNSUPPORTED_BLOCK_SIZE: return "unsupported block size";
    case ERROR_INIT_CRYPT: return "init crypt";
    case ERROR_NO_CRYPT_PASSWORD: return "no password given for cipher";
    case ERROR_INVALID_PASSWORD: return "invalid password";
    case ERROR_INIT_CIPHER: return "init cipher";
    case ERROR_ENCRYPT_FAIL: return "encrypt";
    case ERROR_DECRYPT_FAIL: return "decrypt";
    case ERROR_CREATE_FILE:
    case ERROR_OPEN_FILE:
    case ERROR_OPEN_DIRECTORY:
    case ERROR_IO_ERROR:
      {
        strncpy(errorText,strerror(ERROR_CODE),sizeof(errorText)-1); errorText[sizeof(errorText)-1] = '\0';
      }
      break;
    case ERROR_PARSE_DEVICE_LIST: return "error parsing device list";
    case ERROR_FILE_EXITS: return "file already exists";
    case ERROR_FILE_NOT_FOUND: return "file not found";
    case ERROR_END_OF_ARCHIVE: return "end of archive";
    case ERROR_NO_FILE_ENTRY: return "no file entry";
    case ERROR_NO_FILE_DATA: return "no data entry";
    case ERROR_NO_DIRECTORY_ENTRY: return "no directory entry";
    case ERROR_NO_LINK_ENTRY: return "no link entry";
    case ERROR_NO_SPECIAL_ENTRY: return "no special entry";
    case ERROR_END_OF_DATA: return "end of data";
    case ERROR_CRC_ERROR: return "CRC error";
    case ERROR_FILE_INCOMPLETE: return "file is incomplete";
    case ERROR_WRONG_FILE_TYPE: return "wrong file type";
    case ERROR_FILES_DIFFER: return "files differ";
    case ERROR_CORRUPT_DATA: return "corrupt data or invalid password";
    case ERROR_NOT_AN_INCREMENTAL_FILE: return "invalid incremental file";
    case ERROR_WRONG_INCREMENTAL_FILE_VERSION: return "wrong incremental file version";
    case ERROR_CORRUPT_INCREMENTAL_FILE: return "corrupt incremental file";
    case ERROR_HOST_NOT_FOUND: return "host not found";
    case ERROR_CONNECT_FAIL:
      {
        strncpy(errorText,strerror(ERROR_CODE),sizeof(errorText)-1); errorText[sizeof(errorText)-1] = '\0';
      }
      break;
    case ERROR_NO_LOGIN_NAME: return "no login name given";
    case ERROR_NO_PASSWORD: return "no password given";
    case ERROR_NETWORK_SEND: return "sending data fail";
    case ERROR_NETWORK_RECEIVE: return "receiving data fail";
    case ERROR_NETWORK_EXECUTE_FAIL: return "execute command fail";
    case ERROR_INVALID_DEVICE_SPECIFIER: return "invalid device specifier";
    case ERROR_LOAD_VOLUME_FAIL: return "load volume fail";
    case ERROR_FORK_FAIL: return "fork for execute external program fail";
    case ERROR_EXEC_FAIL: return "execute external program fail";

  }

  return errorText;
}
