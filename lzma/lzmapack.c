/***********************************************************************\
*
* $Source$
* $Revision$
* $Author$
* Contents: lzma pack demo
* Systems: Linux
*
\***********************************************************************/

/* Note: compile with

   gcc -o lzmapack lzmapack.c -llzma
*/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lzma.h"

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

int main(int argc, const char *argv[])
{
  lzma_stream  stream = LZMA_STREAM_INIT;
  unsigned int in,out;
  int          n;
  char         ch;
  char         buffer[1024*1024];
  lzma_ret     result;

  result = lzma_easy_encoder(&stream,9,LZMA_CHECK_NONE);
  if (result != LZMA_OK)
  {
    fprintf(stderr,"Can not initialize easy encoder (error: %d)!\n",result);
    exit(1);
  }

  in  = 0;
  out = 0;
  while ((n = getc(stdin)) != EOF)
  {
    ch = (char)n; in++;
    stream.next_in   = (unsigned char*)&ch;
    stream.avail_in  = 1;
    stream.next_out  = (unsigned char*)buffer;
    stream.avail_out = sizeof(buffer);
    result = lzma_code(&stream,LZMA_RUN);
    if (result != LZMA_OK)
    {
      fprintf(stderr,"Can not encode data (error: %d)!\n",result);
      exit(1);
    }
    fwrite(buffer,1,sizeof(buffer)-stream.avail_out,stdout); out += ((sizeof(buffer)-stream.avail_out));
  }
  do
  {
    stream.next_out  = (unsigned char*)buffer;
    stream.avail_out = sizeof(buffer);
    result = lzma_code(&stream,LZMA_FINISH);
    if ((result != LZMA_OK) && (result != LZMA_STREAM_END))
    {
      fprintf(stderr,"Can not encode data (error: %d)!\n",result);
      exit(1);
    }
    fwrite(buffer,1,sizeof(buffer)-stream.avail_out,stdout); out += ((sizeof(buffer)-stream.avail_out));
  }
  while (result == LZMA_OK);

  lzma_end(&stream);

  fprintf(stderr,"pack: in=%u out=%u\n",in,out);

  return 0;
}

#ifdef __cplusplus
  }
#endif

/* end of file */
