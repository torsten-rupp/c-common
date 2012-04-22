/***********************************************************************\
*
* $Source: /home/torsten/cvs/sc/sc_codescanner.c,v $
* $Revision$
* $Author$
* Contents: code scanner functions
* Systems : all
*
\***********************************************************************/

#define DPRINTF_PREFIX "SCANNER"

/****************************** Includes *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "global.h"

#include "sc_debug.h"
#include "sc_codeparser.h"

#include "sc_ir.h"

#include "sc_codescanner.h"

/****************** Conditional compilation switches *******************/
#define _DEBUG

/***************************** Constants *******************************/
#define INPUT_BUFFER_SIZE 4096

#define CHAR_EOL '\n'

/***************************** Datatypes *******************************/
typedef struct
{
  char       *buffer;
  uint       bufferSize;
  uint       nextIn,nextOut;
  uint       available;
  ulong      lineNb;
  uint       column;
  const char *line;
} InputBuffer;

typedef struct
{
  char *keyword;
  int  tokenType;
} Keyword;

LOCAL Keyword keywords[] =
{
  {"module",    TOKENTYPE_KEYWORD_MODULE     },

  {"const",     TOKENTYPE_KEYWORD_CONST      },
  {"var",       TOKENTYPE_KEYWORD_VAR        },
  {"typedef",   TOKENTYPE_KEYWORD_TYPEDEF    },

  {"void",      TOKENTYPE_KEYWORD_VOID       },
  {"char",      TOKENTYPE_KEYWORD_CHAR       },
  {"int",       TOKENTYPE_KEYWORD_INT        },
  {"long",      TOKENTYPE_KEYWORD_LONG       },
  {"longlong",  TOKENTYPE_KEYWORD_LONGLONG   },
  {"float",     TOKENTYPE_KEYWORD_FLOAT      },
  {"double",    TOKENTYPE_KEYWORD_DOUBLE     },
  {"string",    TOKENTYPE_KEYWORD_STRING     },
  {"struct",    TOKENTYPE_KEYWORD_STRUCT     },

  {"array",     TOKENTYPE_KEYWORD_ARRAY      },
  {"list",      TOKENTYPE_KEYWORD_LIST       },
  {"set",       TOKENTYPE_KEYWORD_SET        },
  {"of",        TOKENTYPE_KEYWORD_OF         },

  {"extern",    TOKENTYPE_KEYWORD_EXTERN     },
  {"inline",    TOKENTYPE_KEYWORD_INLINE     },
  {"public",    TOKENTYPE_KEYWORD_PUBLIC     },

  {"return",    TOKENTYPE_KEYWORD_RETURN     },
};

/***************************** Variables *******************************/
LOCAL ReadDataFunction readData;
LOCAL InputBuffer      inputBuffer;

/****************************** Macros *********************************/
#define GET_BUFFER(i)    ((inputBuffer.buffer[i] != '\0')?inputBuffer.buffer[i]:CHAR_EOL)
#define PUT_BUFFER(i,ch) do { inputBuffer.buffer[i] = (ch != CHAR_EOL)?ch:'\0'; } while (0)

/***************************** Functions *******************************/

#ifdef __cplusplus
extern "C" {
#endif

#if 0
LOCAL int inputAvailable(void)
{
  if      (inputBuffer.nextOut<inputBuffer.nextIn)
  {
    return inputBuffer.nextIn-inputBuffer.nextOut;
  }
  else if (inputBuffer.nextOut>inputBuffer.nextIn)
  {
    return INPUT_BUFFER_SIZE-inputBuffer.nextOut+inputBuffer.nextIn;
  }
  else
  {
    return INPUT_BUFFER_SIZE;
  }
}
#endif /* 0 */

/***********************************************************************\
* Name       : getChar
* Purpose    : get next char from input
* Input      : userData - user data
* Output     : -
* Return     : next char or EOF
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL int getChar(void *userData)
{
  unsigned int availableSpace;
  bool         eofFlag;
  char         buffer[256];
  int          n,z;
  char         ch;

  // fill buffer
  if (inputBuffer.available <= 0)
  {
    assert(inputBuffer.nextOut == inputBuffer.nextIn);
    availableSpace = INPUT_BUFFER_SIZE/2;
    eofFlag = FALSE;
    while ((availableSpace > 0) && !eofFlag)
    {
      n = readData(userData,buffer,MIN(availableSpace,sizeof(buffer)));
      if (n > 0)
      {
        for (z = 0; z < n; z++)
        {
          PUT_BUFFER(inputBuffer.nextIn,buffer[z]);
          inputBuffer.nextIn = (inputBuffer.nextIn + 1) % INPUT_BUFFER_SIZE;
        }
        inputBuffer.available += n;
        availableSpace -= n;
      }
      else
      {
        eofFlag = TRUE;
      }
    }
  }

  // get char
  if (inputBuffer.available>0)
  {
    // get char
    ch = GET_BUFFER(inputBuffer.nextOut);
    inputBuffer.nextOut = (inputBuffer.nextOut + 1) % INPUT_BUFFER_SIZE;
    inputBuffer.available--;

    // adapt line/column number, line
    if (ch != CHAR_EOL)
    {
      inputBuffer.column++;
    }
    else
    {
      inputBuffer.lineNb++;
      inputBuffer.column = 0;

      inputBuffer.line = &inputBuffer.buffer[inputBuffer.nextOut];
    }
  }
  else
  {
    ch = EOF;
  }
//fprintf(stderr,"%s,%d: getchar '%c',%d\n",__FILE__,__LINE__,isalnum(ch)?ch:'.',ch);

  return ch;
}

/***********************************************************************\
* Name       : ungetChar
* Purpose    : revoke char into input
* Input      : userData - user data
*              ch       - char
* Output     : -
* Return     : -
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL void ungetChar(void *userData, char ch)
{
  ulong      n;
  const char *s;

  assert(inputBuffer.available<INPUT_BUFFER_SIZE);

//fprintf(stderr,"%s,%d: ungetchar '%c',%d\n",__FILE__,__LINE__,isalnum(ch)?ch:'.',ch);

  // revoke char
  inputBuffer.nextOut = (inputBuffer.nextOut > 0)?inputBuffer.nextOut - 1:INPUT_BUFFER_SIZE - 1;
  PUT_BUFFER(inputBuffer.nextOut,ch);
  inputBuffer.available++;

  // adapt line/column number, line
  if (ch != CHAR_EOL)
  {
    assert(inputBuffer.column > 0);
    inputBuffer.column--;
  }
  else
  {
    assert(inputBuffer.lineNb > 0);
    inputBuffer.lineNb--;
    n = 0;
    s = &inputBuffer.buffer[inputBuffer.nextOut];
    do
    {
      inputBuffer.column = n;
      inputBuffer.line   = s;
      n++;
      s = (s > inputBuffer.buffer)?s - 1:&inputBuffer.buffer[INPUT_BUFFER_SIZE - 1];
    }
    while ((s != &inputBuffer.buffer[inputBuffer.nextIn]) && ((*s) != '\0'));
  }
}

/***********************************************************************\
* Name       : skipSpacesComments
* Purpose    : skip spaces and comments
* Input      : userData - user data
* Output     : -
* Return     : -
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL void skipSpacesComments(void *userData)
{
  char ch1,ch2;

  for(;;)
  {
    ch1 = getChar(userData);
    if (ch1 == EOF) return;

    // skip spaces
    if (isspace(ch1))
    {
      continue;
    }

    ch2 = getChar(userData);

    // skip comments
    if ((ch1 == '/') && (ch2 == '*'))
    {
      ch2 = getChar(userData);
      if (ch2 == EOF) return;
      do
      {
        ch1 = ch2;
        ch2 = getChar(userData);
        if (ch2 == EOF) return;
      }
      while ((ch1 != '*') || (ch2 != '/'));
      continue;
    }

    if ((ch1 == '/') && (ch2 == '/'))
    {
      do
      {
        ch1 = getChar(userData);
        if (ch1 == EOF) return;
      }
      while (ch1 != CHAR_EOL);
      continue;
    }

    // ok
    ungetChar(userData,ch2);
    ungetChar(userData,ch1);

    break;
  }
}

/***********************************************************************\
* Name       : checkCharRange
* Purpose    : check if char is in given range
* Input      : lower - lower limit
*              ch    - char
*              upper - upper limit
* Output     : -
* Return     : TRUE if lower <= ch <= upper, FALSE otherwise
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL inline bool checkCharRange(char lower, char ch, char upper)
{
  return (lower <= ch) && (ch <= upper);
}

/*---------------------------------------------------------------------*/

/***********************************************************************\
* Name       : findKeyword
* Purpose    : find keyword
* Input      : s - string 
* Output     : -
* Return     : token type or TOKENTYPE_UNKNOWN
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL int findKeyword(const char *s)
{
  int n;
  int z,i;

  n = strlen(s);
  for (z = 0; z < SIZE_OF_ARRAY(keywords); z++)
  {
    if (n == (int)strlen(keywords[z].keyword))
    {
      i = 0;
      while ((i < n) && (s[i] == keywords[z].keyword[i]))
      {
        i++;
      }
      if (i >= n) return keywords[z].tokenType;
    }
  }

  return TOKENTYPE_UNKNOWN;
}

/***********************************************************************\
* Name       : tokenIntegerFloat
* Purpose    : get number token
* Input      : token          - token to fill
*              tokenInfoBlock - token info to fill
*              userData       - user data
* Output     : -
* Return     : TOKEN_NUMBER or TOKENTYPE_UNKNOWN if not a number
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL int tokenIntegerFloat(Token *token, TokenInfoBlock *tokenInfoBlock, void *userData)
{
  int    i;
  int    ch;
  double f;

  ch = getChar(userData);
  if (ch == EOF) return TOKENTYPE_NONE;
  if (!checkCharRange('0',ch,'9'))
  {
    ungetChar(userData,ch);
    return TOKENTYPE_UNKNOWN;
  }

  i = 0;
  token->i = 0;
  while (checkCharRange('0',ch,'9'))
  {
    token->i = token->i*10+(ch-'0');
    ch = getChar(userData);
    if (ch == EOF) return TOKENTYPE_NONE;
  }
  if (ch != '.')
  {
    ungetChar(userData,ch);

    return TOKENTYPE_INTEGER;
  }
  else
  {
    token->d = (double)token->i;
    f = 0.1;
    ch = getChar(userData);
    if (ch == EOF) return TOKENTYPE_NONE;
    while (checkCharRange('0',ch,'9'))
    {
      token->d = token->d+f*(double)(ch-'0');
      f = f/10;
      ch = getChar(userData);
      if (ch == EOF) return TOKENTYPE_NONE;
    }

    ungetChar(userData,ch);

    return TOKENTYPE_FLOAT;
  }
}

/***********************************************************************\
* Name       : tokenString
* Purpose    : get string token
* Input      : token          - token to fill
*              tokenInfoBlock - token info to fill
*              userData       - user data
* Output     : -
* Return     : TOKENTYPE_STRING or TOKENTYPE_UNKNOWN if not a string
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL int tokenString(Token *token, TokenInfoBlock *tokenInfoBlock, void *userData)
{
  int i;
  int ch;
  int z;

  // check if string
  ch = getChar(userData);
  if (ch == EOF) return TOKENTYPE_NONE;
  if (ch != '"') 
  {
    ungetChar(userData,ch);
    return TOKENTYPE_UNKNOWN;
  }

  // scan string  
  i = 0;
  z = 0;
  ch = getChar(userData);
  if (ch == EOF) return TOKENTYPE_NONE;
  while ((z < MAX_STRING_LEN) && (ch != '"'))
  {
#if 0
    if (ch=='\\')
    {
      ch=getChar(userData);
      if (ch==EOF) return TOKENTYPE_NONE;
      switch (ch)
      {
        case 'n':
          ch='\n';
          break;
        case '"':
        case '\\':
        default:
          break;
      }
    }
#endif /* 0 */
    token->s[z] = ch;
    z++;
    ch = getChar(userData);
    if (ch == EOF) return TOKENTYPE_NONE;
  }
  token->s[z] = '\0';

  return TOKENTYPE_STRING;
}

/***********************************************************************\
* Name       : tokenOperator
* Purpose    : get operator token
* Input      : token          - token to fill
*              tokenInfoBlock - token info to fill
*              userData       - user data
* Output     : -
* Return     : operator or TOKENTYPE_UNKNOWN if not a operator
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL int tokenOperator(TokenInfoBlock *tokenInfoBlock, void *userData)
{
  int i;
  int ch,ch1,ch2;
  int tokenType;

  i = 0;
  ch = getChar(userData);
  if (ch == EOF) return TOKENTYPE_NONE;
  switch (ch)
  {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
    case '=':
      tokenType = ch;
      break;
    case '.':
      ch1 = getChar(userData);
      ch2 = getChar(userData);
      if ((ch1 == '.') && (ch2 == '.'))
      {
        tokenType = TOKENTYPE_ELLIPSE;
      }
      else
      {
        if (ch2 != EOF)
        {
          ungetChar(userData,ch2);
        }
        if (ch1 != EOF)
        {
          ungetChar(userData,ch1);
        }
        tokenType = ch;
      }
      break;
    default:
      ungetChar(userData,ch);
      tokenType = TOKENTYPE_UNKNOWN;
      break;
  }

  return tokenType;
}

/***********************************************************************\
* Name       : tokenKeywordIdentifier
* Purpose    : get keyword/identifier token
* Input      : token          - token to fill
*              tokenInfoBlock - token info to fill
*              userData       - user data
* Output     : -
* Return     : keyword or TOKENTYPE_IDENTIFIER or TOKENTYPE_UNKNOWN if not a
*              keyword/identifier
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL int tokenKeywordIdentifier(Token *token, TokenInfoBlock *tokenInfoBlock, void *userData)
 {
  int i;
  int ch;
  int tokenType;
  int z;

  // check if text/string
  ch = getChar(userData);
  if (ch == EOF) return TOKENTYPE_NONE;
  if (!checkCharRange('A',toupper(ch),'Z') && (ch != '_') && (ch != '@'))
  {
    ungetChar(userData,ch);
    return TOKENTYPE_UNKNOWN;
  }

  // get text
  i = 0;
  z = 0;
  while (checkCharRange('A',toupper(ch),'Z') ||
         checkCharRange('0',ch,'9') ||
         (ch == '_') ||
         (ch == '@')
        )
  {
    token->s[z] = ch;
    z++;
    ch = getChar(userData);
    if (ch == EOF) return TOKENTYPE_NONE;
  }
  token->s[z] = '\0';

  ungetChar(userData,ch);

  // check if keyword or identifier
  tokenType = findKeyword(token->s);
  if (tokenType != TOKENTYPE_UNKNOWN)
  {
  }
  else
  {
    strncpy(token->identifier,token->s,MAX_IDENTIFIER_LEN);
    tokenType = TOKENTYPE_IDENTIFIER;
  }

  return tokenType;
}

/***********************************************************************\
* Name       : tokenChar
* Purpose    : get char token
* Input      : token          - token to fill
*              tokenInfoBlock - token info to fill
*              userData       - user data
* Output     : -
* Return     : char
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL int tokenChar(Token *token, TokenInfoBlock *tokenInfoBlock, void *userData)
 {
  int ch;

  // get character
  ch = getChar(userData);
  if (ch == EOF) return TOKENTYPE_NONE;

  return ch;
}

/***********************************************************************\
* Name       : scCodeScanner_lex
* Purpose    : scanner for BISON
* Input      : userData - user data
* Output     : token          - token
*              tokenInfoBlock - token info block
* Return     : token type
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

int scCodeScanner_lex(Token *token, TokenInfoBlock *tokenInfoBlock, void *userData)
{
  int tokenType;

  assert(tokenInfoBlock != NULL);

  // skip spaces and comments
  skipSpacesComments(userData);  

  // initialise info-block
  tokenInfoBlock->timestamp    = 0;
  tokenInfoBlock->first_line   = inputBuffer.lineNb;
  tokenInfoBlock->first_column = inputBuffer.column;
  tokenInfoBlock->last_line    = inputBuffer.lineNb;
  tokenInfoBlock->last_column  = inputBuffer.column;
  tokenInfoBlock->line         = inputBuffer.line;
//fprintf(stderr,"%s,%d: %p %d %d\n",__FILE__,__LINE__,tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column);

  // integer/float
  tokenType = tokenIntegerFloat(token,tokenInfoBlock,userData);
  if (tokenType != TOKENTYPE_UNKNOWN)
  {
    #ifdef DEBUG
      if (tokenType==TOKENTYPE_INTEGER) dprintf(DEBUGGROUP_SCANNER,1,"Line '%s', %lu:%lu: integer, %ld\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column,token->i);
      if (tokenType==TOKENTYPE_FLOAT)   dprintf(DEBUGGROUP_SCANNER,1,"Line '%s', %lu:%lu: float, %f\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column,token->d);
    #endif
    tokenInfoBlock->last_line   = inputBuffer.lineNb;
    tokenInfoBlock->last_column = inputBuffer.column;
    return tokenType;
  }

  // string
  tokenType = tokenString(token,tokenInfoBlock,userData);
  if (tokenType != TOKENTYPE_UNKNOWN)
  {
    #ifdef DEBUG
      if (tokenType != TOKENTYPE_NONE) dprintf(DEBUGGROUP_SCANNER,1,"Line '%s', %lu:%lu: STRING, '%s'\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column,token->s);
    #endif
    tokenInfoBlock->last_line   = inputBuffer.lineNb;
    tokenInfoBlock->last_column = inputBuffer.column;
    return tokenType;
  }

  // operator
  tokenType = tokenOperator(tokenInfoBlock,userData);
  if (tokenType != TOKENTYPE_UNKNOWN)
  {
    #ifdef DEBUG
      if      (tokenType >= 256)            dprintf(DEBUGGROUP_SCANNER,1,"Line '%s', %lu:%lu: operator, %d\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column,tokenType);
      else if (tokenType != TOKENTYPE_NONE) dprintf(DEBUGGROUP_SCANNER,1,"Line '%s', %lu:%lu: operator, '%c'\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column,tokenType);
    #endif
    tokenInfoBlock->last_line   = inputBuffer.lineNb;
    tokenInfoBlock->last_column = inputBuffer.column;
    return tokenType;
  }

  // keyword/identifier
  tokenType = tokenKeywordIdentifier(token,tokenInfoBlock,userData);
  if (tokenType != TOKENTYPE_UNKNOWN)
  {
    #ifdef DEBUG
      if      (tokenType == TOKENTYPE_IDENTIFIER) dprintf(DEBUGGROUP_SCANNER,1,"Line '%s', %lu:%lu: identifier '%s'\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column,token->identifier);
      else                                        dprintf(DEBUGGROUP_SCANNER,1,"Line '%s', %lu:%lu: keyword %d\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column,tokenType);
    #endif
    tokenInfoBlock->last_line   = inputBuffer.lineNb;
    tokenInfoBlock->last_column = inputBuffer.column;
    return tokenType;
  }

  // character
  tokenType = tokenChar(token,tokenInfoBlock,userData);
  if (tokenType != TOKENTYPE_UNKNOWN)
  {
    #ifdef DEBUG
      if (tokenType != TOKENTYPE_NONE) dprintf(DEBUGGROUP_SCANNER,1,"Line '%s', %lu:%lu: character, %c\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column,tokenType);
    #endif
    tokenInfoBlock->last_line   = inputBuffer.lineNb;
    tokenInfoBlock->last_column = inputBuffer.column;
    return tokenType;
  }

  // unknown
  #ifdef DEBUG
   fprintf(stderr,"Token '%s', %lu:%lu: unknown\n",tokenInfoBlock->line,tokenInfoBlock->first_line,tokenInfoBlock->first_column);
  #endif

  return TOKENTYPE_UNKNOWN;
}

/***********************************************************************\
* Name       : scCodeScanner_init
* Purpose    : scanner init
* Input      : getCharFunction   - function to get next input char
*              ungetCharFunction - function to unget last char
* Output     : -
* Return     : TRUE if no error occurred, FALSE otherwise
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

bool scCodeScanner_init(ReadDataFunction readDataFunction)
{
  // initialise variables
  readData = readDataFunction;

  inputBuffer.buffer = (char*)malloc(INPUT_BUFFER_SIZE);
  if (inputBuffer.buffer == NULL)
  {
    return FALSE;
  }
  inputBuffer.buffer[0] = '\0';

  inputBuffer.nextIn  = 0;
  inputBuffer.nextOut = 0;
  inputBuffer.lineNb  = 1;
  inputBuffer.column  = 0;
  inputBuffer.line    = inputBuffer.buffer;

  return TRUE;
}

/***********************************************************************\
* Name       : scCodeScanner_done
* Purpose    : scanner deinit
* Input      : -
* Output     : -
* Return     : -
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

void scCodeScanner_done(void)
{
  assert(inputBuffer.buffer != NULL);

  // free resources
  free(inputBuffer.buffer);  
}

#ifdef __cplusplus
}
#endif

/* end of file */
