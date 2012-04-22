%{
/***********************************************************************\
*
* $Source: /home/torsten/cvs/sc/sc_codeparser.y,v $
* $Revision: 1.1 $
* $Author: torsten $
* Contents: code parser
* Systems : all
*
\***********************************************************************/

#define DPRINTF_PREFIX "PARSER"

/****************************** Includes *******************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>

#include "global.h"
#include "dprintf.h"
#include "strings.h"
#include "lists.h"

#include "sc_debug.h"
#include "sc_error.h"
#include "sc_codescanner.h"
#include "sc_codeparser.h"
#include "sc_files.h"
#include "sc_ir.h"
#include "sc_datatype.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/
#define CONTEXT_STACK_SIZE 64

/***************************** Datatypes *******************************/
typedef struct
{
  IRNode *irFunctionNode;
  IRList irList;
} Context;

/***************************** Variables *******************************/
LOCAL FileNode *fileNode;

LOCAL Context contextStack[CONTEXT_STACK_SIZE];
LOCAL int contextStackIndex;

LOCAL String moduleName;

/****************************** Macros *********************************/
//#define YYPARSE_PARAM FILE *userData, bool quietFlag
// #define YYPARSE_PARAM FILE *&userData, bool quietFlag
#define YYERROR_VERBOSE

#define PARSE_ERROR(line,lineNb,column,format,args...) \
  do \
  { \
    if (!quietFlag) parseError(line,lineNb,column,format, ## args); \
    YYABORT; \
  } while (0)

#define CURRENT_FUNCTION_NODE        contextStack[contextStackIndex].irFunctionNode
#define CURRENT_FUNCTION_DECLARATION ((CURRENT_FUNCTION_NODE != NULL)?((IRFunctionDeclaration*)CURRENT_FUNCTION_NODE->data):NULL)
#define CURRENT_IRLIST               contextStack[contextStackIndex].irList
#define CURRENT_FILE                 fileNode

#define INIT_IR(ir,irType,n) \
 do { \
   ir.irHeader.fileNode = fileNode; \
   ir.irHeader.lineNb   = n.first_line; \
   ir.irHeader.column   = n.first_column; \
   ir.irHeader.type     = irType; \
 } while (0)

/***************************** Functions *******************************/

#ifdef __cplusplus
 extern "C" {
#endif

/***********************************************************************\
* Name       : yyerror
* Purpose    : scanner error handler
* Input      : s - error string
* Output     : -
* Return     : -
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL int yyerror(TokenInfoBlock *tokenInfoBlock, void *userData, bool quietFlag, const char *s)
{
  // ignore
  s=NULL;

  return(0);
}

/***********************************************************************\
* Name       : parseError
* Purpose    : parser error handler
* Input      : lineNb - line number
*              column - column
*              format - format string
*              ...    - optional arguments    
* Output     : -
* Return     : -
* Side-effect: unknown
* Notes      : -
\***********************************************************************/

LOCAL void parseError(const char *line, ulong lineNb, uint column, const char *format, ...)
{
  va_list arguments;
  ulong   z;

  fprintf(stderr,"ERROR: ");
  va_start(arguments,format);
  vfprintf(stderr,format,arguments);
  va_end(arguments);
  fprintf(stderr," in line %lu, column %u\n",lineNb,column);
  if (line!=NULL)
  {
    fprintf(stderr,"%s\n",line);
    for (z=0;z<column;z++) fprintf(stderr," ");
    fprintf(stderr,"^\n");
  }
}

/***********************************************************************\
* Name   : 
* Purpose: 
* Input  : -
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

LOCAL void pushContext(void)
{
  assert(contextStackIndex < CONTEXT_STACK_SIZE - 1);

  // save context
  contextStackIndex++;

  // create new context
  CURRENT_FUNCTION_NODE = NULL;
  List_init(&CURRENT_IRLIST);
}

/***********************************************************************\
* Name   : 
* Purpose: 
* Input  : -
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

LOCAL void popContext(void)
{
  assert(contextStackIndex > 0);

  // delete context
  List_done(&CURRENT_IRLIST,(NodeFreeFunction)scIR_deleteNode,NULL);

  // restore context
  contextStackIndex--;
}

/***********************************************************************\
* Name   : promoteConstExpressionType
* Purpose: promote type of constant expression
* Input  : irExpression - constant expression
*          type         - promotion type
* Output : irExpression - promoted constant expression
* Return : -
* Notes  : -
\***********************************************************************/

LOCAL void promoteConstExpressionType(IRConstExpression *irConstExpression, ExpressionTypes type)
{
  assert(irConstExpression != NULL);

  if ((irConstExpression->type == EXPRESSION_TYPE_INTEGER) && (type == EXPRESSION_TYPE_FLOAT))
  {
    irConstExpression->type = EXPRESSION_TYPE_FLOAT;
    irConstExpression->d    = (double)irConstExpression->i;
  }
}

%}

/*---------------------------------------------------------------------*/

%require "2.3"
%pure-parser
%debug

%token TOKENTYPE_KEYWORD_MODULE
//%token TOKENTYPE_END

%token TOKENTYPE_KEYWORD_CONST
%token TOKENTYPE_KEYWORD_VAR
%token TOKENTYPE_KEYWORD_TYPEDEF

%token TOKENTYPE_KEYWORD_VOID
%token TOKENTYPE_KEYWORD_CHAR
%token TOKENTYPE_KEYWORD_SHORT
%token TOKENTYPE_KEYWORD_INT
%token TOKENTYPE_KEYWORD_LONG
%token TOKENTYPE_KEYWORD_LONGLONG
%token TOKENTYPE_KEYWORD_FLOAT
%token TOKENTYPE_KEYWORD_DOUBLE
%token TOKENTYPE_KEYWORD_STRING
%token TOKENTYPE_KEYWORD_ARRAY
%token TOKENTYPE_KEYWORD_LIST
%token TOKENTYPE_KEYWORD_SET
%token TOKENTYPE_KEYWORD_OF
%token TOKENTYPE_KEYWORD_STRUCT

%token TOKENTYPE_KEYWORD_EXTERN
%token TOKENTYPE_KEYWORD_INLINE
%token TOKENTYPE_KEYWORD_PUBLIC

%token TOKENTYPE_KEYWORD_RETURN

%token TOKENTYPE_IDENTIFIER

%token TOKENTYPE_INTEGER
%token TOKENTYPE_FLOAT
%token TOKENTYPE_STRING

%token TOKENTYPE_ELLIPSE

%left '+' '-' '*'
%right '/' '%'
//%left TOKENTYPE_UMINUS
%right '^'

%lex-param   {void *userData}

%parse-param {void *userData}
%parse-param {bool quietFlag}

%start start

%%

// -------------------------------------------------------
start
  : module
  | /* empty */
  // --- error
  | error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected 'module'");
    }
  ;

// -------------------------------------------------------
module
  : TOKENTYPE_KEYWORD_MODULE identifier ';'
    {
      moduleName = $2.irIdentifier.name;
    }
    code
    {
    }
  | TOKENTYPE_KEYWORD_MODULE identifier error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    }
  | TOKENTYPE_KEYWORD_MODULE error ';'
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected identifier");
    }
  ;

// -------------------------------------------------------

attributes
  : attributes TOKENTYPE_KEYWORD_PUBLIC
    {
      $$.irAttributes = $1.irAttributes | SC_IR_ATTRIBUTE_MASK(PUBLIC);
    }
  | /* empty */
    {
      $$.irAttributes = SC_IR_ATTRIBUTE_MASK(NONE);
    }
  ;

// -------------------------------------------------------
code
  : attributes constantDeclaration
    {
      IRConstantDeclaration irConstantDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add constant declaration '%s', attributes 0x%x\n",String_cString($2.irConstantDeclaration.name),$1.irAttributes);

      irConstantDeclaration            = $2.irConstantDeclaration;
      irConstantDeclaration.attributes = $1.irAttributes;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irConstantDeclaration,IR_TYPE_CONSTANT_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | attributes datatypeDeclaration
    {
      IRDatatypeDeclaration irDatatypeDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add datatype declaration '%s', '%s', attributes 0x%x\n", String_cString($2.irDatatypeDeclaration.name),scIR_getBaseDatatype(&$2.irDatatypeDeclaration.datatype),$1.irAttributes);

      irDatatypeDeclaration            = $2.irDatatypeDeclaration;
      irDatatypeDeclaration.attributes = $1.irAttributes;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | attributes variableDeclaration
    {
      IRVariableDeclaration irVariableDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add variable declaration, %d names, attributes 0x%x\n", $2.irVariableDeclaration.nameList.count,$1.irAttributes);

      irVariableDeclaration            = $2.irVariableDeclaration;
      irVariableDeclaration.attributes = $1.irAttributes;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irVariableDeclaration,IR_TYPE_VARIABLE_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | attributes functionDeclaration
    {
      #define NODE $2.irNode
      #define DATA(name) ((IRFunctionDeclaration*)NODE->data)->name

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add function declaration '%s', %d argument(s), attributes 0x%x\n",String_cString(DATA(name)),DATA(argumentList.count),$1.irAttributes);

      DATA(attributes) = $1.irAttributes;
      List_add(&CURRENT_IRLIST,(Node*)NODE);

      #undef DATA
      #undef NODE
    }
    code
  | attributes functionForwardDeclaration
    {
      IRFunctionDeclaration irFunctionDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add function fordward declaration '%s', %d argument(s), attributes 0x%x\n",String_cString($2.irFunctionDeclaration.name),$2.irFunctionDeclaration.argumentList.count,$1.irAttributes);

      irFunctionDeclaration            = $2.irFunctionDeclaration;
      irFunctionDeclaration.attributes = $1.irAttributes | SC_IR_ATTRIBUTE_MASK(FORWARD);
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irFunctionDeclaration,IR_TYPE_FUNCTION_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | externConstantDeclaration
    {
      IRConstantDeclaration irConstantDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add extern constant declaration '%s'\n",String_cString($1.irConstantDeclaration.name));

      irConstantDeclaration = $1.irConstantDeclaration;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irConstantDeclaration,IR_TYPE_CONSTANT_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | externDatatypeDeclaration
    {
      IRDatatypeDeclaration irDatatypeDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add extern datatype declaration '%s', '%s'\n", String_cString($1.irDatatypeDeclaration.name),scIR_getBaseDatatype(&$1.irDatatypeDeclaration.datatype));

      irDatatypeDeclaration = $1.irDatatypeDeclaration;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | externVariableDeclaration
    {
      IRVariableDeclaration irVariableDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add extern variable declaration, %d names\n", $1.irVariableDeclaration.nameList.count);

      irVariableDeclaration = $1.irVariableDeclaration;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irVariableDeclaration,IR_TYPE_VARIABLE_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | externFunctionDefinition
    {
      IRFunctionDeclaration irFunctionDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add extern function definition '%s', %d argument(s)\n",String_cString($1.irFunctionDeclaration.name),$1.irFunctionDeclaration.argumentList.count);

      irFunctionDeclaration = $1.irFunctionDeclaration;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irFunctionDeclaration,IR_TYPE_FUNCTION_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | statement
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"add statement '%s'\n",scIR_getStatementType(&$1.irStatement));
      List_add(&CURRENT_IRLIST, (Node*)SC_IR_NEW_NODE($1.irStatement,IR_TYPE_STATEMENT,CURRENT_FILE,@1.first_line,@1.first_column));
    }
    code
  | /* empty */
  | error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"unexpected statement");
    }
  ;

// -------------------------------------------------------
constantDefinition
  : TOKENTYPE_KEYWORD_CONST datatype identifier
    {
      INIT_IR($$.irConstantDeclaration,IR_TYPE_CONSTANT_DECLARATION,@1);
      $$.irConstantDeclaration.parentFunctionDeclaration = NULL;
      $$.irConstantDeclaration.name                      = $3.irIdentifier.name;
      $$.irConstantDeclaration.datatype                  = $2.irDatatype;
      $$.irConstantDeclaration.location                  = LOCATION_NONE;
      $$.irConstantDeclaration.attributes                = SC_IR_ATTRIBUTE_MASK(NONE);
      $$.irConstantDeclaration.expression                = EXPRESSION_NONE;
    }
  | TOKENTYPE_KEYWORD_CONST datatype error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected constant name");
    }
  | TOKENTYPE_KEYWORD_CONST error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected constant datatype");
    }
  ;

datatypeDefinition
  : TOKENTYPE_KEYWORD_TYPEDEF datatype
    {
      INIT_IR($$.irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,@1);
      $$.irDatatypeDeclaration.parentFunctionDeclaration = NULL;
      $$.irDatatypeDeclaration.name                      = NULL;
      $$.irDatatypeDeclaration.datatype                  = $2.irDatatype;
      $$.irDatatypeDeclaration.location                  = LOCATION_NONE;
      $$.irDatatypeDeclaration.attributes                = SC_IR_ATTRIBUTE_MASK(NONE);
    }
  | TOKENTYPE_KEYWORD_TYPEDEF error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    }
  ;

variableIdentifierList
  : variableIdentifierList ',' identifier
    {
      $$.irIdentifierList = $1.irIdentifierList;
      List_add(&$$.irIdentifierList,(Node*)SC_IR_NEW_NODE($3.irIdentifier,IR_TYPE_IDENTIFIER,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | identifier
    {
      List_init(&$$.irIdentifierList);
      List_add(&$$.irIdentifierList,(Node*)SC_IR_NEW_NODE($1.irIdentifier,IR_TYPE_IDENTIFIER,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  ;

variableDefinition
  : datatype variableIdentifierList
    {
      INIT_IR($$.irVariableDeclaration,IR_TYPE_VARIABLE_DECLARATION,@1);
      $$.irVariableDeclaration.parentFunctionDeclaration = NULL;
      $$.irVariableDeclaration.nameList                  = $2.irIdentifierList;
      $$.irVariableDeclaration.datatype                  = $1.irDatatype;
      $$.irVariableDeclaration.location                  = LOCATION_NONE;
      $$.irVariableDeclaration.attributes                = SC_IR_ATTRIBUTE_MASK(NONE);
      $$.irVariableDeclaration.expression                = EXPRESSION_NONE;
    }
  ;

functionDefinition
  : datatype identifier '(' functionArguments ')'
    {
      INIT_IR($$.irFunctionDeclaration,IR_TYPE_FUNCTION_DECLARATION,@1);
      $$.irFunctionDeclaration.parentFunctionDeclaration = NULL;
      $$.irFunctionDeclaration.name                      = $2.irIdentifier.name;
      $$.irFunctionDeclaration.returnDatatype            = $1.irDatatype;
      $$.irFunctionDeclaration.location                  = LOCATION_NONE;
      $$.irFunctionDeclaration.attributes                = SC_IR_ATTRIBUTE_MASK(NONE);
      $$.irFunctionDeclaration.argumentList              = $4.irArgumentList;
      List_init(&$$.irFunctionDeclaration.code);
    }
  ;

// -------------------------------------------------------
constantDeclaration
  : constantDefinition '=' expression ';'
    {
      $$.irConstantDeclaration                           = $1.irConstantDeclaration;
      $$.irConstantDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irConstantDeclaration.location.type             = LOCATION_TYPE_INTERN;
      $$.irConstantDeclaration.expression                = $3.irExpression;
    }
  | constantDefinition '=' error
    {
      PARSE_ERROR(@1.line,@1.first_line,@1.last_column,"expected constant expression");
    }
  | constantDefinition error
    {
      PARSE_ERROR(@1.line,@1.first_line,@1.last_column,"expected '='");
    }
  ;

datatypeDeclaration
  : datatypeDefinition identifier ';'
    {
      $$.irDatatypeDeclaration                           = $1.irDatatypeDeclaration;
      $$.irDatatypeDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irDatatypeDeclaration.name                      = $2.irIdentifier.name;
      $$.irDatatypeDeclaration.location.type             = LOCATION_TYPE_INTERN;
    }
  ;

variableDeclaration
  : variableDefinition '=' expression ';'
    {
      $$.irVariableDeclaration                           = $1.irVariableDeclaration;
      $$.irVariableDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irVariableDeclaration.location.type             = LOCATION_TYPE_INTERN;
      $$.irVariableDeclaration.expression                = $3.irExpression;
    }
  | variableDefinition ';'
    {
      $$.irVariableDeclaration                           = $1.irVariableDeclaration;
      $$.irVariableDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irVariableDeclaration.location.type             = LOCATION_TYPE_INTERN;
      $$.irVariableDeclaration.expression                = EXPRESSION_NONE;
    }
  | variableDefinition '=' expression error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    }
  | variableDefinition '=' error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected expression");
    }
  | variableDefinition error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';' or '='");
    }
  ;

functionDeclaration
  : functionDefinition '{'
    {
      // create node
      $$.irNode = SC_IR_ALLOC_NODE(IR_TYPE_FUNCTION_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column);
      assert($$.irNode != NULL);

      // push context
      pushContext();
      CURRENT_FUNCTION_NODE = $$.irNode;
    }
    code '}'
    {
      IRList code;

//      dprintf(DEBUGGROUP_PARSER,2,"function declaration '%s'\n",String_cString($1.irFunctionDeclaration.name));

      // get statement list, restore context
      List_init(&code);
      List_move(&CURRENT_IRLIST,&code,LIST_START,LIST_END,NULL);
      popContext();

      // init function declartion
      $$.irNode = $3.irNode;
      (*((IRFunctionDeclaration*)$$.irNode->data))                         = $1.irFunctionDeclaration;
      ((IRFunctionDeclaration*)$$.irNode->data)->parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      ((IRFunctionDeclaration*)$$.irNode->data)->location.type             = LOCATION_TYPE_INTERN;
      ((IRFunctionDeclaration*)$$.irNode->data)->code                      = code;
    }
  ;

functionForwardDeclaration
  : functionDefinition ';'
    {
      $$.irFunctionDeclaration                           = $1.irFunctionDeclaration;
      $$.irFunctionDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irFunctionDeclaration.location.type             = LOCATION_TYPE_INTERN;
      List_init(&$$.irFunctionDeclaration.code);
    }
  ;

// -------------------------------------------------------

filename
  : TOKENTYPE_STRING
    {
      $$.irFilename.name = String_newCString($1.s);
    }
  ;

filenameList
  : filenameList ',' filename
    {
      $$.irFilenameList = $1.irFilenameList;
      List_add(&$$.irFilenameList,(Node*)SC_IR_NEW_NODE($3.irFilename,IR_TYPE_FILENAME,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | filename
    {
      List_init(&$$.irFilenameList);
      List_add(&$$.irFilenameList,(Node*)SC_IR_NEW_NODE($1.irFilename,IR_TYPE_FILENAME,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  ;

externConstantDeclaration
  : TOKENTYPE_KEYWORD_EXTERN filenameList constantDefinition ';'
    {
      $$.irConstantDeclaration                           = $3.irConstantDeclaration;
      $$.irConstantDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irConstantDeclaration.location.type             = LOCATION_TYPE_EXTERN;
      $$.irConstantDeclaration.location.filenameList     = $2.irFilenameList;
    }
  | TOKENTYPE_KEYWORD_EXTERN filenameList constantDefinition error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    }
  ;

externDatatypeDeclaration
  : TOKENTYPE_KEYWORD_EXTERN filenameList datatypeDefinition ';'
    {
      $$.irDatatypeDeclaration                           = $3.irDatatypeDeclaration;
      $$.irDatatypeDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irDatatypeDeclaration.location.type             = LOCATION_TYPE_EXTERN;
      $$.irDatatypeDeclaration.location.filenameList     = $2.irFilenameList;
    }
  | TOKENTYPE_KEYWORD_EXTERN filenameList datatypeDefinition error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    }
  ;

externVariableDeclaration
  : TOKENTYPE_KEYWORD_EXTERN filenameList variableDefinition ';'
    {
      $$.irVariableDeclaration                           = $3.irVariableDeclaration;
      $$.irVariableDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irVariableDeclaration.location.type             = LOCATION_TYPE_EXTERN;
      $$.irVariableDeclaration.location.filenameList     = $2.irFilenameList;
    }
  | TOKENTYPE_KEYWORD_EXTERN filenameList variableDefinition error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    }
  ;

externFunctionDefinition
  : TOKENTYPE_KEYWORD_EXTERN filenameList functionDefinition ';'
    {
      $$.irFunctionDeclaration                           = $3.irFunctionDeclaration;
      $$.irFunctionDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irFunctionDeclaration.location.type             = LOCATION_TYPE_EXTERN;
      $$.irFunctionDeclaration.location.filenameList     = $2.irFilenameList;
    }
  | TOKENTYPE_KEYWORD_EXTERN filenameList functionDefinition error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    }
  ;

// -------------------------------------------------------
statement
  : assignment ';'
    {
      INIT_IR($$.irStatement,IR_TYPE_STATEMENT,@1);
      $$.irStatement.type       = STATEMENT_TYPE_EXPRESSION;
      $$.irStatement.expression = $1.irExpression;
    }
  | functionCall ';'
    {
      INIT_IR($$.irStatement,IR_TYPE_STATEMENT,@1);
      $$.irStatement.type       = STATEMENT_TYPE_EXPRESSION;
      $$.irStatement.expression = $1.irExpression;
    }
  | TOKENTYPE_KEYWORD_RETURN expression ';'
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"return value\n");
      INIT_IR($$.irStatement,IR_TYPE_STATEMENT,@1);
      $$.irStatement.type       = STATEMENT_TYPE_RETURN;
      $$.irStatement.expression = $2.irExpression;
    }
  | TOKENTYPE_KEYWORD_RETURN ';'
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"return\n");
      INIT_IR($$.irStatement,IR_TYPE_STATEMENT,@1);
      $$.irStatement.type       = STATEMENT_TYPE_RETURN;
      $$.irStatement.expression = EXPRESSION_NONE;
    }
  ;

// -------------------------------------------------------
identifier
  : TOKENTYPE_IDENTIFIER
    {
      $$.irIdentifier.name = String_newCString($1.identifier);
    }
  ;

// -------------------------------------------------------
namedDatatype
  : datatype identifier ';'
    {
      INIT_IR($$.irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,@1);
      $$.irDatatypeDeclaration.name     = $2.irIdentifier.name;
      $$.irDatatypeDeclaration.datatype = $1.irDatatype;
    }
  ;

datatypeList
  : datatypeList namedDatatype
    {
      $$.irDatatypeList = $1.irDatatypeList;
      List_add(&$$.irDatatypeList,(Node*)SC_IR_NEW_NODE($2.irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | namedDatatype
    {
      List_init(&$$.irDatatypeList);
      List_add(&$$.irDatatypeList,(Node*)SC_IR_NEW_NODE($1.irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | /* empty */
    {
      List_init(&$$.irDatatypeList);
    }
  ;

arraySize
  : arraySize ',' constExpression
    {
      IRArraySize irArraySize;

      if ($3.irExpression.type != EXPRESSION_TYPE_INTEGER)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected constant integer expression");
      }

      $$.irArraySizeList = $1.irArraySizeList;
      irArraySize.size = $3.irExpression.i;
      List_add(&$$.irArraySizeList,(Node*)SC_IR_NEW_NODE($3.irArraySize,IR_TYPE_ARRAY_SIZE,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | constExpression
    {
      IRArraySize irArraySize;

      if ($1.irExpression.type != EXPRESSION_TYPE_INTEGER)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected constant integer expression");
      }

      List_init(&$$.irArraySizeList);
      irArraySize.size = $1.irExpression.i;
      List_add(&$$.irArraySizeList,(Node*)SC_IR_NEW_NODE($1.irArraySize,IR_TYPE_ARRAY_SIZE,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  ;

arraySizeList
  : arraySizeList '[' arraySize ']'
    {
      $$.irArraySizeList = $1.irArraySizeList;
      List_move(&$3.irArraySizeList,&$1.irArraySizeList,LIST_START,LIST_END,NULL);
    }
  | arraySizeList '[' ']'
    {
      IRArraySize irArraySize;

      $$.irArraySizeList = $1.irArraySizeList;
      irArraySize.size = 0;
      List_add(&$$.irArraySizeList,(Node*)SC_IR_NEW_NODE(irArraySize,IR_TYPE_ARRAY_SIZE,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | '[' arraySize ']'
    {
      $$.irArraySizeList = $1.irArraySizeList;
    }
  | '[' ']'
    {
      IRArraySize irArraySize;

      List_init(&$$.irArraySizeList);
      irArraySize.size = 0;
      List_add(&$$.irArraySizeList,(Node*)SC_IR_NEW_NODE(irArraySize,IR_TYPE_ARRAY_SIZE,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | '[' error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ']'");
    }
  | error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected array dimension");
    }
  ;

datatype
  : TOKENTYPE_KEYWORD_VOID
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_VOID;
    }
  | TOKENTYPE_KEYWORD_CHAR
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_CHAR;
    }
  | TOKENTYPE_KEYWORD_SHORT
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_SHORT;
    }
  | TOKENTYPE_KEYWORD_INT
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_INT;
    }
  | TOKENTYPE_KEYWORD_LONG
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_LONG;
    }
  | TOKENTYPE_KEYWORD_LONGLONG
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_LONGLONG;
    }
  | TOKENTYPE_KEYWORD_FLOAT
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_FLOAT;
    }
  | TOKENTYPE_KEYWORD_DOUBLE
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_DOUBLE;
    }
  | TOKENTYPE_KEYWORD_STRING
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type = DATA_TYPE_STRING;
    }
  | identifier
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type      = DATA_TYPE_USER;
      $$.irDatatype.user.name = $1.irIdentifier.name;
    }
  | datatype '*'
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type             = DATA_TYPE_POINTER;
      $$.irDatatype.pointer.datatype = SC_IR_NEW($1.irDatatype);
    }
  | datatype arraySizeList
    {
//      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }

  | TOKENTYPE_KEYWORD_STRUCT '{' datatypeList '}'
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"datatype structure\n");
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type                 = DATA_TYPE_STRUCT;
      $$.irDatatype.struct_.datatypeList = $3.irDatatypeList;
    }
  | TOKENTYPE_KEYWORD_STRUCT '{' datatypeList error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '}'");
    }
  | TOKENTYPE_KEYWORD_STRUCT '{' error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype list");
    }
  | TOKENTYPE_KEYWORD_STRUCT error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '{'");
    }

  | TOKENTYPE_KEYWORD_ARRAY arraySizeList '<' datatype '>'
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type                = DATA_TYPE_ARRAY;
      $$.irDatatype.array.datatype      = SC_IR_NEW($4.irDatatype);
      $$.irDatatype.array.arraySizeList = $2.irArraySizeList;
    }
  | TOKENTYPE_KEYWORD_ARRAY arraySizeList '<' datatype error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '>'");
    }
  | TOKENTYPE_KEYWORD_ARRAY arraySizeList '<' error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    }
  | TOKENTYPE_KEYWORD_ARRAY arraySizeList error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ',' or 'of'");
    }
  | TOKENTYPE_KEYWORD_ARRAY error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected array specification");
    }
  | TOKENTYPE_KEYWORD_ARRAY arraySizeList TOKENTYPE_KEYWORD_OF datatype
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type                = DATA_TYPE_ARRAY;
      $$.irDatatype.array.datatype      = SC_IR_NEW($4.irDatatype);
      $$.irDatatype.array.arraySizeList = $2.irArraySizeList;
    }
  | TOKENTYPE_KEYWORD_ARRAY arraySizeList TOKENTYPE_KEYWORD_OF error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    }

  | TOKENTYPE_KEYWORD_LIST '<' datatype '>'
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type          = DATA_TYPE_LIST;
      $$.irDatatype.list.datatype = SC_IR_NEW($3.irDatatype);
    }
  | TOKENTYPE_KEYWORD_LIST '<' datatype error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '>'");
    }
  | TOKENTYPE_KEYWORD_LIST '<' error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    }
  | TOKENTYPE_KEYWORD_LIST error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected list specification");
    }
  | TOKENTYPE_KEYWORD_LIST TOKENTYPE_KEYWORD_OF datatype
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"datatype list\n");
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type          = DATA_TYPE_LIST;
      $$.irDatatype.list.datatype = SC_IR_NEW($3.irDatatype);
    }
  | TOKENTYPE_KEYWORD_LIST TOKENTYPE_KEYWORD_OF error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");

    }
  | TOKENTYPE_KEYWORD_SET '<' datatype '>'
    {
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type         = DATA_TYPE_LIST;
      $$.irDatatype.set.datatype = SC_IR_NEW($3.irDatatype);
    }
  | TOKENTYPE_KEYWORD_SET '<' datatype error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '>'");
    }
  | TOKENTYPE_KEYWORD_SET '<' error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    }
  | TOKENTYPE_KEYWORD_SET error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected set specification");
    }
  | TOKENTYPE_KEYWORD_SET TOKENTYPE_KEYWORD_OF datatype
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"datatype set\n");
      INIT_IR($$.irDatatype,IR_TYPE_DATATYPE,@1);
      $$.irDatatype.type         = DATA_TYPE_LIST;
      $$.irDatatype.set.datatype = SC_IR_NEW($3.irDatatype);
    }
  | TOKENTYPE_KEYWORD_SET TOKENTYPE_KEYWORD_OF error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    }
  ;

// -------------------------------------------------------
argument
  : datatype identifier
    {
      INIT_IR($$.irArgument,IR_TYPE_ARGUMENT,@1);
      $$.irArgument.type           = ARGUMENT_TYPE_VALUE;
      $$.irArgument.value.name     = $2.irIdentifier.name; 
      $$.irArgument.value.datatype = $1.irDatatype; 
    }
  ;

// -------------------------------------------------------
argumentList
  : argumentList ',' argument
    {
      $$.irArgumentList = $1.irArgumentList;
      List_add(&$$.irArgumentList, (Node*)SC_IR_NEW_NODE($3.irArgument,IR_TYPE_ARGUMENT,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | argument
    {
      List_init(&$$.irArgumentList);
      List_add(&$$.irArgumentList, (Node*)SC_IR_NEW_NODE($1.irArgument,IR_TYPE_ARGUMENT,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  ;

// -------------------------------------------------------
functionArguments
  : argumentList
  | argumentList ',' TOKENTYPE_ELLIPSE
    {
      IRArgument irArgument;

      irArgument.type = ARGUMENT_TYPE_ELLIPSE;
      List_add(&$$.irArgumentList, (Node*)SC_IR_NEW_NODE(irArgument,IR_TYPE_ARGUMENT,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | /* empty */
    {
      List_init(&$$.irArgumentList);
    }
  ;

// -------------------------------------------------------
block
  : ';'
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }
  | statement
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }
  | '{' code '}'
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }
  ;

// -------------------------------------------------------
parameter
  : expression
    {
      INIT_IR($$.irParameter,IR_TYPE_PARAMETER,@1);
      $$.irParameter.expression = $1.irExpression;
    }
  ;

// -------------------------------------------------------
parameterList
  : parameterList ',' parameter
    {
      $$.irParameterList = $1.irParameterList;
      List_add(&$$.irParameterList, (Node*)SC_IR_NEW_NODE($3.irParameter,IR_TYPE_PARAMETER,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  | parameter
    {
      List_init(&$$.irParameterList);
      List_add(&$$.irParameterList, (Node*)SC_IR_NEW_NODE($1.irParameter,IR_TYPE_PARAMETER,CURRENT_FILE,@1.first_line,@1.first_column));
    }
  ;

// -------------------------------------------------------
functionParameters
  : parameterList
    {
      $$.irParameterList = $1.irParameterList;
    }
  | /* empty */
    {
      List_init(&$$.irParameterList);
    }
  ;

// -------------------------------------------------------
functionCall
  : identifier '(' functionParameters ')'
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"function call (%d parameters)\n", $3.irParameterList.count);

      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type                             = EXPRESSION_TYPE_FUNCTION_CALL;
      $$.irExpression.functionCall.functionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irExpression.functionCall.name                = $1.irIdentifier.name;
      $$.irExpression.functionCall.declaration         = NULL;
      $$.irExpression.functionCall.parameterList       = $3.irParameterList;
    }
  | identifier '(' functionParameters error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ')'");
    }
  | identifier '(' error
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected parameter list");
    }
  ;


// -------------------------------------------------------
assignment
  : identifier '=' expression
    {
      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type                           = EXPRESSION_TYPE_ASSIGNMENT;
      $$.irExpression.assignment.functionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irExpression.assignment.name                = $1.irIdentifier.name;
      $$.irExpression.assignment.declaration         = NULL;
      $$.irExpression.assignment.operator            = OPERATOR_TYPE_NONE;
      $$.irExpression.assignment.expression          = (IRExpression*)SC_IR_NEW($3.irExpression);
    }
  | identifier '=' 
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected expression");
    }
  ;

// -------------------------------------------------------
expression
  : '+' expression
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }
  | '-' expression
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }
  | expression '+' expression
    {
      IRDatatype datatype1,datatype2;

      datatype1 = scDatatype_getExpressionDatatype(&$1.irExpression);
//fprintf(stderr,"%s,%d: %s\n",__FILE__,__LINE__,scIR_getBaseDatatype(&datatype1));
      datatype2 = scDatatype_getExpressionDatatype(&$3.irExpression);
//fprintf(stderr,"%s,%d: %s\n",__FILE__,__LINE__,scIR_getBaseDatatype(&datatype2));
      if (!scDatatype_checkCompatible(&datatype1,&datatype2))
      {
        PARSE_ERROR(@1.line,@1.first_line,@1.last_column,"incompatible datatypes for '+'");
      }
      SC_IR_DONE(datatype2);
      SC_IR_DONE(datatype1);

      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type                       = EXPRESSION_TYPE_BINARY_OPERATOR;
      $$.irExpression.binaryOperator.operator    = OPERATOR_TYPE_ADD;
      $$.irExpression.binaryOperator.expression1 = (IRExpression*)SC_IR_NEW($1.irExpression);
      $$.irExpression.binaryOperator.expression2 = (IRExpression*)SC_IR_NEW($3.irExpression);
    }
  | expression '-' expression
    {
      IRDatatype datatype1,datatype2;

      datatype1 = scDatatype_getExpressionDatatype(&$1.irExpression);
      datatype2 = scDatatype_getExpressionDatatype(&$3.irExpression);
      if (!scDatatype_checkCompatible(&datatype1,&datatype2))
      {
        PARSE_ERROR(@1.line,@1.first_line,@1.last_column,"incompatible datatypes for '+'");
      }
      SC_IR_DONE(datatype2);
      SC_IR_DONE(datatype1);

      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type                       = EXPRESSION_TYPE_BINARY_OPERATOR;
      $$.irExpression.binaryOperator.operator    = OPERATOR_TYPE_SUB;
      $$.irExpression.binaryOperator.expression1 = (IRExpression*)SC_IR_NEW($1.irExpression);
      $$.irExpression.binaryOperator.expression2 = (IRExpression*)SC_IR_NEW($3.irExpression);
    }
  | expression '*' expression
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }
  | expression '/' expression
    {
      IRDatatype datatype1,datatype2;

      datatype1 = scDatatype_getExpressionDatatype(&$1.irExpression);
      datatype2 = scDatatype_getExpressionDatatype(&$3.irExpression);
      if (!scDatatype_checkCompatible(&datatype1,&datatype2))
      {
        PARSE_ERROR(@1.line,@1.first_line,@1.last_column,"incompatible datatypes for '+'");
      }
      SC_IR_DONE(datatype2);
      SC_IR_DONE(datatype1);

      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type                       = EXPRESSION_TYPE_BINARY_OPERATOR;
      $$.irExpression.binaryOperator.operator    = OPERATOR_TYPE_DIV;
      $$.irExpression.binaryOperator.expression1 = (IRExpression*)SC_IR_NEW($1.irExpression);
      $$.irExpression.binaryOperator.expression2 = (IRExpression*)SC_IR_NEW($3.irExpression);
    }
  | expression '%' expression
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }
  | '(' expression ')'
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    }
  | assignment
    {
      $$.irExpression = $1.irExpression;
    }
  | functionCall
    {
      $$.irExpression = $1.irExpression;
    }
  | identifier
    {
      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type                         = EXPRESSION_TYPE_IDENTIFIER;
      $$.irExpression.variable.functionDeclaration = CURRENT_FUNCTION_DECLARATION;
      $$.irExpression.variable.name                = $1.irIdentifier.name;
      $$.irExpression.variable.declaration         = NULL;
    }
  | TOKENTYPE_INTEGER
    {
      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type = EXPRESSION_TYPE_INTEGER;
      $$.irExpression.i    = $1.i;
    }
  | TOKENTYPE_FLOAT
    {
      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type = EXPRESSION_TYPE_FLOAT;
      $$.irExpression.d    = $1.d;
    }
  | TOKENTYPE_STRING
    {
      INIT_IR($$.irExpression,IR_TYPE_EXPRESSION,@1);
      $$.irExpression.type = EXPRESSION_TYPE_STRING;
      $$.irExpression.s    = String_newCString($1.s);
    }
  ;

// -------------------------------------------------------
constExpression
  : '+' constExpression
    {
      switch ($1.irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          $$.irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          $$.irConstExpression.i    = $1.irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          $$.irConstExpression.type = TOKENTYPE_FLOAT;
          $$.irConstExpression.d    = $1.irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '+' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",$1.irConstExpression.type);
          break;
      }
    }
  | '-' constExpression
    {
      switch ($1.irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          $$.irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          $$.irConstExpression.i    = -$1.irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          $$.irConstExpression.type = TOKENTYPE_FLOAT;
          $$.irConstExpression.d    = -$1.irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '-' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",$1.irConstExpression.type);
          break;
      }
    }
  | constExpression '+' constExpression
    {
      promoteConstExpressionType(&$1.irConstExpression,$3.irConstExpression.type);
      promoteConstExpressionType(&$3.irConstExpression,$1.irConstExpression.type);

      if ($1.irExpression.type != $3.irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ($1.irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          $$.irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          $$.irConstExpression.i    = $1.irConstExpression.i + $3.irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          $$.irConstExpression.type = TOKENTYPE_FLOAT;
          $$.irConstExpression.d    = $1.irConstExpression.d + $3.irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          $$.irConstExpression.type = TOKENTYPE_STRING;
          $$.irConstExpression.s    = String_append(String_copy($1.irConstExpression.s),$3.irConstExpression.s);
          String_delete($1.irConstExpression.s);
          String_delete($2.irConstExpression.s);
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",$1.irConstExpression.type);
          break;
      }
    }
  | constExpression '-' constExpression
    {
      promoteConstExpressionType(&$1.irConstExpression,$3.irConstExpression.type);
      promoteConstExpressionType(&$3.irConstExpression,$1.irConstExpression.type);

      if ($1.irExpression.type != $3.irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ($1.irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          $$.irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          $$.irConstExpression.i    = $1.irConstExpression.i - $3.irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          $$.irConstExpression.type = TOKENTYPE_FLOAT;
          $$.irConstExpression.d    = $1.irConstExpression.d - $3.irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '-' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",$1.irConstExpression.type);
          break;
      }
    }
  | constExpression '*' constExpression
    {
      promoteConstExpressionType(&$1.irConstExpression,$3.irConstExpression.type);
      promoteConstExpressionType(&$3.irConstExpression,$1.irConstExpression.type);

      if ($1.irExpression.type != $3.irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ($1.irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          $$.irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          $$.irConstExpression.i    = $1.irConstExpression.i * $3.irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          $$.irConstExpression.type = TOKENTYPE_FLOAT;
          $$.irConstExpression.d    = $1.irConstExpression.d * $3.irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '*' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",$1.irConstExpression.type);
          break;
      }
    }
  | constExpression '/' constExpression
    {
      promoteConstExpressionType(&$1.irConstExpression,$3.irConstExpression.type);
      promoteConstExpressionType(&$3.irConstExpression,$1.irConstExpression.type);

      if ($1.irExpression.type != $3.irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ($1.irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          $$.irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          $$.irConstExpression.i    = $1.irConstExpression.i / $3.irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          $$.irConstExpression.type = TOKENTYPE_FLOAT;
          $$.irConstExpression.d    = $1.irConstExpression.d / $3.irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '/' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",$1.irConstExpression.type);
          break;
      }
    }
  | constExpression '%' constExpression
    {
      promoteConstExpressionType(&$1.irConstExpression,$3.irConstExpression.type);
      promoteConstExpressionType(&$3.irConstExpression,$1.irConstExpression.type);

      if ($1.irExpression.type != $3.irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ($1.irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          $$.irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          $$.irConstExpression.i    = $1.irConstExpression.i % $3.irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          $$.irConstExpression.type = TOKENTYPE_FLOAT;
          $$.irConstExpression.d    = fmod($1.irConstExpression.d,$3.irConstExpression.d);
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '%' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",$1.irConstExpression.type);
          break;
      }
    }
  | '(' constExpression ')'
    {
      $$.irConstExpression = $2.irConstExpression;
    }
  | TOKENTYPE_INTEGER
    {
      INIT_IR($$.irConstExpression,IR_TYPE_EXPRESSION,@1);
      $$.irConstExpression.type = EXPRESSION_TYPE_INTEGER;
      $$.irConstExpression.i    = $1.i;
    }
  | TOKENTYPE_FLOAT
    {
      INIT_IR($$.irConstExpression,IR_TYPE_EXPRESSION,@1);
      $$.irConstExpression.type = EXPRESSION_TYPE_FLOAT;
      $$.irConstExpression.d    = $1.d;
    }
  | TOKENTYPE_STRING
    {
      INIT_IR($$.irConstExpression,IR_TYPE_EXPRESSION,@1);
      $$.irConstExpression.type = EXPRESSION_TYPE_STRING;
      $$.irConstExpression.s    = String_newCString($1.s);
    }
  ;

%%

/*---------------------------------------------------------------------*/

/***********************************************************************\
* Name   : readDataFromFile
* Purpose: read data from string
* Input  : -
* Output : -
* Return : -
* Notes  : -
\***********************************************************************/

LOCAL int readDataFromFile(void *userData, void *buffer, unsigned int length)
{
  int readLength;
  int n;

  assert(userData!=NULL);
  assert(buffer!=NULL);

  if (!feof((FILE*)userData))
  {
    readLength=0;
    do
    {
      n=fread((char*)buffer+readLength,1,(int)length-readLength,(FILE*)userData);
      if (n>0)
      {
        readLength+=n;
      }
    }
    while ((n>0) && (readLength<(int)length) && !feof((FILE*)userData));
    return readLength;
  }
  else
  {
    return -1;
  }
}

/***********************************************************************\
* Name   : scCodeParser_parseFile
* Purpose: parse file
* Input  : filename  - filename
*          quietFlag - TRUE to surpress output
* Output : -
* Return : TRUE if no error occurred, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool scCodeParser_parseFile(const char *filename, IRList *irList, bool quietFlag)
{
  FILE *file;

  assert(filename != NULL);
  assert(irList != NULL);

  // init variables
  code_debug = (scCodeParser_debug)?1:0;
  moduleName = NULL;
  contextStackIndex = 0;
  List_init(&CURRENT_IRLIST);

  // open file
  file = fopen((char*)filename,"r");
  if (file == NULL)
  {
    fprintf(stderr,"ERROR: Can not open source file '%s' (error: %s)\n",
            filename,
            strerror(errno)
           );
    return FALSE;
  }
  CURRENT_FILE = scFiles_add(filename);

  // init scanner
  if (!scCodeScanner_init(readDataFromFile))
  {
    fclose(file);
    return FALSE;
  }

  // parse program, create intermediate representation
  contextStackIndex = 0;
  if (code_parse(file,quietFlag)!=0)
  {
    scCodeScanner_done();
    fclose(file);
    return FALSE;
  }

  // deinit scanner
  scCodeScanner_done();

  // close file
  fclose(file);

  // get intermediate representation
  List_init(irList);
  irList->moduleName = moduleName;
  List_move(&CURRENT_IRLIST,irList,LIST_START,LIST_END,NULL);

  // free resources
  do
  {
    List_done(&CURRENT_IRLIST,(NodeFreeFunction)scIR_deleteNode, NULL);
    contextStackIndex--;
  }
  while (contextStackIndex > 0);

  return TRUE;
}

#ifdef __cplusplus
 }
#endif
