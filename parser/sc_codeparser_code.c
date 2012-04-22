/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse code_parse
#define yylex   code_lex
#define yyerror code_error
#define yylval  code_lval
#define yychar  code_char
#define yydebug code_debug
#define yynerrs code_nerrs
#define yylloc code_lloc

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKENTYPE_KEYWORD_MODULE = 258,
     TOKENTYPE_KEYWORD_CONST = 259,
     TOKENTYPE_KEYWORD_VAR = 260,
     TOKENTYPE_KEYWORD_TYPEDEF = 261,
     TOKENTYPE_KEYWORD_VOID = 262,
     TOKENTYPE_KEYWORD_CHAR = 263,
     TOKENTYPE_KEYWORD_SHORT = 264,
     TOKENTYPE_KEYWORD_INT = 265,
     TOKENTYPE_KEYWORD_LONG = 266,
     TOKENTYPE_KEYWORD_LONGLONG = 267,
     TOKENTYPE_KEYWORD_FLOAT = 268,
     TOKENTYPE_KEYWORD_DOUBLE = 269,
     TOKENTYPE_KEYWORD_STRING = 270,
     TOKENTYPE_KEYWORD_ARRAY = 271,
     TOKENTYPE_KEYWORD_LIST = 272,
     TOKENTYPE_KEYWORD_SET = 273,
     TOKENTYPE_KEYWORD_OF = 274,
     TOKENTYPE_KEYWORD_STRUCT = 275,
     TOKENTYPE_KEYWORD_EXTERN = 276,
     TOKENTYPE_KEYWORD_INLINE = 277,
     TOKENTYPE_KEYWORD_PUBLIC = 278,
     TOKENTYPE_KEYWORD_RETURN = 279,
     TOKENTYPE_IDENTIFIER = 280,
     TOKENTYPE_INTEGER = 281,
     TOKENTYPE_FLOAT = 282,
     TOKENTYPE_STRING = 283,
     TOKENTYPE_ELLIPSE = 284
   };
#endif
/* Tokens.  */
#define TOKENTYPE_KEYWORD_MODULE 258
#define TOKENTYPE_KEYWORD_CONST 259
#define TOKENTYPE_KEYWORD_VAR 260
#define TOKENTYPE_KEYWORD_TYPEDEF 261
#define TOKENTYPE_KEYWORD_VOID 262
#define TOKENTYPE_KEYWORD_CHAR 263
#define TOKENTYPE_KEYWORD_SHORT 264
#define TOKENTYPE_KEYWORD_INT 265
#define TOKENTYPE_KEYWORD_LONG 266
#define TOKENTYPE_KEYWORD_LONGLONG 267
#define TOKENTYPE_KEYWORD_FLOAT 268
#define TOKENTYPE_KEYWORD_DOUBLE 269
#define TOKENTYPE_KEYWORD_STRING 270
#define TOKENTYPE_KEYWORD_ARRAY 271
#define TOKENTYPE_KEYWORD_LIST 272
#define TOKENTYPE_KEYWORD_SET 273
#define TOKENTYPE_KEYWORD_OF 274
#define TOKENTYPE_KEYWORD_STRUCT 275
#define TOKENTYPE_KEYWORD_EXTERN 276
#define TOKENTYPE_KEYWORD_INLINE 277
#define TOKENTYPE_KEYWORD_PUBLIC 278
#define TOKENTYPE_KEYWORD_RETURN 279
#define TOKENTYPE_IDENTIFIER 280
#define TOKENTYPE_INTEGER 281
#define TOKENTYPE_FLOAT 282
#define TOKENTYPE_STRING 283
#define TOKENTYPE_ELLIPSE 284




/* Copy the first part of user declarations.  */
#line 1 "sc_codeparser.y"

/***********************************************************************\
*
* $Source: /home/torsten/cvs/sc/sc_codeparser.y,v $
* $Revision$
* $Author$
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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 381 "sc_codeparser_code.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   721

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  157
/* YYNRULES -- Number of states.  */
#define YYNSTATES  244

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   284

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    34,     2,     2,
      38,    39,    32,    30,    37,    31,     2,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    36,
      45,    40,    46,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    44,    35,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,     2,    42,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,     9,    15,    19,    23,
      26,    27,    28,    33,    34,    39,    40,    45,    46,    51,
      52,    57,    58,    62,    63,    67,    68,    72,    73,    77,
      78,    82,    83,    85,    89,    93,    96,    99,   102,   106,
     108,   111,   117,   122,   126,   129,   133,   138,   141,   146,
     150,   153,   154,   160,   163,   165,   169,   171,   176,   181,
     186,   191,   196,   201,   206,   211,   214,   217,   221,   224,
     226,   230,   233,   235,   236,   240,   242,   247,   251,   255,
     258,   261,   263,   265,   267,   269,   271,   273,   275,   277,
     279,   281,   283,   286,   289,   294,   299,   303,   306,   312,
     318,   323,   327,   330,   335,   340,   345,   350,   354,   357,
     361,   365,   370,   375,   379,   382,   386,   390,   393,   397,
     399,   401,   405,   406,   408,   412,   414,   416,   417,   422,
     427,   431,   435,   438,   441,   444,   448,   452,   456,   460,
     464,   468,   470,   472,   474,   476,   478,   480,   483,   486,
     490,   494,   498,   502,   506,   510,   512,   514
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    49,    -1,    -1,     1,    -1,    -1,     3,
      81,    36,    50,    52,    -1,     3,    81,     1,    -1,     3,
       1,    36,    -1,    51,    23,    -1,    -1,    -1,    51,    68,
      53,    52,    -1,    -1,    51,    69,    54,    52,    -1,    -1,
      51,    70,    55,    52,    -1,    -1,    51,    71,    56,    52,
      -1,    -1,    51,    73,    57,    52,    -1,    -1,    76,    58,
      52,    -1,    -1,    77,    59,    52,    -1,    -1,    78,    60,
      52,    -1,    -1,    79,    61,    52,    -1,    -1,    80,    62,
      52,    -1,    -1,     1,    -1,     4,    86,    81,    -1,     4,
      86,     1,    -1,     4,     1,    -1,     6,    86,    -1,     6,
       1,    -1,    65,    37,    81,    -1,    81,    -1,    86,    65,
      -1,    86,    81,    38,    89,    39,    -1,    63,    40,    95,
      36,    -1,    63,    40,     1,    -1,    63,     1,    -1,    64,
      81,    36,    -1,    66,    40,    95,    36,    -1,    66,    36,
      -1,    66,    40,    95,     1,    -1,    66,    40,     1,    -1,
      66,     1,    -1,    -1,    67,    41,    72,    52,    42,    -1,
      67,    36,    -1,    28,    -1,    75,    37,    74,    -1,    74,
      -1,    21,    75,    63,    36,    -1,    21,    75,    63,     1,
      -1,    21,    75,    64,    36,    -1,    21,    75,    64,     1,
      -1,    21,    75,    66,    36,    -1,    21,    75,    66,     1,
      -1,    21,    75,    67,    36,    -1,    21,    75,    67,     1,
      -1,    94,    36,    -1,    93,    36,    -1,    24,    95,    36,
      -1,    24,    36,    -1,    25,    -1,    86,    81,    36,    -1,
      83,    82,    -1,    82,    -1,    -1,    84,    37,    96,    -1,
      96,    -1,    85,    43,    84,    44,    -1,    85,    43,    44,
      -1,    43,    84,    44,    -1,    43,    44,    -1,    43,     1,
      -1,     1,    -1,     7,    -1,     8,    -1,     9,    -1,    10,
      -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,    15,
      -1,    81,    -1,    86,    32,    -1,    86,    85,    -1,    20,
      41,    83,    42,    -1,    20,    41,    83,     1,    -1,    20,
      41,     1,    -1,    20,     1,    -1,    16,    85,    45,    86,
      46,    -1,    16,    85,    45,    86,     1,    -1,    16,    85,
      45,     1,    -1,    16,    85,     1,    -1,    16,     1,    -1,
      16,    85,    19,    86,    -1,    16,    85,    19,     1,    -1,
      17,    45,    86,    46,    -1,    17,    45,    86,     1,    -1,
      17,    45,     1,    -1,    17,     1,    -1,    17,    19,    86,
      -1,    17,    19,     1,    -1,    18,    45,    86,    46,    -1,
      18,    45,    86,     1,    -1,    18,    45,     1,    -1,    18,
       1,    -1,    18,    19,    86,    -1,    18,    19,     1,    -1,
      86,    81,    -1,    88,    37,    87,    -1,    87,    -1,    88,
      -1,    88,    37,    29,    -1,    -1,    95,    -1,    91,    37,
      90,    -1,    90,    -1,    91,    -1,    -1,    81,    38,    92,
      39,    -1,    81,    38,    92,     1,    -1,    81,    38,     1,
      -1,    81,    40,    95,    -1,    81,    40,    -1,    30,    95,
      -1,    31,    95,    -1,    95,    30,    95,    -1,    95,    31,
      95,    -1,    95,    32,    95,    -1,    95,    33,    95,    -1,
      95,    34,    95,    -1,    38,    95,    39,    -1,    94,    -1,
      93,    -1,    81,    -1,    26,    -1,    27,    -1,    28,    -1,
      30,    96,    -1,    31,    96,    -1,    96,    30,    96,    -1,
      96,    31,    96,    -1,    96,    32,    96,    -1,    96,    33,
      96,    -1,    96,    34,    96,    -1,    38,    96,    39,    -1,
      26,    -1,    27,    -1,    28,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   256,   256,   257,   259,   268,   267,   274,   278,   287,
     292,   300,   299,   311,   310,   322,   321,   333,   332,   347,
     346,   358,   357,   368,   367,   378,   377,   388,   387,   398,
     397,   403,   404,   412,   422,   426,   433,   442,   449,   454,
     462,   475,   490,   497,   501,   508,   518,   525,   532,   536,
     540,   548,   547,   578,   590,   597,   602,   610,   617,   624,
     631,   638,   645,   652,   659,   667,   673,   679,   686,   697,
     705,   714,   719,   725,   731,   744,   760,   765,   773,   777,
     785,   789,   796,   801,   806,   811,   816,   821,   826,   831,
     836,   841,   847,   853,   858,   865,   869,   873,   878,   885,
     889,   893,   897,   901,   908,   913,   919,   923,   927,   931,
     938,   943,   949,   953,   957,   961,   968,   976,   987,   992,
    1001,  1002,  1010,  1033,  1042,  1047,  1056,  1061,  1068,  1079,
    1083,  1092,  1102,  1110,  1114,  1118,  1139,  1158,  1162,  1181,
    1185,  1189,  1193,  1197,  1205,  1211,  1217,  1227,  1247,  1267,
    1298,  1326,  1354,  1382,  1410,  1414,  1420,  1426
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOKENTYPE_KEYWORD_MODULE",
  "TOKENTYPE_KEYWORD_CONST", "TOKENTYPE_KEYWORD_VAR",
  "TOKENTYPE_KEYWORD_TYPEDEF", "TOKENTYPE_KEYWORD_VOID",
  "TOKENTYPE_KEYWORD_CHAR", "TOKENTYPE_KEYWORD_SHORT",
  "TOKENTYPE_KEYWORD_INT", "TOKENTYPE_KEYWORD_LONG",
  "TOKENTYPE_KEYWORD_LONGLONG", "TOKENTYPE_KEYWORD_FLOAT",
  "TOKENTYPE_KEYWORD_DOUBLE", "TOKENTYPE_KEYWORD_STRING",
  "TOKENTYPE_KEYWORD_ARRAY", "TOKENTYPE_KEYWORD_LIST",
  "TOKENTYPE_KEYWORD_SET", "TOKENTYPE_KEYWORD_OF",
  "TOKENTYPE_KEYWORD_STRUCT", "TOKENTYPE_KEYWORD_EXTERN",
  "TOKENTYPE_KEYWORD_INLINE", "TOKENTYPE_KEYWORD_PUBLIC",
  "TOKENTYPE_KEYWORD_RETURN", "TOKENTYPE_IDENTIFIER", "TOKENTYPE_INTEGER",
  "TOKENTYPE_FLOAT", "TOKENTYPE_STRING", "TOKENTYPE_ELLIPSE", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'^'", "';'", "','", "'('", "')'", "'='", "'{'",
  "'}'", "'['", "']'", "'<'", "'>'", "$accept", "start", "module", "@1",
  "attributes", "code", "@2", "@3", "@4", "@5", "@6", "@7", "@8", "@9",
  "@10", "@11", "constantDefinition", "datatypeDefinition",
  "variableIdentifierList", "variableDefinition", "functionDefinition",
  "constantDeclaration", "datatypeDeclaration", "variableDeclaration",
  "functionDeclaration", "@12", "functionForwardDeclaration", "filename",
  "filenameList", "externConstantDeclaration", "externDatatypeDeclaration",
  "externVariableDeclaration", "externFunctionDefinition", "statement",
  "identifier", "namedDatatype", "datatypeList", "arraySize",
  "arraySizeList", "datatype", "argument", "argumentList",
  "functionArguments", "parameter", "parameterList", "functionParameters",
  "functionCall", "assignment", "expression", "constExpression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
      43,    45,    42,    47,    37,    94,    59,    44,    40,    41,
      61,   123,   125,    91,    93,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    48,    48,    50,    49,    49,    49,    51,
      51,    53,    52,    54,    52,    55,    52,    56,    52,    57,
      52,    58,    52,    59,    52,    60,    52,    61,    52,    62,
      52,    52,    52,    63,    63,    63,    64,    64,    65,    65,
      66,    67,    68,    68,    68,    69,    70,    70,    70,    70,
      70,    72,    71,    73,    74,    75,    75,    76,    76,    77,
      77,    78,    78,    79,    79,    80,    80,    80,    80,    81,
      82,    83,    83,    83,    84,    84,    85,    85,    85,    85,
      85,    85,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    87,    88,    88,
      89,    89,    89,    90,    91,    91,    92,    92,    93,    93,
      93,    94,    94,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     0,     5,     3,     3,     2,
       0,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     1,     3,     3,     2,     2,     2,     3,     1,
       2,     5,     4,     3,     2,     3,     4,     2,     4,     3,
       2,     0,     5,     2,     1,     3,     1,     4,     4,     4,
       4,     4,     4,     4,     4,     2,     2,     3,     2,     1,
       3,     2,     1,     0,     3,     1,     4,     3,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     4,     4,     3,     2,     5,     5,
       4,     3,     2,     4,     4,     4,     4,     3,     2,     3,
       3,     4,     4,     3,     2,     3,     3,     2,     3,     1,
       1,     3,     0,     1,     3,     1,     1,     0,     4,     4,
       3,     3,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     2,     0,    69,     0,     1,     8,
       7,     5,     0,    32,     0,     0,     0,     6,    21,    23,
      25,    27,    29,     0,     0,     0,    54,    56,     0,   144,
     145,   146,     0,     0,    68,     0,   143,   142,   141,     0,
       0,     0,    82,    83,    84,    85,    86,    87,    88,    89,
      90,     0,     0,     0,     0,     9,     0,     0,     0,     0,
      11,    13,    15,    17,    19,    91,     0,     0,     0,     0,
       0,     0,     0,   132,    66,    65,     0,     0,     0,     0,
       0,   133,   134,     0,     0,     0,     0,     0,     0,    67,
      35,     0,    37,     0,    81,     0,     0,   108,     0,     0,
     114,     0,     0,    97,     0,    44,     0,     0,    50,    47,
       0,    53,    51,     0,     0,     0,     0,     0,    81,    92,
      40,    39,    93,    22,    24,    26,    28,    30,   130,   125,
     126,     0,   123,   131,    55,    58,    57,    60,    59,    62,
      61,    64,    63,   140,   135,   136,   137,   138,   139,    34,
      33,    80,   155,   156,   157,     0,     0,     0,    79,     0,
      75,   101,     0,     0,     0,   110,     0,   107,     0,   116,
       0,   113,     0,    96,    72,     0,     0,    43,     0,    45,
      49,     0,     0,    12,    14,    16,    18,    20,     0,   122,
       0,   129,   128,   147,   148,     0,     0,    78,     0,     0,
       0,     0,     0,   104,     0,    77,     0,   100,     0,    81,
     105,    81,   111,    95,    94,    71,     0,    42,    48,    46,
       0,    38,     0,   119,   120,     0,   124,   154,    74,   149,
     150,   151,   152,   153,    76,    81,    98,    70,    52,   117,
       0,    41,   121,   118
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,    12,    16,    17,   113,   114,   115,   116,
     117,    67,    68,    69,    70,    71,    56,    57,   120,    58,
      59,    60,    61,    62,    63,   182,    64,    27,    28,    18,
      19,    20,    21,    22,    36,   174,   175,   159,   122,    66,
     223,   224,   225,   129,   130,   131,    37,    38,   132,   160
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -115
static const yytype_int16 yypact[] =
{
     175,  -115,    15,    11,  -115,    -3,  -115,    36,  -115,  -115,
    -115,  -115,   412,  -115,    64,   640,   616,  -115,  -115,  -115,
    -115,  -115,  -115,     6,    80,    92,  -115,  -115,   380,  -115,
    -115,  -115,   376,   376,  -115,   376,     6,  -115,  -115,   234,
     433,   453,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,    20,     0,    17,     7,  -115,    14,    85,    55,    -7,
    -115,  -115,  -115,  -115,  -115,  -115,   193,   233,   233,   233,
     233,   233,   313,   376,  -115,  -115,    64,   108,   119,   129,
     131,    -6,    -6,   655,   376,   376,   376,   376,   376,  -115,
    -115,   230,  -115,   192,   317,   183,    74,  -115,   473,   493,
    -115,   513,   533,  -115,   291,  -115,   330,    99,  -115,  -115,
     344,  -115,  -115,   233,   233,   233,   233,   233,  -115,  -115,
     101,   103,   121,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
     105,     8,   682,   682,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,    -6,    -6,    -6,    -6,    -6,    -8,
    -115,  -115,  -115,  -115,  -115,   653,   653,   653,  -115,   -14,
     687,  -115,   553,   631,   573,  -115,    97,  -115,   107,  -115,
     120,  -115,   136,  -115,  -115,   312,   193,  -115,   675,  -115,
    -115,   347,   270,  -115,  -115,  -115,  -115,  -115,    85,   635,
     376,  -115,  -115,   124,   124,   665,   653,  -115,   653,   653,
     653,   653,   653,  -115,   145,  -115,   -12,  -115,   153,    -5,
    -115,    58,  -115,  -115,  -115,  -115,   123,  -115,  -115,  -115,
     127,  -115,   193,  -115,   134,   144,  -115,  -115,   687,   124,
     124,   124,   124,   124,  -115,    65,  -115,  -115,  -115,  -115,
     592,  -115,  -115,  -115
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -115,  -115,  -115,  -115,  -115,   -64,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,   162,   167,  -115,   169,
     170,  -115,  -115,  -115,  -115,  -115,  -115,   130,  -115,  -115,
    -115,  -115,  -115,  -115,    -2,    30,  -115,    44,   157,   -28,
     -25,  -115,  -115,    26,  -115,  -115,   -10,    10,   116,  -114
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -128
static const yytype_int16 yytable[] =
{
       7,    97,    24,   123,   124,   125,   126,   127,   103,   191,
      23,     8,    91,    93,    65,   105,     5,   -81,   100,    98,
    -106,    94,    25,   196,   -81,   196,    65,    87,    88,   111,
     197,  -106,   234,     9,   112,   -81,   101,    10,    65,    65,
       6,   193,   194,   195,    72,    99,    73,   192,   104,   183,
     184,   185,   186,   187,   106,   107,   108,    24,    24,    24,
      24,    24,   102,    95,   121,    23,    23,    23,    23,    23,
     166,   168,    11,   170,   172,   161,   176,    25,    25,    25,
      25,    25,   228,  -112,   229,   230,   231,   232,   233,   150,
     -99,   109,    26,   162,  -112,   110,    65,    65,   118,    65,
      65,   -99,    65,    24,    24,    24,    24,    24,   209,   135,
       6,    23,    23,    23,    23,    23,    74,   163,   220,   164,
     137,   118,  -109,    25,    25,    25,    25,    25,    75,   119,
     139,    39,   141,  -109,   204,   179,   208,   211,   188,   119,
      95,   189,   190,  -109,   136,  -115,   118,   176,    81,    82,
      95,    83,   119,   210,   235,   138,  -115,   201,   202,   237,
      65,   222,    65,    95,   163,   140,  -115,   142,   119,   238,
    -103,   240,    24,    65,   216,    -3,     1,   119,     2,    95,
      23,  -103,   212,   241,   151,   119,   221,    65,    95,   133,
      77,  -103,    25,   118,   118,    78,    95,    79,    80,   236,
     144,   145,   146,   147,   148,   215,   134,   206,    96,   152,
     153,   154,   222,   155,   156,   243,   226,   -36,     6,     0,
     239,   157,   178,     0,   119,   119,   181,   158,   -36,     0,
       0,   149,     0,   -31,    13,    95,    95,   -10,    65,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,     0,   -10,    14,     6,   -10,    15,     6,     0,
       0,     0,   119,     0,    84,    85,    86,    87,    88,     0,
      89,    13,     0,    95,   -10,   -31,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,     0,
     -10,    14,   173,   -10,    15,     6,     0,     0,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
       0,    54,   -31,   213,   128,     0,     6,     0,     0,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,   177,    54,   -73,     0,     0,     0,     6,     6,    29,
      30,    31,  -102,    32,    33,   180,     0,     0,   218,  -102,
       0,    35,  -127,  -102,   214,     6,    29,    30,    31,     0,
      32,    33,     0,  -102,     0,     0,     0,     0,    35,     6,
      29,    30,    31,     0,    32,    33,     0,    84,    85,    86,
      87,    88,    35,   219,    40,     0,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,     0,
      54,     6,    29,    30,    31,     6,    32,    33,     0,     0,
       0,     0,   -31,    13,    35,     0,   -10,    76,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,     0,   -10,    14,    90,   -10,    15,     6,     0,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,    54,    92,     0,     0,     0,     6,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,    54,   165,     0,     0,     0,     6,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,    54,   167,     0,     0,     0,     6,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,    54,   169,     0,     0,     0,     6,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,    54,   171,     0,     0,     0,     6,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,    54,   203,     0,     0,     0,     6,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,    54,   207,     0,     0,     0,     6,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,    54,     0,     0,     0,     0,     6,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,     0,    54,     0,     0,     0,     0,     6,     0,     0,
      40,   242,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,     0,    54,     0,     0,    55,
       0,     6,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,     0,    54,     0,   152,   153,   154,
       6,   155,   156,     0,     0,     6,    29,    30,    31,   157,
      32,    33,     0,     0,     0,   205,    34,     0,    35,   152,
     153,   154,     0,   155,   156,    84,    85,    86,    87,    88,
       0,   157,     0,     0,   143,   198,   199,   200,   201,   202,
       0,     0,     0,     0,   227,    84,    85,    86,    87,    88,
       0,   217,    84,    85,    86,    87,    88,   198,   199,   200,
     201,   202
};

static const yytype_int16 yycheck[] =
{
       2,     1,    12,    67,    68,    69,    70,    71,     1,     1,
      12,     0,    40,    41,    16,     1,     1,    25,     1,    19,
      25,     1,    12,    37,    32,    37,    28,    33,    34,    36,
      44,    36,    44,    36,    41,    43,    19,     1,    40,    41,
      25,   155,   156,   157,    38,    45,    40,    39,    41,   113,
     114,   115,   116,   117,    40,    57,     1,    67,    68,    69,
      70,    71,    45,    43,    66,    67,    68,    69,    70,    71,
      98,    99,    36,   101,   102,     1,   104,    67,    68,    69,
      70,    71,   196,    25,   198,   199,   200,   201,   202,    91,
      25,    36,    28,    19,    36,    40,    98,    99,     1,   101,
     102,    36,   104,   113,   114,   115,   116,   117,     1,     1,
      25,   113,   114,   115,   116,   117,    36,    43,   182,    45,
       1,     1,    25,   113,   114,   115,   116,   117,    36,    32,
       1,    15,     1,    36,   162,    36,   164,     1,    37,    32,
      43,    38,    37,    46,    36,    25,     1,   175,    32,    33,
      43,    35,    32,    46,     1,    36,    36,    33,    34,    36,
     162,   189,   164,    43,    43,    36,    46,    36,    32,    42,
      25,    37,   182,   175,   176,     0,     1,    32,     3,    43,
     182,    36,    46,    39,     1,    32,   188,   189,    43,    73,
      28,    46,   182,     1,     1,    28,    43,    28,    28,    46,
      84,    85,    86,    87,    88,   175,    76,   163,    51,    26,
      27,    28,   240,    30,    31,   240,   190,    25,    25,    -1,
     222,    38,   106,    -1,    32,    32,   110,    44,    36,    -1,
      -1,     1,    -1,     0,     1,    43,    43,     4,   240,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,    21,    25,    23,    24,    25,    -1,
      -1,    -1,    32,    -1,    30,    31,    32,    33,    34,    -1,
      36,     1,    -1,    43,     4,    42,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      20,    21,     1,    23,    24,    25,    -1,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    20,    42,     1,     1,    -1,    25,    -1,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,     1,    20,    42,    -1,    -1,    -1,    25,    25,    26,
      27,    28,    25,    30,    31,     1,    -1,    -1,     1,    32,
      -1,    38,    39,    36,    42,    25,    26,    27,    28,    -1,
      30,    31,    -1,    46,    -1,    -1,    -1,    -1,    38,    25,
      26,    27,    28,    -1,    30,    31,    -1,    30,    31,    32,
      33,    34,    38,    36,     4,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      20,    25,    26,    27,    28,    25,    30,    31,    -1,    -1,
      -1,    -1,     0,     1,    38,    -1,     4,    37,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    20,    21,     1,    23,    24,    25,    -1,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,     1,    -1,    -1,    -1,    25,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,     1,    -1,    -1,    -1,    25,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,     1,    -1,    -1,    -1,    25,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,     1,    -1,    -1,    -1,    25,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,     1,    -1,    -1,    -1,    25,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,     1,    -1,    -1,    -1,    25,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,     1,    -1,    -1,    -1,    25,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
       4,    29,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    20,    -1,    -1,    23,
      -1,    25,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    20,    -1,    26,    27,    28,
      25,    30,    31,    -1,    -1,    25,    26,    27,    28,    38,
      30,    31,    -1,    -1,    -1,    44,    36,    -1,    38,    26,
      27,    28,    -1,    30,    31,    30,    31,    32,    33,    34,
      -1,    38,    -1,    -1,    39,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    39,    30,    31,    32,    33,    34,
      -1,    36,    30,    31,    32,    33,    34,    30,    31,    32,
      33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    48,    49,     1,    25,    81,     0,    36,
       1,    36,    50,     1,    21,    24,    51,    52,    76,    77,
      78,    79,    80,    81,    93,    94,    28,    74,    75,    26,
      27,    28,    30,    31,    36,    38,    81,    93,    94,    95,
       4,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    20,    23,    63,    64,    66,    67,
      68,    69,    70,    71,    73,    81,    86,    58,    59,    60,
      61,    62,    38,    40,    36,    36,    37,    63,    64,    66,
      67,    95,    95,    95,    30,    31,    32,    33,    34,    36,
       1,    86,     1,    86,     1,    43,    85,     1,    19,    45,
       1,    19,    45,     1,    41,     1,    40,    81,     1,    36,
      40,    36,    41,    53,    54,    55,    56,    57,     1,    32,
      65,    81,    85,    52,    52,    52,    52,    52,     1,    90,
      91,    92,    95,    95,    74,     1,    36,     1,    36,     1,
      36,     1,    36,    39,    95,    95,    95,    95,    95,     1,
      81,     1,    26,    27,    28,    30,    31,    38,    44,    84,
      96,     1,    19,    43,    45,     1,    86,     1,    86,     1,
      86,     1,    86,     1,    82,    83,    86,     1,    95,    36,
       1,    95,    72,    52,    52,    52,    52,    52,    37,    38,
      37,     1,    39,    96,    96,    96,    37,    44,    30,    31,
      32,    33,    34,     1,    86,    44,    84,     1,    86,     1,
      46,     1,    46,     1,    42,    82,    81,    36,     1,    36,
      52,    81,    86,    87,    88,    89,    90,    39,    96,    96,
      96,    96,    96,    96,    44,     1,    46,    36,    42,    81,
      37,    39,    29,    87
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, userData, quietFlag, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, userData)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, userData, quietFlag); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *userData, bool quietFlag)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, userData, quietFlag)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    void *userData;
    bool quietFlag;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (userData);
  YYUSE (quietFlag);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *userData, bool quietFlag)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, userData, quietFlag)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    void *userData;
    bool quietFlag;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, userData, quietFlag);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *userData, bool quietFlag)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, userData, quietFlag)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    void *userData;
    bool quietFlag;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , userData, quietFlag);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, userData, quietFlag); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *userData, bool quietFlag)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, userData, quietFlag)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    void *userData;
    bool quietFlag;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (userData);
  YYUSE (quietFlag);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void *userData, bool quietFlag);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *userData, bool quietFlag)
#else
int
yyparse (userData, quietFlag)
    void *userData;
    bool quietFlag;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 260 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected 'module'");
    ;}
    break;

  case 5:
#line 268 "sc_codeparser.y"
    {
      moduleName = (yyvsp[(2) - (3)]).irIdentifier.name;
    ;}
    break;

  case 6:
#line 272 "sc_codeparser.y"
    {
    ;}
    break;

  case 7:
#line 275 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    ;}
    break;

  case 8:
#line 279 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected identifier");
    ;}
    break;

  case 9:
#line 288 "sc_codeparser.y"
    {
      (yyval).irAttributes = (yyvsp[(1) - (2)]).irAttributes | SC_IR_ATTRIBUTE_MASK(PUBLIC);
    ;}
    break;

  case 10:
#line 292 "sc_codeparser.y"
    {
      (yyval).irAttributes = SC_IR_ATTRIBUTE_MASK(NONE);
    ;}
    break;

  case 11:
#line 300 "sc_codeparser.y"
    {
      IRConstantDeclaration irConstantDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add constant declaration '%s', attributes 0x%x\n",String_cString((yyvsp[(2) - (2)]).irConstantDeclaration.name),(yyvsp[(1) - (2)]).irAttributes);

      irConstantDeclaration            = (yyvsp[(2) - (2)]).irConstantDeclaration;
      irConstantDeclaration.attributes = (yyvsp[(1) - (2)]).irAttributes;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irConstantDeclaration,IR_TYPE_CONSTANT_DECLARATION,CURRENT_FILE,(yylsp[(1) - (2)]).first_line,(yylsp[(1) - (2)]).first_column));
    ;}
    break;

  case 13:
#line 311 "sc_codeparser.y"
    {
      IRDatatypeDeclaration irDatatypeDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add datatype declaration '%s', '%s', attributes 0x%x\n", String_cString((yyvsp[(2) - (2)]).irDatatypeDeclaration.name),scIR_getBaseDatatype(&(yyvsp[(2) - (2)]).irDatatypeDeclaration.datatype),(yyvsp[(1) - (2)]).irAttributes);

      irDatatypeDeclaration            = (yyvsp[(2) - (2)]).irDatatypeDeclaration;
      irDatatypeDeclaration.attributes = (yyvsp[(1) - (2)]).irAttributes;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,CURRENT_FILE,(yylsp[(1) - (2)]).first_line,(yylsp[(1) - (2)]).first_column));
    ;}
    break;

  case 15:
#line 322 "sc_codeparser.y"
    {
      IRVariableDeclaration irVariableDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add variable declaration, %d names, attributes 0x%x\n", (yyvsp[(2) - (2)]).irVariableDeclaration.nameList.count,(yyvsp[(1) - (2)]).irAttributes);

      irVariableDeclaration            = (yyvsp[(2) - (2)]).irVariableDeclaration;
      irVariableDeclaration.attributes = (yyvsp[(1) - (2)]).irAttributes;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irVariableDeclaration,IR_TYPE_VARIABLE_DECLARATION,CURRENT_FILE,(yylsp[(1) - (2)]).first_line,(yylsp[(1) - (2)]).first_column));
    ;}
    break;

  case 17:
#line 333 "sc_codeparser.y"
    {
      #define NODE (yyvsp[(2) - (2)]).irNode
      #define DATA(name) ((IRFunctionDeclaration*)NODE->data)->name

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add function declaration '%s', %d argument(s), attributes 0x%x\n",String_cString(DATA(name)),DATA(argumentList.count),(yyvsp[(1) - (2)]).irAttributes);

      DATA(attributes) = (yyvsp[(1) - (2)]).irAttributes;
      List_add(&CURRENT_IRLIST,(Node*)NODE);

      #undef DATA
      #undef NODE
    ;}
    break;

  case 19:
#line 347 "sc_codeparser.y"
    {
      IRFunctionDeclaration irFunctionDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add function fordward declaration '%s', %d argument(s), attributes 0x%x\n",String_cString((yyvsp[(2) - (2)]).irFunctionDeclaration.name),(yyvsp[(2) - (2)]).irFunctionDeclaration.argumentList.count,(yyvsp[(1) - (2)]).irAttributes);

      irFunctionDeclaration            = (yyvsp[(2) - (2)]).irFunctionDeclaration;
      irFunctionDeclaration.attributes = (yyvsp[(1) - (2)]).irAttributes | SC_IR_ATTRIBUTE_MASK(FORWARD);
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irFunctionDeclaration,IR_TYPE_FUNCTION_DECLARATION,CURRENT_FILE,(yylsp[(1) - (2)]).first_line,(yylsp[(1) - (2)]).first_column));
    ;}
    break;

  case 21:
#line 358 "sc_codeparser.y"
    {
      IRConstantDeclaration irConstantDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add extern constant declaration '%s'\n",String_cString((yyvsp[(1) - (1)]).irConstantDeclaration.name));

      irConstantDeclaration = (yyvsp[(1) - (1)]).irConstantDeclaration;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irConstantDeclaration,IR_TYPE_CONSTANT_DECLARATION,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 23:
#line 368 "sc_codeparser.y"
    {
      IRDatatypeDeclaration irDatatypeDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add extern datatype declaration '%s', '%s'\n", String_cString((yyvsp[(1) - (1)]).irDatatypeDeclaration.name),scIR_getBaseDatatype(&(yyvsp[(1) - (1)]).irDatatypeDeclaration.datatype));

      irDatatypeDeclaration = (yyvsp[(1) - (1)]).irDatatypeDeclaration;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 25:
#line 378 "sc_codeparser.y"
    {
      IRVariableDeclaration irVariableDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add extern variable declaration, %d names\n", (yyvsp[(1) - (1)]).irVariableDeclaration.nameList.count);

      irVariableDeclaration = (yyvsp[(1) - (1)]).irVariableDeclaration;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irVariableDeclaration,IR_TYPE_VARIABLE_DECLARATION,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 27:
#line 388 "sc_codeparser.y"
    {
      IRFunctionDeclaration irFunctionDeclaration;

      dprintf(DEBUGGROUP_CODE_PARSER,2,"add extern function definition '%s', %d argument(s)\n",String_cString((yyvsp[(1) - (1)]).irFunctionDeclaration.name),(yyvsp[(1) - (1)]).irFunctionDeclaration.argumentList.count);

      irFunctionDeclaration = (yyvsp[(1) - (1)]).irFunctionDeclaration;
      List_add(&CURRENT_IRLIST,(Node*)SC_IR_NEW_NODE(irFunctionDeclaration,IR_TYPE_FUNCTION_DECLARATION,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 29:
#line 398 "sc_codeparser.y"
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"add statement '%s'\n",scIR_getStatementType(&(yyvsp[(1) - (1)]).irStatement));
      List_add(&CURRENT_IRLIST, (Node*)SC_IR_NEW_NODE((yyvsp[(1) - (1)]).irStatement,IR_TYPE_STATEMENT,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 32:
#line 405 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"unexpected statement");
    ;}
    break;

  case 33:
#line 413 "sc_codeparser.y"
    {
      INIT_IR((yyval).irConstantDeclaration,IR_TYPE_CONSTANT_DECLARATION,(yylsp[(1) - (3)]));
      (yyval).irConstantDeclaration.parentFunctionDeclaration = NULL;
      (yyval).irConstantDeclaration.name                      = (yyvsp[(3) - (3)]).irIdentifier.name;
      (yyval).irConstantDeclaration.datatype                  = (yyvsp[(2) - (3)]).irDatatype;
      (yyval).irConstantDeclaration.location                  = LOCATION_NONE;
      (yyval).irConstantDeclaration.attributes                = SC_IR_ATTRIBUTE_MASK(NONE);
      (yyval).irConstantDeclaration.expression                = EXPRESSION_NONE;
    ;}
    break;

  case 34:
#line 423 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected constant name");
    ;}
    break;

  case 35:
#line 427 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected constant datatype");
    ;}
    break;

  case 36:
#line 434 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,(yylsp[(1) - (2)]));
      (yyval).irDatatypeDeclaration.parentFunctionDeclaration = NULL;
      (yyval).irDatatypeDeclaration.name                      = NULL;
      (yyval).irDatatypeDeclaration.datatype                  = (yyvsp[(2) - (2)]).irDatatype;
      (yyval).irDatatypeDeclaration.location                  = LOCATION_NONE;
      (yyval).irDatatypeDeclaration.attributes                = SC_IR_ATTRIBUTE_MASK(NONE);
    ;}
    break;

  case 37:
#line 443 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    ;}
    break;

  case 38:
#line 450 "sc_codeparser.y"
    {
      (yyval).irIdentifierList = (yyvsp[(1) - (3)]).irIdentifierList;
      List_add(&(yyval).irIdentifierList,(Node*)SC_IR_NEW_NODE((yyvsp[(3) - (3)]).irIdentifier,IR_TYPE_IDENTIFIER,CURRENT_FILE,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).first_column));
    ;}
    break;

  case 39:
#line 455 "sc_codeparser.y"
    {
      List_init(&(yyval).irIdentifierList);
      List_add(&(yyval).irIdentifierList,(Node*)SC_IR_NEW_NODE((yyvsp[(1) - (1)]).irIdentifier,IR_TYPE_IDENTIFIER,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 40:
#line 463 "sc_codeparser.y"
    {
      INIT_IR((yyval).irVariableDeclaration,IR_TYPE_VARIABLE_DECLARATION,(yylsp[(1) - (2)]));
      (yyval).irVariableDeclaration.parentFunctionDeclaration = NULL;
      (yyval).irVariableDeclaration.nameList                  = (yyvsp[(2) - (2)]).irIdentifierList;
      (yyval).irVariableDeclaration.datatype                  = (yyvsp[(1) - (2)]).irDatatype;
      (yyval).irVariableDeclaration.location                  = LOCATION_NONE;
      (yyval).irVariableDeclaration.attributes                = SC_IR_ATTRIBUTE_MASK(NONE);
      (yyval).irVariableDeclaration.expression                = EXPRESSION_NONE;
    ;}
    break;

  case 41:
#line 476 "sc_codeparser.y"
    {
      INIT_IR((yyval).irFunctionDeclaration,IR_TYPE_FUNCTION_DECLARATION,(yylsp[(1) - (5)]));
      (yyval).irFunctionDeclaration.parentFunctionDeclaration = NULL;
      (yyval).irFunctionDeclaration.name                      = (yyvsp[(2) - (5)]).irIdentifier.name;
      (yyval).irFunctionDeclaration.returnDatatype            = (yyvsp[(1) - (5)]).irDatatype;
      (yyval).irFunctionDeclaration.location                  = LOCATION_NONE;
      (yyval).irFunctionDeclaration.attributes                = SC_IR_ATTRIBUTE_MASK(NONE);
      (yyval).irFunctionDeclaration.argumentList              = (yyvsp[(4) - (5)]).irArgumentList;
      List_init(&(yyval).irFunctionDeclaration.code);
    ;}
    break;

  case 42:
#line 491 "sc_codeparser.y"
    {
      (yyval).irConstantDeclaration                           = (yyvsp[(1) - (4)]).irConstantDeclaration;
      (yyval).irConstantDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irConstantDeclaration.location.type             = LOCATION_TYPE_INTERN;
      (yyval).irConstantDeclaration.expression                = (yyvsp[(3) - (4)]).irExpression;
    ;}
    break;

  case 43:
#line 498 "sc_codeparser.y"
    {
      PARSE_ERROR((yylsp[(1) - (3)]).line,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).last_column,"expected constant expression");
    ;}
    break;

  case 44:
#line 502 "sc_codeparser.y"
    {
      PARSE_ERROR((yylsp[(1) - (2)]).line,(yylsp[(1) - (2)]).first_line,(yylsp[(1) - (2)]).last_column,"expected '='");
    ;}
    break;

  case 45:
#line 509 "sc_codeparser.y"
    {
      (yyval).irDatatypeDeclaration                           = (yyvsp[(1) - (3)]).irDatatypeDeclaration;
      (yyval).irDatatypeDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irDatatypeDeclaration.name                      = (yyvsp[(2) - (3)]).irIdentifier.name;
      (yyval).irDatatypeDeclaration.location.type             = LOCATION_TYPE_INTERN;
    ;}
    break;

  case 46:
#line 519 "sc_codeparser.y"
    {
      (yyval).irVariableDeclaration                           = (yyvsp[(1) - (4)]).irVariableDeclaration;
      (yyval).irVariableDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irVariableDeclaration.location.type             = LOCATION_TYPE_INTERN;
      (yyval).irVariableDeclaration.expression                = (yyvsp[(3) - (4)]).irExpression;
    ;}
    break;

  case 47:
#line 526 "sc_codeparser.y"
    {
      (yyval).irVariableDeclaration                           = (yyvsp[(1) - (2)]).irVariableDeclaration;
      (yyval).irVariableDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irVariableDeclaration.location.type             = LOCATION_TYPE_INTERN;
      (yyval).irVariableDeclaration.expression                = EXPRESSION_NONE;
    ;}
    break;

  case 48:
#line 533 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    ;}
    break;

  case 49:
#line 537 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected expression");
    ;}
    break;

  case 50:
#line 541 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';' or '='");
    ;}
    break;

  case 51:
#line 548 "sc_codeparser.y"
    {
      // create node
      (yyval).irNode = SC_IR_ALLOC_NODE(IR_TYPE_FUNCTION_DECLARATION,CURRENT_FILE,(yylsp[(1) - (2)]).first_line,(yylsp[(1) - (2)]).first_column);
      assert((yyval).irNode != NULL);

      // push context
      pushContext();
      CURRENT_FUNCTION_NODE = (yyval).irNode;
    ;}
    break;

  case 52:
#line 558 "sc_codeparser.y"
    {
      IRList code;

//      dprintf(DEBUGGROUP_PARSER,2,"function declaration '%s'\n",String_cString($1.irFunctionDeclaration.name));

      // get statement list, restore context
      List_init(&code);
      List_move(&CURRENT_IRLIST,&code,LIST_START,LIST_END,NULL);
      popContext();

      // init function declartion
      (yyval).irNode = (yyvsp[(3) - (5)]).irNode;
      (*((IRFunctionDeclaration*)(yyval).irNode->data))                         = (yyvsp[(1) - (5)]).irFunctionDeclaration;
      ((IRFunctionDeclaration*)(yyval).irNode->data)->parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      ((IRFunctionDeclaration*)(yyval).irNode->data)->location.type             = LOCATION_TYPE_INTERN;
      ((IRFunctionDeclaration*)(yyval).irNode->data)->code                      = code;
    ;}
    break;

  case 53:
#line 579 "sc_codeparser.y"
    {
      (yyval).irFunctionDeclaration                           = (yyvsp[(1) - (2)]).irFunctionDeclaration;
      (yyval).irFunctionDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irFunctionDeclaration.location.type             = LOCATION_TYPE_INTERN;
      List_init(&(yyval).irFunctionDeclaration.code);
    ;}
    break;

  case 54:
#line 591 "sc_codeparser.y"
    {
      (yyval).irFilename.name = String_newCString((yyvsp[(1) - (1)]).s);
    ;}
    break;

  case 55:
#line 598 "sc_codeparser.y"
    {
      (yyval).irFilenameList = (yyvsp[(1) - (3)]).irFilenameList;
      List_add(&(yyval).irFilenameList,(Node*)SC_IR_NEW_NODE((yyvsp[(3) - (3)]).irFilename,IR_TYPE_FILENAME,CURRENT_FILE,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).first_column));
    ;}
    break;

  case 56:
#line 603 "sc_codeparser.y"
    {
      List_init(&(yyval).irFilenameList);
      List_add(&(yyval).irFilenameList,(Node*)SC_IR_NEW_NODE((yyvsp[(1) - (1)]).irFilename,IR_TYPE_FILENAME,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 57:
#line 611 "sc_codeparser.y"
    {
      (yyval).irConstantDeclaration                           = (yyvsp[(3) - (4)]).irConstantDeclaration;
      (yyval).irConstantDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irConstantDeclaration.location.type             = LOCATION_TYPE_EXTERN;
      (yyval).irConstantDeclaration.location.filenameList     = (yyvsp[(2) - (4)]).irFilenameList;
    ;}
    break;

  case 58:
#line 618 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    ;}
    break;

  case 59:
#line 625 "sc_codeparser.y"
    {
      (yyval).irDatatypeDeclaration                           = (yyvsp[(3) - (4)]).irDatatypeDeclaration;
      (yyval).irDatatypeDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irDatatypeDeclaration.location.type             = LOCATION_TYPE_EXTERN;
      (yyval).irDatatypeDeclaration.location.filenameList     = (yyvsp[(2) - (4)]).irFilenameList;
    ;}
    break;

  case 60:
#line 632 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    ;}
    break;

  case 61:
#line 639 "sc_codeparser.y"
    {
      (yyval).irVariableDeclaration                           = (yyvsp[(3) - (4)]).irVariableDeclaration;
      (yyval).irVariableDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irVariableDeclaration.location.type             = LOCATION_TYPE_EXTERN;
      (yyval).irVariableDeclaration.location.filenameList     = (yyvsp[(2) - (4)]).irFilenameList;
    ;}
    break;

  case 62:
#line 646 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    ;}
    break;

  case 63:
#line 653 "sc_codeparser.y"
    {
      (yyval).irFunctionDeclaration                           = (yyvsp[(3) - (4)]).irFunctionDeclaration;
      (yyval).irFunctionDeclaration.parentFunctionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irFunctionDeclaration.location.type             = LOCATION_TYPE_EXTERN;
      (yyval).irFunctionDeclaration.location.filenameList     = (yyvsp[(2) - (4)]).irFilenameList;
    ;}
    break;

  case 64:
#line 660 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ';'");
    ;}
    break;

  case 65:
#line 668 "sc_codeparser.y"
    {
      INIT_IR((yyval).irStatement,IR_TYPE_STATEMENT,(yylsp[(1) - (2)]));
      (yyval).irStatement.type       = STATEMENT_TYPE_EXPRESSION;
      (yyval).irStatement.expression = (yyvsp[(1) - (2)]).irExpression;
    ;}
    break;

  case 66:
#line 674 "sc_codeparser.y"
    {
      INIT_IR((yyval).irStatement,IR_TYPE_STATEMENT,(yylsp[(1) - (2)]));
      (yyval).irStatement.type       = STATEMENT_TYPE_EXPRESSION;
      (yyval).irStatement.expression = (yyvsp[(1) - (2)]).irExpression;
    ;}
    break;

  case 67:
#line 680 "sc_codeparser.y"
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"return value\n");
      INIT_IR((yyval).irStatement,IR_TYPE_STATEMENT,(yylsp[(1) - (3)]));
      (yyval).irStatement.type       = STATEMENT_TYPE_RETURN;
      (yyval).irStatement.expression = (yyvsp[(2) - (3)]).irExpression;
    ;}
    break;

  case 68:
#line 687 "sc_codeparser.y"
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"return\n");
      INIT_IR((yyval).irStatement,IR_TYPE_STATEMENT,(yylsp[(1) - (2)]));
      (yyval).irStatement.type       = STATEMENT_TYPE_RETURN;
      (yyval).irStatement.expression = EXPRESSION_NONE;
    ;}
    break;

  case 69:
#line 698 "sc_codeparser.y"
    {
      (yyval).irIdentifier.name = String_newCString((yyvsp[(1) - (1)]).identifier);
    ;}
    break;

  case 70:
#line 706 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,(yylsp[(1) - (3)]));
      (yyval).irDatatypeDeclaration.name     = (yyvsp[(2) - (3)]).irIdentifier.name;
      (yyval).irDatatypeDeclaration.datatype = (yyvsp[(1) - (3)]).irDatatype;
    ;}
    break;

  case 71:
#line 715 "sc_codeparser.y"
    {
      (yyval).irDatatypeList = (yyvsp[(1) - (2)]).irDatatypeList;
      List_add(&(yyval).irDatatypeList,(Node*)SC_IR_NEW_NODE((yyvsp[(2) - (2)]).irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,CURRENT_FILE,(yylsp[(1) - (2)]).first_line,(yylsp[(1) - (2)]).first_column));
    ;}
    break;

  case 72:
#line 720 "sc_codeparser.y"
    {
      List_init(&(yyval).irDatatypeList);
      List_add(&(yyval).irDatatypeList,(Node*)SC_IR_NEW_NODE((yyvsp[(1) - (1)]).irDatatypeDeclaration,IR_TYPE_DATATYPE_DECLARATION,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 73:
#line 725 "sc_codeparser.y"
    {
      List_init(&(yyval).irDatatypeList);
    ;}
    break;

  case 74:
#line 732 "sc_codeparser.y"
    {
      IRArraySize irArraySize;

      if ((yyvsp[(3) - (3)]).irExpression.type != EXPRESSION_TYPE_INTEGER)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected constant integer expression");
      }

      (yyval).irArraySizeList = (yyvsp[(1) - (3)]).irArraySizeList;
      irArraySize.size = (yyvsp[(3) - (3)]).irExpression.i;
      List_add(&(yyval).irArraySizeList,(Node*)SC_IR_NEW_NODE((yyvsp[(3) - (3)]).irArraySize,IR_TYPE_ARRAY_SIZE,CURRENT_FILE,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).first_column));
    ;}
    break;

  case 75:
#line 745 "sc_codeparser.y"
    {
      IRArraySize irArraySize;

      if ((yyvsp[(1) - (1)]).irExpression.type != EXPRESSION_TYPE_INTEGER)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected constant integer expression");
      }

      List_init(&(yyval).irArraySizeList);
      irArraySize.size = (yyvsp[(1) - (1)]).irExpression.i;
      List_add(&(yyval).irArraySizeList,(Node*)SC_IR_NEW_NODE((yyvsp[(1) - (1)]).irArraySize,IR_TYPE_ARRAY_SIZE,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 76:
#line 761 "sc_codeparser.y"
    {
      (yyval).irArraySizeList = (yyvsp[(1) - (4)]).irArraySizeList;
      List_move(&(yyvsp[(3) - (4)]).irArraySizeList,&(yyvsp[(1) - (4)]).irArraySizeList,LIST_START,LIST_END,NULL);
    ;}
    break;

  case 77:
#line 766 "sc_codeparser.y"
    {
      IRArraySize irArraySize;

      (yyval).irArraySizeList = (yyvsp[(1) - (3)]).irArraySizeList;
      irArraySize.size = 0;
      List_add(&(yyval).irArraySizeList,(Node*)SC_IR_NEW_NODE(irArraySize,IR_TYPE_ARRAY_SIZE,CURRENT_FILE,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).first_column));
    ;}
    break;

  case 78:
#line 774 "sc_codeparser.y"
    {
      (yyval).irArraySizeList = (yyvsp[(1) - (3)]).irArraySizeList;
    ;}
    break;

  case 79:
#line 778 "sc_codeparser.y"
    {
      IRArraySize irArraySize;

      List_init(&(yyval).irArraySizeList);
      irArraySize.size = 0;
      List_add(&(yyval).irArraySizeList,(Node*)SC_IR_NEW_NODE(irArraySize,IR_TYPE_ARRAY_SIZE,CURRENT_FILE,(yylsp[(1) - (2)]).first_line,(yylsp[(1) - (2)]).first_column));
    ;}
    break;

  case 80:
#line 786 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ']'");
    ;}
    break;

  case 81:
#line 790 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected array dimension");
    ;}
    break;

  case 82:
#line 797 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_VOID;
    ;}
    break;

  case 83:
#line 802 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_CHAR;
    ;}
    break;

  case 84:
#line 807 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_SHORT;
    ;}
    break;

  case 85:
#line 812 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_INT;
    ;}
    break;

  case 86:
#line 817 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_LONG;
    ;}
    break;

  case 87:
#line 822 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_LONGLONG;
    ;}
    break;

  case 88:
#line 827 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_FLOAT;
    ;}
    break;

  case 89:
#line 832 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_DOUBLE;
    ;}
    break;

  case 90:
#line 837 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type = DATA_TYPE_STRING;
    ;}
    break;

  case 91:
#line 842 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (1)]));
      (yyval).irDatatype.type      = DATA_TYPE_USER;
      (yyval).irDatatype.user.name = (yyvsp[(1) - (1)]).irIdentifier.name;
    ;}
    break;

  case 92:
#line 848 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (2)]));
      (yyval).irDatatype.type             = DATA_TYPE_POINTER;
      (yyval).irDatatype.pointer.datatype = SC_IR_NEW((yyvsp[(1) - (2)]).irDatatype);
    ;}
    break;

  case 93:
#line 854 "sc_codeparser.y"
    {
//      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    ;}
    break;

  case 94:
#line 859 "sc_codeparser.y"
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"datatype structure\n");
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (4)]));
      (yyval).irDatatype.type                 = DATA_TYPE_STRUCT;
      (yyval).irDatatype.struct_.datatypeList = (yyvsp[(3) - (4)]).irDatatypeList;
    ;}
    break;

  case 95:
#line 866 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '}'");
    ;}
    break;

  case 96:
#line 870 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype list");
    ;}
    break;

  case 97:
#line 874 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '{'");
    ;}
    break;

  case 98:
#line 879 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (5)]));
      (yyval).irDatatype.type                = DATA_TYPE_ARRAY;
      (yyval).irDatatype.array.datatype      = SC_IR_NEW((yyvsp[(4) - (5)]).irDatatype);
      (yyval).irDatatype.array.arraySizeList = (yyvsp[(2) - (5)]).irArraySizeList;
    ;}
    break;

  case 99:
#line 886 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '>'");
    ;}
    break;

  case 100:
#line 890 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    ;}
    break;

  case 101:
#line 894 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ',' or 'of'");
    ;}
    break;

  case 102:
#line 898 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected array specification");
    ;}
    break;

  case 103:
#line 902 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (4)]));
      (yyval).irDatatype.type                = DATA_TYPE_ARRAY;
      (yyval).irDatatype.array.datatype      = SC_IR_NEW((yyvsp[(4) - (4)]).irDatatype);
      (yyval).irDatatype.array.arraySizeList = (yyvsp[(2) - (4)]).irArraySizeList;
    ;}
    break;

  case 104:
#line 909 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    ;}
    break;

  case 105:
#line 914 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (4)]));
      (yyval).irDatatype.type          = DATA_TYPE_LIST;
      (yyval).irDatatype.list.datatype = SC_IR_NEW((yyvsp[(3) - (4)]).irDatatype);
    ;}
    break;

  case 106:
#line 920 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '>'");
    ;}
    break;

  case 107:
#line 924 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    ;}
    break;

  case 108:
#line 928 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected list specification");
    ;}
    break;

  case 109:
#line 932 "sc_codeparser.y"
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"datatype list\n");
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (3)]));
      (yyval).irDatatype.type          = DATA_TYPE_LIST;
      (yyval).irDatatype.list.datatype = SC_IR_NEW((yyvsp[(3) - (3)]).irDatatype);
    ;}
    break;

  case 110:
#line 939 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");

    ;}
    break;

  case 111:
#line 944 "sc_codeparser.y"
    {
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (4)]));
      (yyval).irDatatype.type         = DATA_TYPE_LIST;
      (yyval).irDatatype.set.datatype = SC_IR_NEW((yyvsp[(3) - (4)]).irDatatype);
    ;}
    break;

  case 112:
#line 950 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected '>'");
    ;}
    break;

  case 113:
#line 954 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    ;}
    break;

  case 114:
#line 958 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected set specification");
    ;}
    break;

  case 115:
#line 962 "sc_codeparser.y"
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"datatype set\n");
      INIT_IR((yyval).irDatatype,IR_TYPE_DATATYPE,(yylsp[(1) - (3)]));
      (yyval).irDatatype.type         = DATA_TYPE_LIST;
      (yyval).irDatatype.set.datatype = SC_IR_NEW((yyvsp[(3) - (3)]).irDatatype);
    ;}
    break;

  case 116:
#line 969 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected datatype");
    ;}
    break;

  case 117:
#line 977 "sc_codeparser.y"
    {
      INIT_IR((yyval).irArgument,IR_TYPE_ARGUMENT,(yylsp[(1) - (2)]));
      (yyval).irArgument.type           = ARGUMENT_TYPE_VALUE;
      (yyval).irArgument.value.name     = (yyvsp[(2) - (2)]).irIdentifier.name; 
      (yyval).irArgument.value.datatype = (yyvsp[(1) - (2)]).irDatatype; 
    ;}
    break;

  case 118:
#line 988 "sc_codeparser.y"
    {
      (yyval).irArgumentList = (yyvsp[(1) - (3)]).irArgumentList;
      List_add(&(yyval).irArgumentList, (Node*)SC_IR_NEW_NODE((yyvsp[(3) - (3)]).irArgument,IR_TYPE_ARGUMENT,CURRENT_FILE,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).first_column));
    ;}
    break;

  case 119:
#line 993 "sc_codeparser.y"
    {
      List_init(&(yyval).irArgumentList);
      List_add(&(yyval).irArgumentList, (Node*)SC_IR_NEW_NODE((yyvsp[(1) - (1)]).irArgument,IR_TYPE_ARGUMENT,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 121:
#line 1003 "sc_codeparser.y"
    {
      IRArgument irArgument;

      irArgument.type = ARGUMENT_TYPE_ELLIPSE;
      List_add(&(yyval).irArgumentList, (Node*)SC_IR_NEW_NODE(irArgument,IR_TYPE_ARGUMENT,CURRENT_FILE,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).first_column));
    ;}
    break;

  case 122:
#line 1010 "sc_codeparser.y"
    {
      List_init(&(yyval).irArgumentList);
    ;}
    break;

  case 123:
#line 1034 "sc_codeparser.y"
    {
      INIT_IR((yyval).irParameter,IR_TYPE_PARAMETER,(yylsp[(1) - (1)]));
      (yyval).irParameter.expression = (yyvsp[(1) - (1)]).irExpression;
    ;}
    break;

  case 124:
#line 1043 "sc_codeparser.y"
    {
      (yyval).irParameterList = (yyvsp[(1) - (3)]).irParameterList;
      List_add(&(yyval).irParameterList, (Node*)SC_IR_NEW_NODE((yyvsp[(3) - (3)]).irParameter,IR_TYPE_PARAMETER,CURRENT_FILE,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).first_column));
    ;}
    break;

  case 125:
#line 1048 "sc_codeparser.y"
    {
      List_init(&(yyval).irParameterList);
      List_add(&(yyval).irParameterList, (Node*)SC_IR_NEW_NODE((yyvsp[(1) - (1)]).irParameter,IR_TYPE_PARAMETER,CURRENT_FILE,(yylsp[(1) - (1)]).first_line,(yylsp[(1) - (1)]).first_column));
    ;}
    break;

  case 126:
#line 1057 "sc_codeparser.y"
    {
      (yyval).irParameterList = (yyvsp[(1) - (1)]).irParameterList;
    ;}
    break;

  case 127:
#line 1061 "sc_codeparser.y"
    {
      List_init(&(yyval).irParameterList);
    ;}
    break;

  case 128:
#line 1069 "sc_codeparser.y"
    {
      dprintf(DEBUGGROUP_CODE_PARSER,2,"function call (%d parameters)\n", (yyvsp[(3) - (4)]).irParameterList.count);

      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (4)]));
      (yyval).irExpression.type                             = EXPRESSION_TYPE_FUNCTION_CALL;
      (yyval).irExpression.functionCall.functionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irExpression.functionCall.name                = (yyvsp[(1) - (4)]).irIdentifier.name;
      (yyval).irExpression.functionCall.declaration         = NULL;
      (yyval).irExpression.functionCall.parameterList       = (yyvsp[(3) - (4)]).irParameterList;
    ;}
    break;

  case 129:
#line 1080 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected ')'");
    ;}
    break;

  case 130:
#line 1084 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected parameter list");
    ;}
    break;

  case 131:
#line 1093 "sc_codeparser.y"
    {
      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (3)]));
      (yyval).irExpression.type                           = EXPRESSION_TYPE_ASSIGNMENT;
      (yyval).irExpression.assignment.functionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irExpression.assignment.name                = (yyvsp[(1) - (3)]).irIdentifier.name;
      (yyval).irExpression.assignment.declaration         = NULL;
      (yyval).irExpression.assignment.operator            = OPERATOR_TYPE_NONE;
      (yyval).irExpression.assignment.expression          = (IRExpression*)SC_IR_NEW((yyvsp[(3) - (3)]).irExpression);
    ;}
    break;

  case 132:
#line 1103 "sc_codeparser.y"
    {
      PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"expected expression");
    ;}
    break;

  case 133:
#line 1111 "sc_codeparser.y"
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    ;}
    break;

  case 134:
#line 1115 "sc_codeparser.y"
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    ;}
    break;

  case 135:
#line 1119 "sc_codeparser.y"
    {
      IRDatatype datatype1,datatype2;

      datatype1 = scDatatype_getExpressionDatatype(&(yyvsp[(1) - (3)]).irExpression);
//fprintf(stderr,"%s,%d: %s\n",__FILE__,__LINE__,scIR_getBaseDatatype(&datatype1));
      datatype2 = scDatatype_getExpressionDatatype(&(yyvsp[(3) - (3)]).irExpression);
//fprintf(stderr,"%s,%d: %s\n",__FILE__,__LINE__,scIR_getBaseDatatype(&datatype2));
      if (!scDatatype_checkCompatible(&datatype1,&datatype2))
      {
        PARSE_ERROR((yylsp[(1) - (3)]).line,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).last_column,"incompatible datatypes for '+'");
      }
      SC_IR_DONE(datatype2);
      SC_IR_DONE(datatype1);

      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (3)]));
      (yyval).irExpression.type                       = EXPRESSION_TYPE_BINARY_OPERATOR;
      (yyval).irExpression.binaryOperator.operator    = OPERATOR_TYPE_ADD;
      (yyval).irExpression.binaryOperator.expression1 = (IRExpression*)SC_IR_NEW((yyvsp[(1) - (3)]).irExpression);
      (yyval).irExpression.binaryOperator.expression2 = (IRExpression*)SC_IR_NEW((yyvsp[(3) - (3)]).irExpression);
    ;}
    break;

  case 136:
#line 1140 "sc_codeparser.y"
    {
      IRDatatype datatype1,datatype2;

      datatype1 = scDatatype_getExpressionDatatype(&(yyvsp[(1) - (3)]).irExpression);
      datatype2 = scDatatype_getExpressionDatatype(&(yyvsp[(3) - (3)]).irExpression);
      if (!scDatatype_checkCompatible(&datatype1,&datatype2))
      {
        PARSE_ERROR((yylsp[(1) - (3)]).line,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).last_column,"incompatible datatypes for '+'");
      }
      SC_IR_DONE(datatype2);
      SC_IR_DONE(datatype1);

      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (3)]));
      (yyval).irExpression.type                       = EXPRESSION_TYPE_BINARY_OPERATOR;
      (yyval).irExpression.binaryOperator.operator    = OPERATOR_TYPE_SUB;
      (yyval).irExpression.binaryOperator.expression1 = (IRExpression*)SC_IR_NEW((yyvsp[(1) - (3)]).irExpression);
      (yyval).irExpression.binaryOperator.expression2 = (IRExpression*)SC_IR_NEW((yyvsp[(3) - (3)]).irExpression);
    ;}
    break;

  case 137:
#line 1159 "sc_codeparser.y"
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    ;}
    break;

  case 138:
#line 1163 "sc_codeparser.y"
    {
      IRDatatype datatype1,datatype2;

      datatype1 = scDatatype_getExpressionDatatype(&(yyvsp[(1) - (3)]).irExpression);
      datatype2 = scDatatype_getExpressionDatatype(&(yyvsp[(3) - (3)]).irExpression);
      if (!scDatatype_checkCompatible(&datatype1,&datatype2))
      {
        PARSE_ERROR((yylsp[(1) - (3)]).line,(yylsp[(1) - (3)]).first_line,(yylsp[(1) - (3)]).last_column,"incompatible datatypes for '+'");
      }
      SC_IR_DONE(datatype2);
      SC_IR_DONE(datatype1);

      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (3)]));
      (yyval).irExpression.type                       = EXPRESSION_TYPE_BINARY_OPERATOR;
      (yyval).irExpression.binaryOperator.operator    = OPERATOR_TYPE_DIV;
      (yyval).irExpression.binaryOperator.expression1 = (IRExpression*)SC_IR_NEW((yyvsp[(1) - (3)]).irExpression);
      (yyval).irExpression.binaryOperator.expression2 = (IRExpression*)SC_IR_NEW((yyvsp[(3) - (3)]).irExpression);
    ;}
    break;

  case 139:
#line 1182 "sc_codeparser.y"
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    ;}
    break;

  case 140:
#line 1186 "sc_codeparser.y"
    {
      HALT_INTERNAL_ERROR_STILL_NOT_IMPLEMENTED();
    ;}
    break;

  case 141:
#line 1190 "sc_codeparser.y"
    {
      (yyval).irExpression = (yyvsp[(1) - (1)]).irExpression;
    ;}
    break;

  case 142:
#line 1194 "sc_codeparser.y"
    {
      (yyval).irExpression = (yyvsp[(1) - (1)]).irExpression;
    ;}
    break;

  case 143:
#line 1198 "sc_codeparser.y"
    {
      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (1)]));
      (yyval).irExpression.type                         = EXPRESSION_TYPE_IDENTIFIER;
      (yyval).irExpression.variable.functionDeclaration = CURRENT_FUNCTION_DECLARATION;
      (yyval).irExpression.variable.name                = (yyvsp[(1) - (1)]).irIdentifier.name;
      (yyval).irExpression.variable.declaration         = NULL;
    ;}
    break;

  case 144:
#line 1206 "sc_codeparser.y"
    {
      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (1)]));
      (yyval).irExpression.type = EXPRESSION_TYPE_INTEGER;
      (yyval).irExpression.i    = (yyvsp[(1) - (1)]).i;
    ;}
    break;

  case 145:
#line 1212 "sc_codeparser.y"
    {
      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (1)]));
      (yyval).irExpression.type = EXPRESSION_TYPE_FLOAT;
      (yyval).irExpression.d    = (yyvsp[(1) - (1)]).d;
    ;}
    break;

  case 146:
#line 1218 "sc_codeparser.y"
    {
      INIT_IR((yyval).irExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (1)]));
      (yyval).irExpression.type = EXPRESSION_TYPE_STRING;
      (yyval).irExpression.s    = String_newCString((yyvsp[(1) - (1)]).s);
    ;}
    break;

  case 147:
#line 1228 "sc_codeparser.y"
    {
      switch ((yyvsp[(1) - (2)]).irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          (yyval).irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          (yyval).irConstExpression.i    = (yyvsp[(1) - (2)]).irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          (yyval).irConstExpression.type = TOKENTYPE_FLOAT;
          (yyval).irConstExpression.d    = (yyvsp[(1) - (2)]).irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '+' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",(yyvsp[(1) - (2)]).irConstExpression.type);
          break;
      }
    ;}
    break;

  case 148:
#line 1248 "sc_codeparser.y"
    {
      switch ((yyvsp[(1) - (2)]).irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          (yyval).irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          (yyval).irConstExpression.i    = -(yyvsp[(1) - (2)]).irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          (yyval).irConstExpression.type = TOKENTYPE_FLOAT;
          (yyval).irConstExpression.d    = -(yyvsp[(1) - (2)]).irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '-' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",(yyvsp[(1) - (2)]).irConstExpression.type);
          break;
      }
    ;}
    break;

  case 149:
#line 1268 "sc_codeparser.y"
    {
      promoteConstExpressionType(&(yyvsp[(1) - (3)]).irConstExpression,(yyvsp[(3) - (3)]).irConstExpression.type);
      promoteConstExpressionType(&(yyvsp[(3) - (3)]).irConstExpression,(yyvsp[(1) - (3)]).irConstExpression.type);

      if ((yyvsp[(1) - (3)]).irExpression.type != (yyvsp[(3) - (3)]).irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ((yyvsp[(1) - (3)]).irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          (yyval).irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          (yyval).irConstExpression.i    = (yyvsp[(1) - (3)]).irConstExpression.i + (yyvsp[(3) - (3)]).irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          (yyval).irConstExpression.type = TOKENTYPE_FLOAT;
          (yyval).irConstExpression.d    = (yyvsp[(1) - (3)]).irConstExpression.d + (yyvsp[(3) - (3)]).irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          (yyval).irConstExpression.type = TOKENTYPE_STRING;
          (yyval).irConstExpression.s    = String_append(String_copy((yyvsp[(1) - (3)]).irConstExpression.s),(yyvsp[(3) - (3)]).irConstExpression.s);
          String_delete((yyvsp[(1) - (3)]).irConstExpression.s);
          String_delete((yyvsp[(2) - (3)]).irConstExpression.s);
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",(yyvsp[(1) - (3)]).irConstExpression.type);
          break;
      }
    ;}
    break;

  case 150:
#line 1299 "sc_codeparser.y"
    {
      promoteConstExpressionType(&(yyvsp[(1) - (3)]).irConstExpression,(yyvsp[(3) - (3)]).irConstExpression.type);
      promoteConstExpressionType(&(yyvsp[(3) - (3)]).irConstExpression,(yyvsp[(1) - (3)]).irConstExpression.type);

      if ((yyvsp[(1) - (3)]).irExpression.type != (yyvsp[(3) - (3)]).irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ((yyvsp[(1) - (3)]).irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          (yyval).irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          (yyval).irConstExpression.i    = (yyvsp[(1) - (3)]).irConstExpression.i - (yyvsp[(3) - (3)]).irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          (yyval).irConstExpression.type = TOKENTYPE_FLOAT;
          (yyval).irConstExpression.d    = (yyvsp[(1) - (3)]).irConstExpression.d - (yyvsp[(3) - (3)]).irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '-' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",(yyvsp[(1) - (3)]).irConstExpression.type);
          break;
      }
    ;}
    break;

  case 151:
#line 1327 "sc_codeparser.y"
    {
      promoteConstExpressionType(&(yyvsp[(1) - (3)]).irConstExpression,(yyvsp[(3) - (3)]).irConstExpression.type);
      promoteConstExpressionType(&(yyvsp[(3) - (3)]).irConstExpression,(yyvsp[(1) - (3)]).irConstExpression.type);

      if ((yyvsp[(1) - (3)]).irExpression.type != (yyvsp[(3) - (3)]).irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ((yyvsp[(1) - (3)]).irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          (yyval).irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          (yyval).irConstExpression.i    = (yyvsp[(1) - (3)]).irConstExpression.i * (yyvsp[(3) - (3)]).irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          (yyval).irConstExpression.type = TOKENTYPE_FLOAT;
          (yyval).irConstExpression.d    = (yyvsp[(1) - (3)]).irConstExpression.d * (yyvsp[(3) - (3)]).irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '*' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",(yyvsp[(1) - (3)]).irConstExpression.type);
          break;
      }
    ;}
    break;

  case 152:
#line 1355 "sc_codeparser.y"
    {
      promoteConstExpressionType(&(yyvsp[(1) - (3)]).irConstExpression,(yyvsp[(3) - (3)]).irConstExpression.type);
      promoteConstExpressionType(&(yyvsp[(3) - (3)]).irConstExpression,(yyvsp[(1) - (3)]).irConstExpression.type);

      if ((yyvsp[(1) - (3)]).irExpression.type != (yyvsp[(3) - (3)]).irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ((yyvsp[(1) - (3)]).irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          (yyval).irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          (yyval).irConstExpression.i    = (yyvsp[(1) - (3)]).irConstExpression.i / (yyvsp[(3) - (3)]).irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          (yyval).irConstExpression.type = TOKENTYPE_FLOAT;
          (yyval).irConstExpression.d    = (yyvsp[(1) - (3)]).irConstExpression.d / (yyvsp[(3) - (3)]).irConstExpression.d;
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '/' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",(yyvsp[(1) - (3)]).irConstExpression.type);
          break;
      }
    ;}
    break;

  case 153:
#line 1383 "sc_codeparser.y"
    {
      promoteConstExpressionType(&(yyvsp[(1) - (3)]).irConstExpression,(yyvsp[(3) - (3)]).irConstExpression.type);
      promoteConstExpressionType(&(yyvsp[(3) - (3)]).irConstExpression,(yyvsp[(1) - (3)]).irConstExpression.type);

      if ((yyvsp[(1) - (3)]).irExpression.type != (yyvsp[(3) - (3)]).irExpression.type)
      {
        PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype in constant expression");
      }

      switch ((yyvsp[(1) - (3)]).irConstExpression.type)
      {
        case EXPRESSION_TYPE_INTEGER:
          (yyval).irConstExpression.type = EXPRESSION_TYPE_INTEGER;
          (yyval).irConstExpression.i    = (yyvsp[(1) - (3)]).irConstExpression.i % (yyvsp[(3) - (3)]).irConstExpression.i;
          break;
        case TOKENTYPE_FLOAT:
          (yyval).irConstExpression.type = TOKENTYPE_FLOAT;
          (yyval).irConstExpression.d    = fmod((yyvsp[(1) - (3)]).irConstExpression.d,(yyvsp[(3) - (3)]).irConstExpression.d);
          break;
        case TOKENTYPE_STRING:
          PARSE_ERROR(yylloc.line,yylloc.first_line,yylloc.first_column,"incompatible datatype for operator '%' in constant expression");
          break;
        default:
          HALT_INTERNAL_ERROR_UNHANDLED_SWITCH_CASE("type %d",(yyvsp[(1) - (3)]).irConstExpression.type);
          break;
      }
    ;}
    break;

  case 154:
#line 1411 "sc_codeparser.y"
    {
      (yyval).irConstExpression = (yyvsp[(2) - (3)]).irConstExpression;
    ;}
    break;

  case 155:
#line 1415 "sc_codeparser.y"
    {
      INIT_IR((yyval).irConstExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (1)]));
      (yyval).irConstExpression.type = EXPRESSION_TYPE_INTEGER;
      (yyval).irConstExpression.i    = (yyvsp[(1) - (1)]).i;
    ;}
    break;

  case 156:
#line 1421 "sc_codeparser.y"
    {
      INIT_IR((yyval).irConstExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (1)]));
      (yyval).irConstExpression.type = EXPRESSION_TYPE_FLOAT;
      (yyval).irConstExpression.d    = (yyvsp[(1) - (1)]).d;
    ;}
    break;

  case 157:
#line 1427 "sc_codeparser.y"
    {
      INIT_IR((yyval).irConstExpression,IR_TYPE_EXPRESSION,(yylsp[(1) - (1)]));
      (yyval).irConstExpression.type = EXPRESSION_TYPE_STRING;
      (yyval).irConstExpression.s    = String_newCString((yyvsp[(1) - (1)]).s);
    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 3401 "sc_codeparser_code.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, userData, quietFlag, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, userData, quietFlag, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, userData, quietFlag, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, userData, quietFlag);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, userData, quietFlag);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, userData, quietFlag, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, userData, quietFlag);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, userData, quietFlag);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1434 "sc_codeparser.y"


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

