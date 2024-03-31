/***********************************************************************\
*
* Contents: functions for base64
* Systems: all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "global.h"
#include "strings.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
  extern "C" {
#endif

#if 0
BEGIN {
  s="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  for (z=0;z<256;z++)
  {
    n=index(s,sprintf("%c",z));
    printf("%d,",(n>0)?n-1:0);
    if (((z+1)%8)==0) printf("\n");
  }
}
#endif /* 0 */

/***********************************************************************\
* Name   : base64Encode
* Purpose: encode base64
* Input  : s      - string variable
*          data   - data to encode
*          length - length of data to encode
* Output : -
* Return : encoded strings
* Notes  : -
\***********************************************************************/

LOCAL String base64Encode(String s, const byte *data, uint length)
{
  const char BASE64_ENCODING_TABLE[] =
  {
    'A','B','C','D','E','F','G','H',
    'I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X',
    'Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3',
    '4','5','6','7','8','9','+','/'
  };

  uint z;
  char b0,b1,b2;
  uint i0,i1,i2,i3;

  z = 0;
  while (z < length)
  {
    b0 = ((z+0) < length)?data[z+0]:0;
    b1 = ((z+1) < length)?data[z+1]:0;
    b2 = ((z+2) < length)?data[z+2]:0;

    i0 = (uint)(b0 & 0xFC) >> 2;
    assert(i0 < 64);
    i1 = (uint)((b0 & 0x03) << 4) | (uint)((b1 & 0xF0) >> 4);
    assert(i1 < 64);
    i2 = (uint)((b1 & 0x0F) << 2) | (uint)((b2 & 0xC0) >> 6);
    assert(i2 < 64);
    i3 = (uint)(b2 & 0x3F);
    assert(i3 < 64);

    String_appendChar(s,BASE64_ENCODING_TABLE[i0]);
    String_appendChar(s,BASE64_ENCODING_TABLE[i1]);
    String_appendChar(s,BASE64_ENCODING_TABLE[i2]);
    String_appendChar(s,BASE64_ENCODING_TABLE[i3]);

    z += 3;
  }

  return s;
}

/***********************************************************************\
* Name   : base64Decode
* Purpose: decode base64
* Input  : data      - data variable
*          maxLength - max. length of data
*          s         - base64 string
* Output : -
* Return : length of decoded data or -1 on error
* Notes  : -
\***********************************************************************/

LOCAL int base64Decode(byte *data, uint maxLength, const String s)
{
  #define VALID_BASE64_CHAR(ch) (   (((ch) >= 'A') && ((ch) <= 'Z')) \
                                 || (((ch) >= 'a') && ((ch) <= 'z')) \
                                 || (((ch) >= '0') && ((ch) <= '9')) \
                                 || ((ch) == '+') \
                                 || ((ch) == '/') \
                                 || ((ch) == '=') \
                                )

  const byte BASE64_DECODING_TABLE[] =
  {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,62,0,0,0,63,
    52,53,54,55,56,57,58,59,
    60,61,0,0,0,0,0,0,
    0,0,1,2,3,4,5,6,
    7,8,9,10,11,12,13,14,
    15,16,17,18,19,20,21,22,
    23,24,25,0,0,0,0,0,
    0,26,27,28,29,30,31,32,
    33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48,
    49,50,51,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
  };

  uint length;
  uint z;
  char x0,x1,x2,x3;
  uint i0,i1,i2,i3;
  char b0,b1,b2;

  length = 0;
  z = 0;
  while (z < String_length(s))
  {
    x0 = String_index(s,z+0); if (!VALID_BASE64_CHAR(x0)) return -1;
    x1 = String_index(s,z+1); if (!VALID_BASE64_CHAR(x1)) return -1;
    x2 = String_index(s,z+2); if (!VALID_BASE64_CHAR(x2)) return -1;
    x3 = String_index(s,z+3); if (!VALID_BASE64_CHAR(x3)) return -1;

    i0 = ((z+0) < String_length(s))?BASE64_DECODING_TABLE[(byte)x0]:0;
    i1 = ((z+1) < String_length(s))?BASE64_DECODING_TABLE[(byte)x1]:0;
    i2 = ((z+2) < String_length(s))?BASE64_DECODING_TABLE[(byte)x2]:0;
    i3 = ((z+3) < String_length(s))?BASE64_DECODING_TABLE[(byte)x3]:0;

    b0 = (char)((i0 << 2) | ((i1 & 0x30) >> 4));
    b1 = (char)(((i1 & 0x0F) << 4) | ((i2 & 0x3C) >> 2));
    b2 = (char)(((i2 & 0x03) << 6) | i3);

    if (length < maxLength) { data[length] = b0; length++; }
    if (length < maxLength) { data[length] = b1; length++; }
    if (length < maxLength) { data[length] = b2; length++; }

    z += 4;
  }

  return length;

  #undef VALID_BASE64_CHAR
}

#ifdef __cplusplus
  }
#endif

/* end of file */
