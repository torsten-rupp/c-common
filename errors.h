
#ifndef __ERRORS__
#define __ERRORS__

/***********************************************************************
* Name   : ERROR_
* Purpose: create error
* Input  : code  - error code; see ERROR_...
*          errno - errno or 0
* Output : -
* Return : error
* Notes  : -
***********************************************************************/

#define ERROR_(code,errno) Error_((ERROR_ ## code),errno)

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

#define ERRORX_(code,errno,format,...) Errorx_((ERROR_ ## code),errno,format, ## __VA_ARGS__)

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
  #define ERRORF_(error,format,...)      ((Errors)(  ((error) & (0x000003FF|0xFFFF0000)) \
                                                   | ((_Error_dataToIndex(__FILE__,__LINE__,format, ## __VA_ARGS__) << 10) & 0x0000FC00) \
                                                  ) \
                                         )
#else
  #define ERRORF_(error,format,...)      ((Errors)(  ((error) & (0x000003FF|0xFFFF0000)) \
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
  #define Error_(code,errno)             ((Errors)(  (((errno) << 16) & 0xFFFF0000) \
                                                   | ((_Error_dataToIndex(__FILE__,__LINE__,NULL) << 10) & 0x0000FC00) \
                                                   | (((code) << 0) & 0x000003FF) \
                                                  ) \
                                         )
#else
  #define Error_(code,errno)             ((Errors)(  (((errno) << 16) & 0xFFFF0000) \
                                                   | ((_Error_dataToIndex(NULL) << 10) & 0x0000FC00) \
                                                   | (((code) << 0) & 0x000003FF) \
                                                  ) \
                                         )
#endif

/***********************************************************************
* Name   : Errorx_
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
  #define Errorx_(code,errno,format,...) ((Errors)(  (((errno) << 16) & 0xFFFF0000) \
                                                   | ((_Error_dataToIndex(__FILE__,__LINE__,format, ## __VA_ARGS__) << 10) & 0x0000FC00) \
                                                   | (((code) << 0) & 0x000003FF) \
                                                  ) \
                                         )
#else
  #define Errorx_(code,errno,format,...) ((Errors)(  (((errno) << 16) & 0xFFFF0000) \
                                                   | ((_Error_dataToIndex(format, ## __VA_ARGS__) << 10) & 0x0000FC00) \
                                                   | (((code) << 0) & 0x000003FF) \
                                                  ) \
                                         )
#endif

typedef enum
{
  ERROR_NONE = 0,
  ERROR_INSUFFICIENT_MEMORY = 1,
  ERROR_INIT = 2,
  ERROR_INVALID_ARGUMENT = 3,
  ERROR_CONFIG = 4,
  ERROR_ABORTED = 5,
  ERROR_FUNCTION_NOT_SUPPORTED = 6,
  ERROR_STILL_NOT_IMPLEMENTED = 7,
  ERROR_TESTCODE = 8,
  ERROR_INVALID_PATTERN = 9,
  ERROR_INIT_TLS = 10,
  ERROR_NO_TLS_CA = 11,
  ERROR_NO_TLS_CERTIFICATE = 12,
  ERROR_NO_TLS_KEY = 13,
  ERROR_INVALID_TLS_CA = 14,
  ERROR_INVALID_TLS_CERTIFICATE = 15,
  ERROR_TLS_HANDSHAKE = 16,
  ERROR_INVALID_SSH_SPEFICIER = 17,
  ERROR_SSH_SESSION_FAIL = 18,
  ERROR_SSH_AUTHENTIFICATION = 19,
  ERROR_FTP_SESSION_FAIL = 20,
  ERROR_FTP_AUTHENTIFICATION = 21,
  ERROR_INIT_COMPRESS = 22,
  ERROR_COMPRESS_ERROR = 23,
  ERROR_DEFLATE_ERROR = 24,
  ERROR_INFLATE_ERROR = 25,
  ERROR_COMPRESS_EOF = 26,
  ERROR_UNSUPPORTED_BLOCK_SIZE = 27,
  ERROR_INIT_CRYPT = 28,
  ERROR_NO_CRYPT_PASSWORD = 29,
  ERROR_INVALID_PASSWORD = 30,
  ERROR_INIT_CIPHER = 31,
  ERROR_ENCRYPT_FAIL = 32,
  ERROR_DECRYPT_FAIL = 33,
  ERROR_CREATE_FILE = 34,
  ERROR_OPEN_FILE = 35,
  ERROR_OPEN_DIRECTORY = 36,
  ERROR_IO_ERROR = 37,
  ERROR_PARSE_DEVICE_LIST = 38,
  ERROR_FILE_EXITS = 39,
  ERROR_FILE_NOT_FOUND = 40,
  ERROR_END_OF_ARCHIVE = 41,
  ERROR_NO_FILE_ENTRY = 42,
  ERROR_NO_FILE_DATA = 43,
  ERROR_NO_DIRECTORY_ENTRY = 44,
  ERROR_NO_LINK_ENTRY = 45,
  ERROR_NO_SPECIAL_ENTRY = 46,
  ERROR_END_OF_DATA = 47,
  ERROR_CRC_ERROR = 48,
  ERROR_FILE_INCOMPLETE = 49,
  ERROR_WRONG_FILE_TYPE = 50,
  ERROR_FILES_DIFFER = 51,
  ERROR_CORRUPT_DATA = 52,
  ERROR_NOT_AN_INCREMENTAL_FILE = 53,
  ERROR_WRONG_INCREMENTAL_FILE_VERSION = 54,
  ERROR_CORRUPT_INCREMENTAL_FILE = 55,
  ERROR_HOST_NOT_FOUND = 56,
  ERROR_CONNECT_FAIL = 57,
  ERROR_NO_LOGIN_NAME = 58,
  ERROR_NO_PASSWORD = 59,
  ERROR_NETWORK_SEND = 60,
  ERROR_NETWORK_RECEIVE = 61,
  ERROR_NETWORK_EXECUTE_FAIL = 62,
  ERROR_INVALID_DEVICE_SPECIFIER = 63,
  ERROR_LOAD_VOLUME_FAIL = 64,
  ERROR_FORK_FAIL = 65,
  ERROR_EXEC_FAIL = 66,

  ERROR_UNKNOWN = 67
} Errors;

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
***********************************************************************/

#ifndef NDEBUG
int _Error_dataToIndex(const char *fileName, ulong lineNb, const char *format, ...);
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
