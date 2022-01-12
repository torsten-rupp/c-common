
#ifndef __ERRORS__
#define __ERRORS__

#include <stdint.h>

/***********************************************************************
* Name   : ERROR_
* Purpose: create error
* Input  : code  - error code; see ERROR_...
*          errno - errno or 0
* Output : -
* Return : error
* Notes  : -
***********************************************************************/

#define ERROR_(code,errno) Error_((ERROR_CODE_ ## code),errno)

/***********************************************************************
* Name   : ERRORX_
* Purpose: create extended error
* Input  : code   - error code; see ERROR_...
*          errno  - errno or 0
*          format - format string (like printf)
*          ...    - optional arguments for format string
* Output : -
* Return : error
* Notes  : -
***********************************************************************/

#ifndef NDEBUG
  #define ERRORX_(code,errno,format,...) Errorx_(__FILE__,__LINE__,(ERROR_CODE_ ## code),errno,format, ## __VA_ARGS__)
#else
  #define ERRORX_(code,errno,format,...) Errorx_((ERROR_CODE_ ## code),errno,format, ## __VA_ARGS__)
#endif

/***********************************************************************
* Name   : ERRORF_
* Purpose: create error from existing error (update text)
* Input  : error  - error
*          format - format string (like printf)
*          ...    - optional arguments for format string
* Output : -
* Return : error
* Notes  : -
***********************************************************************/

#ifndef NDEBUG
  #define ERRORF_(error,format,...)      ((Errors)(intptr_t)(  ((error) & (0x000003FF|0xFFFF0000)) \
                                                             | ((_Error_dataToIndex(__FILE__,__LINE__,format, ## __VA_ARGS__) << 10) & 0x0000FC00) \
                                                            ) \
                                         )
#else
  #define ERRORF_(error,format,...)      ((Errors)(intptr_t)(  ((error) & (0x000003FF|0xFFFF0000)) \
                                                             | ((_Error_dataToIndex(format, ## __VA_ARGS__) << 10) & 0x0000FC00) \
                                                            ) \
                                         )
#endif

/***********************************************************************
* Name   : Error_
* Purpose: create error
* Input  : code  - error code; see ERROR_...
*          errno - errno or 0
* Output : -
* Return : error
* Notes  : -
***********************************************************************/

#ifndef NDEBUG
  #define Error_(code,errno)             ((Errors)(intptr_t)(  (((errno) << 16) & 0xFFFF0000) \
                                                             | ((_Error_dataToIndex(__FILE__,__LINE__,NULL) << 10) & 0x0000FC00) \
                                                             | (((code) << 0) & 0x000003FF) \
                                                            ) \
                                         )
#else
  #define Error_(code,errno)             ((Errors)(intptr_t)(  (((errno) << 16) & 0xFFFF0000) \
                                                             | ((_Error_dataToIndex(NULL) << 10) & 0x0000FC00) \
                                                             | (((code) << 0) & 0x000003FF) \
                                                            ) \
                                         )
#endif

/***********************************************************************
* Name   : Errorx_
* Purpose: create extended error
* Input  : fileName - file name
*          lineNb   - line number
*          code     - error code; see ERROR_...
*          errno    - errno or 0
*          format   - format string (like printf)
*          ...      - optional arguments for format string
* Output : -
* Return : error
* Notes  : -
***********************************************************************/

#ifndef NDEBUG
  #define Errorx_(fileName,lineNb,code,errno,format,...) ((Errors)(intptr_t)(  (((errno) << 16) & 0xFFFF0000) \
                                                                             | ((_Error_dataToIndex(fileName,lineNb,format, ## __VA_ARGS__) << 10) & 0x0000FC00) \
                                                                             | (((code) << 0) & 0x000003FF) \
                                                                            ) \
                                                         )
#else
  #define Errorx_(code,errno,format,...) ((Errors)(intptr_t)(  (((errno) << 16) & 0xFFFF0000) \
                                                             | ((_Error_dataToIndex(format, ## __VA_ARGS__) << 10) & 0x0000FC00) \
                                                             | (((code) << 0) & 0x000003FF) \
                                                            ) \
                                         )
#endif

typedef enum
{
  ERROR_CODE_NONE = 0,
#line 15 "errors.def"
  ERROR_CODE_INSUFFICIENT_MEMORY = 1,
#line 16 "errors.def"
  ERROR_CODE_INIT = 2,
#line 17 "errors.def"
  ERROR_CODE_INVALID_ARGUMENT = 3,
#line 18 "errors.def"
  ERROR_CODE_CONFIG = 4,
#line 19 "errors.def"
  ERROR_CODE_ABORTED = 5,
#line 20 "errors.def"
  ERROR_CODE_FUNCTION_NOT_SUPPORTED = 6,
#line 21 "errors.def"
  ERROR_CODE_STILL_NOT_IMPLEMENTED = 7,
#line 22 "errors.def"
  ERROR_CODE_TESTCODE = 8,
#line 25 "errors.def"
  ERROR_CODE_INVALID_PATTERN = 9,
#line 28 "errors.def"
  ERROR_CODE_INIT_TLS = 10,
#line 29 "errors.def"
  ERROR_CODE_NO_TLS_CA = 11,
#line 30 "errors.def"
  ERROR_CODE_NO_TLS_CERTIFICATE = 12,
#line 31 "errors.def"
  ERROR_CODE_NO_TLS_KEY = 13,
#line 32 "errors.def"
  ERROR_CODE_INVALID_TLS_CA = 14,
#line 33 "errors.def"
  ERROR_CODE_INVALID_TLS_CERTIFICATE = 15,
#line 34 "errors.def"
  ERROR_CODE_TLS_HANDSHAKE = 16,
#line 35 "errors.def"
  ERROR_CODE_INVALID_SSH_SPEFICIER = 17,
#line 36 "errors.def"
  ERROR_CODE_SSH_SESSION_FAIL = 18,
#line 37 "errors.def"
  ERROR_CODE_SSH_AUTHENTIFICATION = 19,
#line 40 "errors.def"
  ERROR_CODE_FTP_SESSION_FAIL = 20,
#line 41 "errors.def"
  ERROR_CODE_FTP_AUTHENTIFICATION = 21,
#line 44 "errors.def"
  ERROR_CODE_INIT_COMPRESS = 22,
#line 45 "errors.def"
  ERROR_CODE_COMPRESS_ERROR = 23,
#line 46 "errors.def"
  ERROR_CODE_DEFLATE_ERROR = 24,
#line 47 "errors.def"
  ERROR_CODE_INFLATE_ERROR = 25,
#line 48 "errors.def"
  ERROR_CODE_COMPRESS_EOF = 26,
#line 51 "errors.def"
  ERROR_CODE_UNSUPPORTED_BLOCK_SIZE = 27,
#line 52 "errors.def"
  ERROR_CODE_INIT_CRYPT = 28,
#line 53 "errors.def"
  ERROR_CODE_NO_CRYPT_PASSWORD = 29,
#line 54 "errors.def"
  ERROR_CODE_INVALID_PASSWORD = 30,
#line 55 "errors.def"
  ERROR_CODE_INIT_CIPHER = 31,
#line 56 "errors.def"
  ERROR_CODE_ENCRYPT_FAIL = 32,
#line 57 "errors.def"
  ERROR_CODE_DECRYPT_FAIL = 33,
#line 60 "errors.def"
  ERROR_CODE_CREATE_FILE = 34,
#line 61 "errors.def"
  ERROR_CODE_OPEN_FILE = 35,
#line 62 "errors.def"
  ERROR_CODE_OPEN_DIRECTORY = 36,
#line 63 "errors.def"
  ERROR_CODE_IO_ERROR = 37,
#line 66 "errors.def"
  ERROR_CODE_PARSE_DEVICE_LIST = 38,
#line 67 "errors.def"
  ERROR_CODE_FILE_EXITS = 39,
#line 68 "errors.def"
  ERROR_CODE_FILE_NOT_FOUND = 40,
#line 71 "errors.def"
  ERROR_CODE_END_OF_ARCHIVE = 41,
#line 72 "errors.def"
  ERROR_CODE_NO_FILE_ENTRY = 42,
#line 73 "errors.def"
  ERROR_CODE_NO_FILE_DATA = 43,
#line 74 "errors.def"
  ERROR_CODE_NO_DIRECTORY_ENTRY = 44,
#line 75 "errors.def"
  ERROR_CODE_NO_LINK_ENTRY = 45,
#line 76 "errors.def"
  ERROR_CODE_NO_SPECIAL_ENTRY = 46,
#line 77 "errors.def"
  ERROR_CODE_END_OF_DATA = 47,
#line 78 "errors.def"
  ERROR_CODE_CRC_ERROR = 48,
#line 79 "errors.def"
  ERROR_CODE_FILE_INCOMPLETE = 49,
#line 80 "errors.def"
  ERROR_CODE_WRONG_FILE_TYPE = 50,
#line 81 "errors.def"
  ERROR_CODE_FILES_DIFFER = 51,
#line 82 "errors.def"
  ERROR_CODE_CORRUPT_DATA = 52,
#line 85 "errors.def"
  ERROR_CODE_NOT_AN_INCREMENTAL_FILE = 53,
#line 86 "errors.def"
  ERROR_CODE_WRONG_INCREMENTAL_FILE_VERSION = 54,
#line 87 "errors.def"
  ERROR_CODE_CORRUPT_INCREMENTAL_FILE = 55,
#line 90 "errors.def"
  ERROR_CODE_HOST_NOT_FOUND = 56,
#line 91 "errors.def"
  ERROR_CODE_CONNECT_FAIL = 57,
#line 94 "errors.def"
  ERROR_CODE_NO_LOGIN_NAME = 58,
#line 95 "errors.def"
  ERROR_CODE_NO_PASSWORD = 59,
#line 96 "errors.def"
  ERROR_CODE_NETWORK_SEND = 60,
#line 97 "errors.def"
  ERROR_CODE_NETWORK_RECEIVE = 61,
#line 98 "errors.def"
  ERROR_CODE_NETWORK_EXECUTE_FAIL = 62,
#line 101 "errors.def"
  ERROR_CODE_INVALID_DEVICE_SPECIFIER = 63,
#line 102 "errors.def"
  ERROR_CODE_LOAD_VOLUME_FAIL = 64,
#line 105 "errors.def"
  ERROR_CODE_FORK_FAIL = 65,
#line 106 "errors.def"
  ERROR_CODE_EXEC_FAIL = 66,

  ERROR_CODE_UNKNOWN = 67
} ErrorCodes;

// special errors type
typedef intptr_t* Errors;

// error macros
#define ERROR_NONE (Errors)(ERROR_CODE_NONE & (0x000003FF|0xFFFF0000))
#define ERROR_INSUFFICIENT_MEMORY Error_(ERROR_CODE_INSUFFICIENT_MEMORY,0)
#define ERROR_INIT Error_(ERROR_CODE_INIT,0)
#define ERROR_INVALID_ARGUMENT Error_(ERROR_CODE_INVALID_ARGUMENT,0)
#define ERROR_CONFIG Error_(ERROR_CODE_CONFIG,0)
#define ERROR_ABORTED Error_(ERROR_CODE_ABORTED,0)
#define ERROR_FUNCTION_NOT_SUPPORTED Error_(ERROR_CODE_FUNCTION_NOT_SUPPORTED,0)
#define ERROR_STILL_NOT_IMPLEMENTED Error_(ERROR_CODE_STILL_NOT_IMPLEMENTED,0)
#define ERROR_TESTCODE Error_(ERROR_CODE_TESTCODE,0)
#define ERROR_INVALID_PATTERN Error_(ERROR_CODE_INVALID_PATTERN,0)
#define ERROR_INIT_TLS Error_(ERROR_CODE_INIT_TLS,0)
#define ERROR_NO_TLS_CA Error_(ERROR_CODE_NO_TLS_CA,0)
#define ERROR_NO_TLS_CERTIFICATE Error_(ERROR_CODE_NO_TLS_CERTIFICATE,0)
#define ERROR_NO_TLS_KEY Error_(ERROR_CODE_NO_TLS_KEY,0)
#define ERROR_INVALID_TLS_CA Error_(ERROR_CODE_INVALID_TLS_CA,0)
#define ERROR_INVALID_TLS_CERTIFICATE Error_(ERROR_CODE_INVALID_TLS_CERTIFICATE,0)
#define ERROR_TLS_HANDSHAKE Error_(ERROR_CODE_TLS_HANDSHAKE,0)
#define ERROR_INVALID_SSH_SPEFICIER Error_(ERROR_CODE_INVALID_SSH_SPEFICIER,0)
#define ERROR_SSH_SESSION_FAIL Error_(ERROR_CODE_SSH_SESSION_FAIL,0)
#define ERROR_SSH_AUTHENTIFICATION Error_(ERROR_CODE_SSH_AUTHENTIFICATION,0)
#define ERROR_FTP_SESSION_FAIL Error_(ERROR_CODE_FTP_SESSION_FAIL,0)
#define ERROR_FTP_AUTHENTIFICATION Error_(ERROR_CODE_FTP_AUTHENTIFICATION,0)
#define ERROR_INIT_COMPRESS Error_(ERROR_CODE_INIT_COMPRESS,0)
#define ERROR_COMPRESS_ERROR Error_(ERROR_CODE_COMPRESS_ERROR,0)
#define ERROR_DEFLATE_ERROR Error_(ERROR_CODE_DEFLATE_ERROR,0)
#define ERROR_INFLATE_ERROR Error_(ERROR_CODE_INFLATE_ERROR,0)
#define ERROR_COMPRESS_EOF Error_(ERROR_CODE_COMPRESS_EOF,0)
#define ERROR_UNSUPPORTED_BLOCK_SIZE Error_(ERROR_CODE_UNSUPPORTED_BLOCK_SIZE,0)
#define ERROR_INIT_CRYPT Error_(ERROR_CODE_INIT_CRYPT,0)
#define ERROR_NO_CRYPT_PASSWORD Error_(ERROR_CODE_NO_CRYPT_PASSWORD,0)
#define ERROR_INVALID_PASSWORD Error_(ERROR_CODE_INVALID_PASSWORD,0)
#define ERROR_INIT_CIPHER Error_(ERROR_CODE_INIT_CIPHER,0)
#define ERROR_ENCRYPT_FAIL Error_(ERROR_CODE_ENCRYPT_FAIL,0)
#define ERROR_DECRYPT_FAIL Error_(ERROR_CODE_DECRYPT_FAIL,0)
#define ERROR_CREATE_FILE Error_(ERROR_CODE_CREATE_FILE,0)
#define ERROR_OPEN_FILE Error_(ERROR_CODE_OPEN_FILE,0)
#define ERROR_OPEN_DIRECTORY Error_(ERROR_CODE_OPEN_DIRECTORY,0)
#define ERROR_IO_ERROR Error_(ERROR_CODE_IO_ERROR,0)
#define ERROR_PARSE_DEVICE_LIST Error_(ERROR_CODE_PARSE_DEVICE_LIST,0)
#define ERROR_FILE_EXITS Error_(ERROR_CODE_FILE_EXITS,0)
#define ERROR_FILE_NOT_FOUND Error_(ERROR_CODE_FILE_NOT_FOUND,0)
#define ERROR_END_OF_ARCHIVE Error_(ERROR_CODE_END_OF_ARCHIVE,0)
#define ERROR_NO_FILE_ENTRY Error_(ERROR_CODE_NO_FILE_ENTRY,0)
#define ERROR_NO_FILE_DATA Error_(ERROR_CODE_NO_FILE_DATA,0)
#define ERROR_NO_DIRECTORY_ENTRY Error_(ERROR_CODE_NO_DIRECTORY_ENTRY,0)
#define ERROR_NO_LINK_ENTRY Error_(ERROR_CODE_NO_LINK_ENTRY,0)
#define ERROR_NO_SPECIAL_ENTRY Error_(ERROR_CODE_NO_SPECIAL_ENTRY,0)
#define ERROR_END_OF_DATA Error_(ERROR_CODE_END_OF_DATA,0)
#define ERROR_CRC_ERROR Error_(ERROR_CODE_CRC_ERROR,0)
#define ERROR_FILE_INCOMPLETE Error_(ERROR_CODE_FILE_INCOMPLETE,0)
#define ERROR_WRONG_FILE_TYPE Error_(ERROR_CODE_WRONG_FILE_TYPE,0)
#define ERROR_FILES_DIFFER Error_(ERROR_CODE_FILES_DIFFER,0)
#define ERROR_CORRUPT_DATA Error_(ERROR_CODE_CORRUPT_DATA,0)
#define ERROR_NOT_AN_INCREMENTAL_FILE Error_(ERROR_CODE_NOT_AN_INCREMENTAL_FILE,0)
#define ERROR_WRONG_INCREMENTAL_FILE_VERSION Error_(ERROR_CODE_WRONG_INCREMENTAL_FILE_VERSION,0)
#define ERROR_CORRUPT_INCREMENTAL_FILE Error_(ERROR_CODE_CORRUPT_INCREMENTAL_FILE,0)
#define ERROR_HOST_NOT_FOUND Error_(ERROR_CODE_HOST_NOT_FOUND,0)
#define ERROR_CONNECT_FAIL Error_(ERROR_CODE_CONNECT_FAIL,0)
#define ERROR_NO_LOGIN_NAME Error_(ERROR_CODE_NO_LOGIN_NAME,0)
#define ERROR_NO_PASSWORD Error_(ERROR_CODE_NO_PASSWORD,0)
#define ERROR_NETWORK_SEND Error_(ERROR_CODE_NETWORK_SEND,0)
#define ERROR_NETWORK_RECEIVE Error_(ERROR_CODE_NETWORK_RECEIVE,0)
#define ERROR_NETWORK_EXECUTE_FAIL Error_(ERROR_CODE_NETWORK_EXECUTE_FAIL,0)
#define ERROR_INVALID_DEVICE_SPECIFIER Error_(ERROR_CODE_INVALID_DEVICE_SPECIFIER,0)
#define ERROR_LOAD_VOLUME_FAIL Error_(ERROR_CODE_LOAD_VOLUME_FAIL,0)
#define ERROR_FORK_FAIL Error_(ERROR_CODE_FORK_FAIL,0)
#define ERROR_EXEC_FAIL Error_(ERROR_CODE_EXEC_FAIL,0)
#define ERROR_UNKNOWN (Errors)(ERROR_CODE_UNKNOWN & (0x000003FF|0xFFFF0000))

#ifdef __cplusplus
  extern "C" {
#endif

/***********************************************************************
* Name   : _Error_dataToIndex
* Purpose: store error data as index
* Input  : fileName - file name
*          lineNb   - line number
*          format   - format string (like printf)
*          ...      - optional arguments for format string
* Output : -
* Return : index
* Notes  : internal usage only!
*          additional format specifiers:
*            %E convert errno to text (with lower case start)
***********************************************************************/

#ifndef NDEBUG
int _Error_dataToIndex(const char *fileName, unsigned long lineNb, const char *format, ...);
#else
int _Error_dataToIndex(const char *format, ...);
#endif

/***********************************************************************
* Name   : Error_getCode
* Purpose: get error code
* Input  : error - error
* Output : -
* Return : error code
* Notes  : -
***********************************************************************/

unsigned int Error_getCode(Errors error);

/***********************************************************************
* Name   : Error_getCodeText
* Purpose: get error code as text (hex)
* Input  : error - error
* Output : -
* Return : text
* Notes  : -
***********************************************************************/

const char *Error_getCodeText(Errors error);

/***********************************************************************
* Name   : Error_getData
* Purpose: get data
* Input  : error - error
* Output : -
* Return : data
* Notes  : -
***********************************************************************/

const char *Error_getData(Errors error);

/***********************************************************************
* Name   : Error_getFileName
* Purpose: get filename
* Input  : error - error
* Output : -
* Return : filename
* Notes  : -
***********************************************************************/

const char *Error_getFileName(Errors error);

/***********************************************************************
* Name   : Error_getLineNbText
* Purpose: get line number text
* Input  : error - error
* Output : -
* Return : line number text
* Notes  : -
***********************************************************************/

const char *Error_getLineNbText(Errors error);

/***********************************************************************
* Name   : Error_getLocationText
* Purpose: get location text (filename+line number)
* Input  : error - error
* Output : -
* Return : location text
* Notes  : -
***********************************************************************/

const char *Error_getLocationText(Errors error);

/***********************************************************************
* Name   : Error_getErrno
* Purpose: get errno
* Input  : error - error
* Output : -
* Return : errno
* Notes  : -
***********************************************************************/

int Error_getErrno(Errors error);

/***********************************************************************
* Name   : Error_getErrnoText
* Purpose: get errno text
* Input  : error - error
* Output : -
* Return : errno text
* Notes  : -
***********************************************************************/

const char *Error_getErrnoText(Errors error);

/***********************************************************************
* Name   : Error_getText
* Purpose: get error text
* Input  : error - error
* Output : -
* Return : error text
* Notes  : -
***********************************************************************/

const char *Error_getText(Errors error);

#ifdef __cplusplus
  }
#endif

#endif /* __ERRORS__ */
