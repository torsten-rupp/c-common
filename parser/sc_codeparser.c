/***********************************************************************\
*
* Contents: code parser functions
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
#include "sc_files.h"
#include "sc_ir.h"
#include "sc_datatype.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/

/***************************** Variables *******************************/
bool scCodeParser_debug = FALSE;

/****************************** Macros *********************************/

/***************************** Functions *******************************/

#ifdef __cplusplus
 extern "C" {
#endif

/***********************************************************************\
* Name   : resolveReferencesExpression
* Purpose: resolve references in expression
* Input  : userData     - user data
*          irExpression - expression
* Output : -
* Return : TRUE if no error, FALSE otherwise
* Notes  : -
\***********************************************************************/

LOCAL bool resolveReferencesExpression(void *userData, IRExpression *irExpression)
{
  IRList *irList = (IRList*)userData;

  assert(irExpression != NULL);

  switch (irExpression->type)
  {
    case EXPRESSION_TYPE_INTEGER:
      break;
    case EXPRESSION_TYPE_FLOAT:
      break;
    case EXPRESSION_TYPE_STRING:
      break;
    case EXPRESSION_TYPE_UNARY_OPERATOR:
      break;
    case EXPRESSION_TYPE_BINARY_OPERATOR:
      break;
    case EXPRESSION_TYPE_ASSIGNMENT:
      {
        dprintf(DEBUGGROUP_CODE_PARSER,2,"resolve expression '%s'\n",scIR_getExpressionType(irExpression));
      }
      break;
    case EXPRESSION_TYPE_FUNCTION_CALL:
      {
        IRFunctionDeclaration *irFunctionDeclaration;

        dprintf(DEBUGGROUP_CODE_PARSER,2,"resolve expression '%s'\n",scIR_getExpressionType(irExpression));

        irFunctionDeclaration = scIR_findFunctionDeclaration(irList,
                                                             irExpression->functionCall.functionDeclaration,
                                                             irExpression->functionCall.name
                                                            );
        if (irFunctionDeclaration == NULL)
        {
          scError_expression(irExpression,"cannot find function '%s'",String_cString(irExpression->functionCall.name));
          return FALSE;
        }
        irExpression->functionCall.declaration = irFunctionDeclaration;
      }
      break;
    case EXPRESSION_TYPE_IDENTIFIER:
      {
        IRVariableDeclaration *irVariableDeclaration;

        dprintf(DEBUGGROUP_CODE_PARSER,2,"resolve expression '%s'\n",scIR_getExpressionType(irExpression));

        irVariableDeclaration = scIR_findVariableDeclaration(irList,
                                                             irExpression->variable.functionDeclaration,
                                                             irExpression->variable.name
                                                            );
        if (irVariableDeclaration == NULL)
        {
          scError_expression(irExpression,"cannot find variable '%s'",String_cString(irExpression->variable.name));
          return FALSE;
        }
        irExpression->variable.declaration = irVariableDeclaration;
      }
      break;
    default:
      HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",irExpression->type);
      break;
  }

  return TRUE;
}

/***********************************************************************\
* Name   : scCodeParser_resolveReferences
* Purpose: resolve references
* Input  : irList - intermediate representation list
* Output : -
* Return : TRUE if no error occurred, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool scCodeParser_resolveReferences(IRList *irList)
 {
  IRIterator irIterator;
  bool       result;

  scIR_initIterator(&irIterator);
  irIterator.expression = resolveReferencesExpression;
  result = scIR_iterateList(&irIterator,ITERATOR_MODE_ANY,irList,NULL,irList);
  scIR_doneIterator(&irIterator);

  return result;
 }

LOCAL bool checkDuplicateIdentifier(IRList *irList, IRFunctionDeclaration *irFunctionDeclaration, String name)
{
  IRFindIdentifierInfo  irFindIdentifierInfo;
  IRNode                *irNode1,*irNode2;
  IRFunctionDeclaration *functionDeclaration1,*functionDeclaration2;
  
  assert(irList != NULL);
  assert(name != NULL);

  irNode1 = scIR_findFirstIdentifier(&irFindIdentifierInfo,
                                     irList,
                                     irFunctionDeclaration,
                                     name,
                                     SC_IR_TYPE_MASK(FUNCTION_DECLARATION) |
                                     SC_IR_TYPE_MASK(CONSTANT_DECLARATION) |
                                     SC_IR_TYPE_MASK(DATATYPE_DECLARATION) |
                                     SC_IR_TYPE_MASK(VARIABLE_DECLARATION)
                                    );
  if (irNode1 != NULL)
  {
    switch (irNode1->type)
    {
      case IR_TYPE_FUNCTION_DECLARATION:
        functionDeclaration1 = ((IRFunctionDeclaration*)irNode1->data)->parentFunctionDeclaration;
        break;
      case IR_TYPE_CONSTANT_DECLARATION:
        functionDeclaration1 = ((IRConstantDeclaration*)irNode1->data)->parentFunctionDeclaration;
        break;
      case IR_TYPE_DATATYPE_DECLARATION:
        functionDeclaration1 = ((IRDatatypeDeclaration*)irNode1->data)->parentFunctionDeclaration;
        break;
      case IR_TYPE_VARIABLE_DECLARATION:
        functionDeclaration1 = ((IRVariableDeclaration*)irNode1->data)->parentFunctionDeclaration;
        break;
      default:
        HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE();
        break;
    }
  }
  else
  {
    functionDeclaration1 = NULL;
  }
  irNode2 = scIR_findNextIdentifier(&irFindIdentifierInfo);
  if (irNode2 != NULL)
  {
    switch (irNode2->type)
    {
      case IR_TYPE_FUNCTION_DECLARATION:
        functionDeclaration2 = ((IRFunctionDeclaration*)irNode2->data)->parentFunctionDeclaration;
        break;
      case IR_TYPE_CONSTANT_DECLARATION:
        functionDeclaration2 = ((IRConstantDeclaration*)irNode2->data)->parentFunctionDeclaration;
        break;
      case IR_TYPE_DATATYPE_DECLARATION:
        functionDeclaration2 = ((IRDatatypeDeclaration*)irNode2->data)->parentFunctionDeclaration;
        break;
      case IR_TYPE_VARIABLE_DECLARATION:
        functionDeclaration2 = ((IRVariableDeclaration*)irNode2->data)->parentFunctionDeclaration;
        break;
      default:
        HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE();
        break;
    }
  }
  else
  {
    functionDeclaration2 = NULL;
  }

  return ((irNode1 != NULL) && (irNode2 != NULL) && (functionDeclaration1 == functionDeclaration2));
}

/***********************************************************************\
* Name   : verifyConstantDeclaration
* Purpose: verify constant declarations
* Input  : userData              - user data
*          irConstantDeclaration - constant declaration
* Output : -
* Return : TRUE if no error, FALSE otherwise
* Notes  : -
\***********************************************************************/

LOCAL bool verifyConstantDeclaration(void *userData, IRFunctionDeclaration *irParentFunctionDeclaration, IRConstantDeclaration *irConstantDeclaration)
{
  IRFindIdentifierInfo irFindIdentifierInfo;
  IRNode               *irNode;
  ulong mask;
  
  IRList *irList = (IRList*)userData;

  assert(irConstantDeclaration != NULL);

  if (checkDuplicateIdentifier(irList,irParentFunctionDeclaration,irConstantDeclaration->name))
  {
    scError_constantDeclaration(irConstantDeclaration,"duplicate identifier '%s'",String_cString(irConstantDeclaration->name));
    return FALSE;
  }

  return TRUE;
}

/***********************************************************************\
* Name   : verifyExpression
* Purpose: verify expression (datatype, operator)
* Input  : userData     - user data
*          irExpression - expression
* Output : -
* Return : TRUE if no error, FALSE otherwise
* Notes  : -
\***********************************************************************/

LOCAL bool verifyExpression(void *userData, IRExpression *irExpression)
{
  assert(irExpression != NULL);

  dprintf(DEBUGGROUP_CODE_PARSER,2,"verify '%s' at %s, %ld\n",scIR_getExpressionType(irExpression),String_cString(irExpression->irHeader.fileNode->name),irExpression->irHeader.lineNb);
  switch (irExpression->type)
  {
    case EXPRESSION_TYPE_INTEGER:
      break;
    case EXPRESSION_TYPE_FLOAT:
      break;
    case EXPRESSION_TYPE_STRING:
      break;
    case EXPRESSION_TYPE_UNARY_OPERATOR:
      break;
    case EXPRESSION_TYPE_BINARY_OPERATOR:
      break;
    case EXPRESSION_TYPE_ASSIGNMENT:
      {
      }
      break;
    case EXPRESSION_TYPE_FUNCTION_CALL:
      {
        IRNode     *irArgument;
        IRNode     *irParameter;
        uint       n;
        IRDatatype parameterDatatype;

        assert(irExpression->functionCall.declaration != NULL);

        /* check if datatype of function arguments/parameters are compatible */
        irArgument  = irExpression->functionCall.declaration->argumentList.head;
        irParameter = irExpression->functionCall.parameterList.head;
        n = 1;
        while ((irArgument != NULL) && (irParameter != NULL))
        {
          assert(irArgument->type == IR_TYPE_ARGUMENT);
          assert(irParameter->type == IR_TYPE_PARAMETER);
//scIR_printNode(irArgument);
//scIR_printNode(irParameter);

          /* check if datatype is compatible */
          parameterDatatype = scDatatype_getExpressionDatatype(&((IRParameter*)irParameter->data)->expression);
          assert(parameterDatatype.type != DATA_TYPE_NONE);
          if ((((IRArgument*)irArgument->data)->type == ARGUMENT_TYPE_VALUE) &&
              !scDatatype_checkCompatible(&((IRArgument*)irArgument->data)->value.datatype,
                                          &parameterDatatype
                                         )
             )
          {
            String s1,s2;

            s1 = scDatatype_datatypeToErrorString(String_new(),&((IRArgument*)irArgument->data)->value.datatype);
            s2 = scDatatype_datatypeToErrorString(String_new(),&parameterDatatype);
            scError_expression(irExpression,"incompatible types for parameter %d: got '%s', but expected '%s'",n,String_cString(s2),String_cString(s1));
            String_delete(s2);
            String_delete(s1);

            scDatatype_done(parameterDatatype);
  
            return FALSE;
          }
          scDatatype_done(parameterDatatype);

          /* next argument/parameter */
          irArgument  = irArgument->next;
          irParameter = irParameter->next;
          n++;
        }
        if      ((irArgument != NULL) && (irParameter == NULL) && (((IRArgument*)irArgument->data)->type != ARGUMENT_TYPE_ELLIPSE))
        {
          scError_expression(irExpression,"not enough parameters to function call");
          return FALSE;
        }
        else if ((irArgument == NULL) && (irParameter != NULL))
        {
          scError_expression(irExpression,"to many parameters to function call");
          return FALSE;
        }
      }
      break;
    case EXPRESSION_TYPE_IDENTIFIER:
      break;
    default:
      HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",irExpression->type);
      break;
  }

  return TRUE;
}

/***********************************************************************\
* Name   : scCodeParser_verify
* Purpose: verify
* Input  : irList - intermediate representation list
* Output : -
* Return : TRUE if no error occurred, FALSE otherwise
* Notes  : -
\***********************************************************************/

bool scCodeParser_verify(IRList *irList)
 {
  IRIterator irIterator;
  bool       result;

  /* verify declarations */
  scIR_initIterator(&irIterator);
  irIterator.constantDeclaration = verifyConstantDeclaration;
//  irIterator.datatypeDeclaration = verifyDatatypeDeclaration;
//  irIterator.variableDeclaration = verifyVariableDeclaration;
//  irIterator.functionDeclaration = verifyFunctionDeclaration;
//  irIterator.argument            = verifyArgument;
  result = scIR_iterateList(&irIterator,ITERATOR_MODE_ANY,irList,NULL,irList);
  scIR_doneIterator(&irIterator);
  if (result != TRUE) return FALSE;

  /* verify expression */
  scIR_initIterator(&irIterator);
  irIterator.expression = verifyExpression;
  result = scIR_iterateList(&irIterator,ITERATOR_MODE_TREE,irList,NULL,irList);
  scIR_doneIterator(&irIterator);
  if (result != TRUE) return FALSE;

  return TRUE;
 }

#ifdef __cplusplus
 }
#endif
