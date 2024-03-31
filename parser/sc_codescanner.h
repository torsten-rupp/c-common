/***********************************************************************\
*
* Contents: code scanner functions
* Systems : all
*
\***********************************************************************/

#ifndef __SC_CODESCANNER__
#define __SC_CODESCANNER__

/****************************** Includes *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "global.h"

#include "sc_ir.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/
#define MAX_STRING_LEN 255
#define MAX_IDENTIFIER_LEN 255
//#define MAX_ARGUMENTS 64

#define TOKENTYPE_NONE     0
#define TOKENTYPE_UNKNOWN -1

/***************************** Datatypes *******************************/
// input functions
typedef int (*ReadDataFunction)(void *userData, void *buffer, unsigned int length);

// token (avoid large structures, because of stack usage!)
typedef struct
 {
   union
   {
     // character

     // keyword

     // INTEGER
     long i;

     // FLOAT
     double d;

     // STRING
     char s[MAX_STRING_LEN];

     // identifier
     char identifier[MAX_IDENTIFIER_LEN];

     // intemediate code
     IRNode *irNode;

     IRIdentifier irIdentifier;
     IRFilename   irFilename;

     IRArraySize  irArraySize;

     IRAttributes      irAttributes;
     IRDatatype        irDatatype;
     IRConstExpression irConstExpression;
     IRExpression      irExpression;
     IRArgument        irArgument;
     IRParameter       irParameter;

     IRIdentifierList irIdentifierList;
     IRFilenameList   irFilenameList;

     IRArraySizeList irArraySizeList;

     IRDatatypeList  irDatatypeList;
     IRArgumentList  irArgumentList;
     IRList          irCodeList;
     IRParameterList irParameterList;

     IRConstantDeclaration irConstantDeclaration;
     IRDatatypeDeclaration irDatatypeDeclaration;
     IRVariableDeclaration irVariableDeclaration;
     IRFunctionDeclaration irFunctionDeclaration;

     IRStatement irStatement;
   };
   struct
   {
     String prefix;
     String postfix;
   } comment;
 } Token;

// info block
typedef struct
 {
  int        timestamp;
  ulong      first_line;
  ulong      first_column;
  ulong      last_line;
  ulong      last_column;
  const char *line;
//char text[256];
 } TokenInfoBlock;

/***************************** Variables *******************************/

/****************************** Macros *********************************/
//#define YYLEX_PARAM userData

#define YYSTYPE Token
#undef YYLTYPE
#define YYLTYPE TokenInfoBlock

/***************************** Functions *******************************/

#ifdef __cplusplus
 extern "C" {
#endif

/***********************************************************************\
* Name       : scCodeScanner_init
* Purpose    : scanner init
* Input      : readDataFunction - function to read data block
* Output     : -
* Return     : TRUE if no error occurred, FALSE otherwise
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

bool scCodeScanner_init(ReadDataFunction readDataFunction);

/***********************************************************************\
* Name       : scCodeScanner_done
* Purpose    : scanner deinit
* Input      : -
* Output     : -
* Return     : -
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

void scCodeScanner_done(void);

/***********************************************************************\
* Name       : scCodeScanner_lex
* Purpose    : scanner for BISON
* Input      : userData - user data
* Output     : Token          - token
*              TokenInfoBlock - token info block
* Return     : token type
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

int scCodeScanner_lex(Token *token, TokenInfoBlock *tokenInfoBlock, void *userData);

#ifdef __cplusplus
 }
#endif

#endif /* __SC_CODESCANNER__ */

/* end of file */
