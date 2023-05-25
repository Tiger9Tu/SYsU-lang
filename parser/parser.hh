/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_WORKSPACE_SYSU_LANG_PARSER_PARSER_HH_INCLUDED
# define YY_YY_WORKSPACE_SYSU_LANG_PARSER_PARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 5 "/workspace/SYsU-lang/parser/parser.y"

#include "asg.hpp"

#line 53 "/workspace/SYsU-lang/parser/parser.hh"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    L_PAREN = 258,                 /* L_PAREN  */
    R_PAREN = 259,                 /* R_PAREN  */
    L_BRACE = 260,                 /* L_BRACE  */
    R_BRACE = 261,                 /* R_BRACE  */
    L_SQUARE = 262,                /* L_SQUARE  */
    R_SQUARE = 263,                /* R_SQUARE  */
    COMMA = 264,                   /* COMMA  */
    SEMI = 265,                    /* SEMI  */
    ELLIPSIS = 266,                /* ELLIPSIS  */
    PLUS = 267,                    /* PLUS  */
    MINUS = 268,                   /* MINUS  */
    SLASH = 269,                   /* SLASH  */
    STAR = 270,                    /* STAR  */
    PERCENT = 271,                 /* PERCENT  */
    EQUAL = 272,                   /* EQUAL  */
    LESS = 273,                    /* LESS  */
    GREATER = 274,                 /* GREATER  */
    LESSEQUAL = 275,               /* LESSEQUAL  */
    GREATEREQUAL = 276,            /* GREATEREQUAL  */
    EQUALEQUAL = 277,              /* EQUALEQUAL  */
    EXCLAIMEQUAL = 278,            /* EXCLAIMEQUAL  */
    EXCLAIM = 279,                 /* EXCLAIM  */
    TILDE = 280,                   /* TILDE  */
    AMP = 281,                     /* AMP  */
    AMPAMP = 282,                  /* AMPAMP  */
    PIPEPIPE = 283,                /* PIPEPIPE  */
    CHAR = 284,                    /* CHAR  */
    SHORT = 285,                   /* SHORT  */
    INT = 286,                     /* INT  */
    LONG = 287,                    /* LONG  */
    SIGNED = 288,                  /* SIGNED  */
    UNSIGNED = 289,                /* UNSIGNED  */
    FLOAT = 290,                   /* FLOAT  */
    DOUBLE = 291,                  /* DOUBLE  */
    CONST = 292,                   /* CONST  */
    VOLATILE = 293,                /* VOLATILE  */
    VOID = 294,                    /* VOID  */
    NUMERIC_CONSTANT = 295,        /* NUMERIC_CONSTANT  */
    IDENTIFIER = 296,              /* IDENTIFIER  */
    STRING_LITERAL = 297,          /* STRING_LITERAL  */
    CASE = 298,                    /* CASE  */
    DEFAULT = 299,                 /* DEFAULT  */
    IF = 300,                      /* IF  */
    ELSE = 301,                    /* ELSE  */
    SWITCH = 302,                  /* SWITCH  */
    WHILE = 303,                   /* WHILE  */
    DO = 304,                      /* DO  */
    FOR = 305,                     /* FOR  */
    GOTO = 306,                    /* GOTO  */
    CONTINUE = 307,                /* CONTINUE  */
    BREAK = 308,                   /* BREAK  */
    RETURN = 309                   /* RETURN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 9 "/workspace/SYsU-lang/parser/parser.y"

  TranslationUnitDecl* TranslationUnitDecl;
  ExternalDecl* ExternalDecl;
  InitDeclList* InitDeclList;
  InitDecltorList* InitDecltorList;
  InitDecltor* InitDecltor;
  Decltor* Decltor;
  VarDecltor* VarDecltor;
  FunctionDecltor* FunctionDecltor;
  VarDecl* VarDecl;
  FunctionDecl* FunctionDecl;
  ParmVarDeclList* ParmVarDeclList;
  ParmVarDecl* ParmVarDecl;
  CompoundStmt* CompoundStmt;
  StmtList* StmtList;
  Stmt* Stmt;
  ReturnStmt* ReturnStmt;
  DeclStmt* DeclStmt;
  ExprStmt* ExprStmt;
  IfStmt* IfStmt;
  WhileStmt* WhileStmt;
  DoStmt* DoStmt;
  BreakStmt* BreakStmt;
  ContinueStmt* ContinueStmt;
  NullStmt* NullStmt;
  Expr* Expr;
  DeclRefExpr* DeclRefExpr;
  CallExpr* CallExpr;
  ExprList* ExprList;
  InitListExpr* InitListExpr;
  StringLiteral* StringLiteral;
  TypeSpec* TypeSpec;
  Type::BaseType BaseType;
  Str* Str;

#line 160 "/workspace/SYsU-lang/parser/parser.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_WORKSPACE_SYSU_LANG_PARSER_PARSER_HH_INCLUDED  */
