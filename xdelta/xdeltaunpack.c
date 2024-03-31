/***********************************************************************\
*
* Contents: xdelta unpack demo
* Systems: Linux
*
\***********************************************************************/

/* Note: compile with

   gcc xdeltaunpack.c -o xdeltaunpack
*/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "xdelta3.h"
#include "xdelta3.c"

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

static int readSourceBlock(xd3_stream *xd3Stream,
                           xd3_source *xd3Source,
                           xoff_t      blkno
                          )
{
  int result;

  assert(xd3Stream != NULL);
  assert(xd3Source != NULL);

  result = fseek((FILE*)xd3Source->ioh,xd3Source->blksize*blkno,SEEK_SET);
  if (result != 0)
  {
    return -1;
  }
  xd3Source->onblk    = fread((void*)xd3Source->curblk,1,xd3Source->blksize,(FILE*)xd3Source->ioh);
  xd3Source->curblkno = blkno;

  return 0;
}

int main(int argc, const char *argv[])
{
  #define BUFFER_SIZE (64*1024)

  const char    *sourceFileName;
  FILE          *sourceHandle;

  xd3_config    xd3Config;
  xd3_stream    xd3Stream;
  int           result;
  uint8_t       buffer[BUFFER_SIZE];
  xd3_source    xd3Source;
  unsigned long in,out;
  int           n;
  char          ch;

  if (argc < 1)
  {
    fprintf(stderr,"No source file name given!\n");
    exit(1);
  }
  sourceFileName = argv[1];

  // open delta source
  sourceHandle = fopen(sourceFileName,"rb");
  if (sourceHandle == NULL)
  {
    fprintf(stderr,"Can not open source file (error: %s)!\n",strerror(errno));
    exit(1);
  }

  memset(&xd3Stream,0,sizeof(xd3Stream));

  // init delta stream
  memset(&xd3Config,0,sizeof(xd3Config));
  xd3_init_config(&xd3Config,0);
  xd3Config.getblk = readSourceBlock;
  result = xd3_config_stream(&xd3Stream,&xd3Config);
  if (result != 0)
  {
    fprintf(stderr,"Can not initialize xdelta decoder (result: %d)!\n",result);
    exit(1);
  }

  // init delta source
  memset(&xd3Source,0,sizeof(xd3Source));
  xd3Source.ioh      = sourceHandle;
  xd3Source.blksize  = BUFFER_SIZE;
  xd3Source.curblkno = (xoff_t)(-1);
  xd3Source.curblk   = buffer;
  result = xd3_set_source(&xd3Stream,&xd3Source);
  if (result != 0)
  {
    fprintf(stderr,"Can not initialize xdelta decoder source (result: %d)!\n",result);
    exit(1);
  }

  in  = 0L;
  out = 0L;
  while ((n = getc(stdin)) != EOF)
  {
    ch = (char)n; in++;

    xd3_avail_input(&xd3Stream,(uint8_t*)&ch,1);

    do
    {
      result = xd3_decode_input(&xd3Stream);
      switch (result)
      {
        case XD3_INPUT:
          break;
        case XD3_OUTPUT:
//fprintf(stderr,"%s,%d: XD3_OUTPUT xd3Stream.avail_out=%d\n",__FILE__,__LINE__,xd3Stream.avail_out);
          fwrite(xd3Stream.next_out,1,xd3Stream.avail_out,stdout); out += xd3Stream.avail_out;
          xd3_consume_output(&xd3Stream);
          break;
        case XD3_GETSRCBLK:
          fprintf(stderr,"Internal error: should not return XD3_GETSRCBLK!\n");
          exit(1);
          break;
        case XD3_GOTHEADER:
          break;
        case XD3_WINSTART:
          break;
        case XD3_WINFINISH:
          break;
        default:
          fprintf(stderr,"Invalid result of xdelta decoder (error: %d)!\n",result);
          exit(1);
          break;
      }
    }
    while (result != XD3_INPUT);
  }
  xd3_set_flags(&xd3Stream,xd3Stream.flags|XD3_FLUSH);
  // it is required to make 0 bytes available here - bug?
  xd3_avail_input(&xd3Stream,(uint8_t*)&ch,0);
  do
  {
    result = xd3_decode_input(&xd3Stream);
    switch (result)
    {
      case XD3_INPUT:
        break;
      case XD3_OUTPUT:
        fwrite(xd3Stream.next_out,1,xd3Stream.avail_out,stdout); out += xd3Stream.avail_out;
        xd3_consume_output(&xd3Stream);
        break;
      case XD3_GETSRCBLK:
        fprintf(stderr,"Internal error: should not return XD3_GETSRCBLK!\n");
        exit(1);
        break;
      case XD3_GOTHEADER:
        break;
      case XD3_WINSTART:
        break;
      case XD3_WINFINISH:
        break;
      default:
        fprintf(stderr,"Invalid result of xdelta decoder (result: %d)!\n",result);
        exit(1);
        break;
    }
  }
  while (result != XD3_INPUT);

  xd3_close_stream(&xd3Stream);
  xd3_free_stream(&xd3Stream);

  fprintf(stderr,"unpack: in=%lu out=%lu\n",in,out);

  return 0;
}

#ifdef __cplusplus
  }
#endif

/* end of file */
