/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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


