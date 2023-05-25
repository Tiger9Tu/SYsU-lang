/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "/workspace/SYsU-lang/parser/parser.y"

#include "relexer.hpp"


#line 76 "/workspace/SYsU-lang/parser/parser.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.hh"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_L_PAREN = 3,                    /* L_PAREN  */
  YYSYMBOL_R_PAREN = 4,                    /* R_PAREN  */
  YYSYMBOL_L_BRACE = 5,                    /* L_BRACE  */
  YYSYMBOL_R_BRACE = 6,                    /* R_BRACE  */
  YYSYMBOL_L_SQUARE = 7,                   /* L_SQUARE  */
  YYSYMBOL_R_SQUARE = 8,                   /* R_SQUARE  */
  YYSYMBOL_COMMA = 9,                      /* COMMA  */
  YYSYMBOL_SEMI = 10,                      /* SEMI  */
  YYSYMBOL_ELLIPSIS = 11,                  /* ELLIPSIS  */
  YYSYMBOL_PLUS = 12,                      /* PLUS  */
  YYSYMBOL_MINUS = 13,                     /* MINUS  */
  YYSYMBOL_SLASH = 14,                     /* SLASH  */
  YYSYMBOL_STAR = 15,                      /* STAR  */
  YYSYMBOL_PERCENT = 16,                   /* PERCENT  */
  YYSYMBOL_EQUAL = 17,                     /* EQUAL  */
  YYSYMBOL_LESS = 18,                      /* LESS  */
  YYSYMBOL_GREATER = 19,                   /* GREATER  */
  YYSYMBOL_LESSEQUAL = 20,                 /* LESSEQUAL  */
  YYSYMBOL_GREATEREQUAL = 21,              /* GREATEREQUAL  */
  YYSYMBOL_EQUALEQUAL = 22,                /* EQUALEQUAL  */
  YYSYMBOL_EXCLAIMEQUAL = 23,              /* EXCLAIMEQUAL  */
  YYSYMBOL_EXCLAIM = 24,                   /* EXCLAIM  */
  YYSYMBOL_TILDE = 25,                     /* TILDE  */
  YYSYMBOL_AMP = 26,                       /* AMP  */
  YYSYMBOL_AMPAMP = 27,                    /* AMPAMP  */
  YYSYMBOL_PIPEPIPE = 28,                  /* PIPEPIPE  */
  YYSYMBOL_CHAR = 29,                      /* CHAR  */
  YYSYMBOL_SHORT = 30,                     /* SHORT  */
  YYSYMBOL_INT = 31,                       /* INT  */
  YYSYMBOL_LONG = 32,                      /* LONG  */
  YYSYMBOL_SIGNED = 33,                    /* SIGNED  */
  YYSYMBOL_UNSIGNED = 34,                  /* UNSIGNED  */
  YYSYMBOL_FLOAT = 35,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 36,                    /* DOUBLE  */
  YYSYMBOL_CONST = 37,                     /* CONST  */
  YYSYMBOL_VOLATILE = 38,                  /* VOLATILE  */
  YYSYMBOL_VOID = 39,                      /* VOID  */
  YYSYMBOL_NUMERIC_CONSTANT = 40,          /* NUMERIC_CONSTANT  */
  YYSYMBOL_IDENTIFIER = 41,                /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 42,            /* STRING_LITERAL  */
  YYSYMBOL_CASE = 43,                      /* CASE  */
  YYSYMBOL_DEFAULT = 44,                   /* DEFAULT  */
  YYSYMBOL_IF = 45,                        /* IF  */
  YYSYMBOL_ELSE = 46,                      /* ELSE  */
  YYSYMBOL_SWITCH = 47,                    /* SWITCH  */
  YYSYMBOL_WHILE = 48,                     /* WHILE  */
  YYSYMBOL_DO = 49,                        /* DO  */
  YYSYMBOL_FOR = 50,                       /* FOR  */
  YYSYMBOL_GOTO = 51,                      /* GOTO  */
  YYSYMBOL_CONTINUE = 52,                  /* CONTINUE  */
  YYSYMBOL_BREAK = 53,                     /* BREAK  */
  YYSYMBOL_RETURN = 54,                    /* RETURN  */
  YYSYMBOL_YYACCEPT = 55,                  /* $accept  */
  YYSYMBOL_l_paren = 56,                   /* l_paren  */
  YYSYMBOL_r_paren = 57,                   /* r_paren  */
  YYSYMBOL_l_brace = 58,                   /* l_brace  */
  YYSYMBOL_r_brace = 59,                   /* r_brace  */
  YYSYMBOL_semi = 60,                      /* semi  */
  YYSYMBOL_translation_unit_decl = 61,     /* translation_unit_decl  */
  YYSYMBOL_external_decl = 62,             /* external_decl  */
  YYSYMBOL_init_decl_list = 63,            /* init_decl_list  */
  YYSYMBOL_init_decltor_list = 64,         /* init_decltor_list  */
  YYSYMBOL_init_decltor = 65,              /* init_decltor  */
  YYSYMBOL_decltor = 66,                   /* decltor  */
  YYSYMBOL_var_decltor = 67,               /* var_decltor  */
  YYSYMBOL_function_decltor = 68,          /* function_decltor  */
  YYSYMBOL_parm_var_decl_list = 69,        /* parm_var_decl_list  */
  YYSYMBOL_parm_var_decl = 70,             /* parm_var_decl  */
  YYSYMBOL_function_head = 71,             /* function_head  */
  YYSYMBOL_function_decl = 72,             /* function_decl  */
  YYSYMBOL_compound_stmt = 73,             /* compound_stmt  */
  YYSYMBOL_stmt_list = 74,                 /* stmt_list  */
  YYSYMBOL_stmt = 75,                      /* stmt  */
  YYSYMBOL_return_stmt = 76,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 77,                 /* expr_stmt  */
  YYSYMBOL_decl_stmt = 78,                 /* decl_stmt  */
  YYSYMBOL_control_stmt = 79,              /* control_stmt  */
  YYSYMBOL_if_stmt = 80,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 81,                /* while_stmt  */
  YYSYMBOL_do_stmt = 82,                   /* do_stmt  */
  YYSYMBOL_break_stmt = 83,                /* break_stmt  */
  YYSYMBOL_continue_stmt = 84,             /* continue_stmt  */
  YYSYMBOL_null_stmt = 85,                 /* null_stmt  */
  YYSYMBOL_primary_expr = 86,              /* primary_expr  */
  YYSYMBOL_postfix_expr = 87,              /* postfix_expr  */
  YYSYMBOL_unary_expr = 88,                /* unary_expr  */
  YYSYMBOL_unary_op = 89,                  /* unary_op  */
  YYSYMBOL_mul_div_expr = 90,              /* mul_div_expr  */
  YYSYMBOL_mul_div_op = 91,                /* mul_div_op  */
  YYSYMBOL_add_sub_expr = 92,              /* add_sub_expr  */
  YYSYMBOL_relational_expr = 93,           /* relational_expr  */
  YYSYMBOL_relational_op = 94,             /* relational_op  */
  YYSYMBOL_equality_expr = 95,             /* equality_expr  */
  YYSYMBOL_logical_and_expr = 96,          /* logical_and_expr  */
  YYSYMBOL_logical_or_expr = 97,           /* logical_or_expr  */
  YYSYMBOL_assign_expr = 98,               /* assign_expr  */
  YYSYMBOL_assign_operator = 99,           /* assign_operator  */
  YYSYMBOL_expr = 100,                     /* expr  */
  YYSYMBOL_decl_ref_expr = 101,            /* decl_ref_expr  */
  YYSYMBOL_call_expr = 102,                /* call_expr  */
  YYSYMBOL_expr_list = 103,                /* expr_list  */
  YYSYMBOL_init_list_expr = 104,           /* init_list_expr  */
  YYSYMBOL_literal = 105,                  /* literal  */
  YYSYMBOL_string_literal = 106,           /* string_literal  */
  YYSYMBOL_type_spec = 107,                /* type_spec  */
  YYSYMBOL_base_type = 108                 /* base_type  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   406

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  176

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    94,    94,    98,   104,   112,   120,   131,   134,   140,
     143,   151,   157,   161,   167,   170,   176,   180,   186,   191,
     196,   202,   206,   211,   218,   222,   228,   234,   239,   246,
     249,   254,   258,   264,   267,   270,   273,   276,   279,   285,
     289,   297,   303,   308,   311,   314,   317,   320,   326,   329,
     334,   339,   344,   349,   354,   362,   363,   364,   367,   368,
     372,   373,   376,   385,   386,   394,   395,   396,   397,   398,
     399,   406,   407,   415,   416,   417,   421,   422,   426,   434,
     435,   443,   444,   445,   446,   450,   451,   455,   464,   465,
     473,   474,   481,   482,   490,   506,   511,   517,   520,   525,
     529,   536,   539,   544,   556,   562,   566,   573,   576,   582,
     585,   588,   591,   594,   597
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "L_PAREN", "R_PAREN",
  "L_BRACE", "R_BRACE", "L_SQUARE", "R_SQUARE", "COMMA", "SEMI",
  "ELLIPSIS", "PLUS", "MINUS", "SLASH", "STAR", "PERCENT", "EQUAL", "LESS",
  "GREATER", "LESSEQUAL", "GREATEREQUAL", "EQUALEQUAL", "EXCLAIMEQUAL",
  "EXCLAIM", "TILDE", "AMP", "AMPAMP", "PIPEPIPE", "CHAR", "SHORT", "INT",
  "LONG", "SIGNED", "UNSIGNED", "FLOAT", "DOUBLE", "CONST", "VOLATILE",
  "VOID", "NUMERIC_CONSTANT", "IDENTIFIER", "STRING_LITERAL", "CASE",
  "DEFAULT", "IF", "ELSE", "SWITCH", "WHILE", "DO", "FOR", "GOTO",
  "CONTINUE", "BREAK", "RETURN", "$accept", "l_paren", "r_paren",
  "l_brace", "r_brace", "semi", "translation_unit_decl", "external_decl",
  "init_decl_list", "init_decltor_list", "init_decltor", "decltor",
  "var_decltor", "function_decltor", "parm_var_decl_list", "parm_var_decl",
  "function_head", "function_decl", "compound_stmt", "stmt_list", "stmt",
  "return_stmt", "expr_stmt", "decl_stmt", "control_stmt", "if_stmt",
  "while_stmt", "do_stmt", "break_stmt", "continue_stmt", "null_stmt",
  "primary_expr", "postfix_expr", "unary_expr", "unary_op", "mul_div_expr",
  "mul_div_op", "add_sub_expr", "relational_expr", "relational_op",
  "equality_expr", "logical_and_expr", "logical_or_expr", "assign_expr",
  "assign_operator", "expr", "decl_ref_expr", "call_expr", "expr_list",
  "init_list_expr", "literal", "string_literal", "type_spec", "base_type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309
};
#endif

#define YYPACT_NINF (-91)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-103)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      88,   -91,   -91,   -27,   -91,    88,   -91,    74,   -91,   -91,
      21,   -91,   -22,   -91,   -91,   -91,   -91,   -91,   -91,   129,
     -91,    45,    36,   -91,    33,    17,    53,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,    45,
      45,   181,    51,    51,   233,   325,   129,   -91,   -91,   -91,
     -91,   129,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,    61,    58,   325,    18,    39,    19,
      40,    43,    49,   -91,    51,    45,   -91,   -91,   -91,    37,
     -22,   342,   -22,   -91,   325,   239,   325,   325,    35,   -91,
     -91,   279,   -91,    51,    80,   378,    51,    38,   -91,   -91,
     285,   -91,   325,   -91,   -91,   -91,   -91,   325,   325,   325,
     -91,   -91,   -91,   -91,   325,   325,   325,   325,   325,   -91,
     319,   -91,   -91,   -91,   -91,     7,   -91,    46,   -91,   -91,
     -91,    78,    80,    80,    45,   -91,   -91,   -91,   -91,   325,
     -91,   -91,    85,   -91,    49,   -91,    18,    18,    39,    19,
      19,    40,    43,   -91,    14,   351,   -91,   -91,    17,   -91,
     181,   181,   325,   -91,   -91,   -91,    80,   -91,    42,   -91,
      80,   -91,   181,    84,   -91,   -91
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,   112,   109,   113,   111,     0,   110,     0,     7,     9,
       0,    10,     0,   107,   114,   108,     1,     8,     4,     0,
      28,    18,     0,    12,    14,    16,    17,     2,     5,     6,
      67,    68,    66,    70,    69,    65,   103,    96,   105,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    54,    42,
      36,     0,    31,    33,    34,    35,    37,    43,    44,    47,
      45,    46,    38,    60,    63,    71,     0,    76,    79,    85,
      88,    90,    92,    95,     0,    55,    59,    58,    56,   104,
       0,     0,     0,    11,     0,     0,     0,     0,     0,    53,
      52,     0,    40,     0,     0,    29,    99,     0,    30,    32,
       0,    94,     0,    64,    74,    73,    75,     0,     0,     0,
      82,    81,    84,    83,     0,     0,     0,     0,     0,    41,
       0,   106,    17,     3,    23,     0,    24,     0,    13,    15,
      19,     0,     0,     0,     0,   102,    99,    39,    57,     0,
     101,    62,     0,    71,    93,    72,    77,    78,    80,    86,
      87,    89,    91,    97,     0,     0,    21,    18,    26,    20,
       0,     0,     0,   100,    61,    98,     0,    25,    48,    50,
       0,    22,     0,     0,    49,    51
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -91,   -18,   -90,   -10,   -26,   -15,   -91,    89,    10,   -91,
      13,   -91,   -25,    92,   -91,   -48,   -91,   -91,   100,   -91,
     -39,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -17,   -91,   -42,   -91,    -2,   -43,   -91,
      -3,     0,    22,   -91,   -91,   -31,   -91,   -91,    -5,   -91,
     -91,   -91,     1,   -91
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    45,   124,    91,    47,    48,     7,     8,    49,    22,
      23,    24,    25,   122,   125,   126,    10,    11,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,   107,    68,    69,   114,
      70,    71,    72,    73,   102,    74,    75,    76,    97,    77,
      78,    79,    80,    13
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      19,    12,    88,    81,   138,    14,    15,    83,    12,    46,
       9,   123,    99,    93,    94,    96,   155,     9,   123,    21,
      95,    86,    87,   139,    85,    98,    18,    89,    90,    92,
     153,    46,   104,   105,   106,   156,    46,   110,   111,   112,
     113,    46,   160,   161,    28,    82,    29,   139,    27,   103,
      84,   108,   109,   129,   131,   132,   133,   120,   -27,   119,
     136,    29,   115,   116,   165,   135,   146,   147,   100,   142,
     117,   140,   149,   150,    16,   101,   171,   118,   137,   121,
     173,   119,   127,   134,   123,   143,   159,   157,   172,   136,
     145,   143,   143,   164,   175,   128,    17,   143,   143,   143,
     143,   143,   158,     1,    26,     2,     3,   167,   163,     4,
      20,     5,   148,     6,   151,   154,   162,     1,   152,     2,
       3,   168,   169,     4,   144,     5,     0,     6,     0,     0,
       0,   170,    27,   174,    18,    28,     0,     0,     0,    29,
       0,    30,    31,     0,    32,     0,     0,     0,     0,     0,
      46,    46,     0,    33,    34,    35,   127,     0,     1,     0,
       2,     3,    46,     0,     4,     0,     5,     0,     6,    36,
      37,    38,     0,     0,    39,     0,     0,    40,    41,     0,
       0,    42,    43,    44,    27,     0,    18,     0,     0,     0,
       0,    29,     0,    30,    31,     0,    32,     0,     0,     0,
       0,     0,     0,     0,     0,    33,    34,    35,     0,     0,
       1,     0,     2,     3,     0,     0,     4,     0,     5,     0,
       6,    36,    37,    38,     0,     0,    39,     0,     0,    40,
      41,     0,     0,    42,    43,    44,    27,     0,    18,     0,
       0,     0,    27,    29,    18,    30,    31,   130,    32,     0,
       0,    30,    31,     0,    32,     0,     0,    33,    34,    35,
       0,     0,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,    36,    37,    38,     0,     0,     0,    36,
      37,    38,    27,     0,    18,    28,     0,     0,    27,     0,
      18,    30,    31,   141,    32,     0,     0,    30,    31,     0,
      32,     0,     0,    33,    34,    35,     0,     0,     0,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    27,   123,    18,    36,    37,    38,    27,     0,
      18,    30,    31,     0,    32,     0,     0,    30,    31,     0,
      32,     0,     0,    33,    34,    35,   123,     0,     0,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,   166,     0,     0,    36,    37,    38,     0,     0,
       0,     1,     0,     2,     3,     0,     0,     4,     0,     5,
       1,     6,     2,     3,     0,  -102,     4,  -102,     5,     0,
       6,     0,  -102,     0,  -102,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,     0,     0,     0,  -102,  -102
};

static const yytype_int16 yycheck[] =
{
      10,     0,    41,    21,    94,    32,     5,    22,     7,    19,
       0,     4,    51,    44,    45,    46,     9,     7,     4,    41,
      46,    39,    40,     9,     7,    51,     5,    42,    43,    44,
     120,    41,    14,    15,    16,   125,    46,    18,    19,    20,
      21,    51,   132,   133,     6,     9,    10,     9,     3,    66,
      17,    12,    13,    84,    85,    86,    87,    75,     5,    74,
      91,    10,    22,    23,   154,    91,   108,   109,     7,   100,
      27,    97,   115,   116,     0,    17,   166,    28,    93,    42,
     170,    96,    81,    48,     4,   102,     8,    41,    46,   120,
     107,   108,   109,     8,    10,    82,     7,   114,   115,   116,
     117,   118,   127,    29,    12,    31,    32,   155,   139,    35,
      10,    37,   114,    39,   117,   120,   134,    29,   118,    31,
      32,   160,   161,    35,   102,    37,    -1,    39,    -1,    -1,
      -1,   162,     3,   172,     5,     6,    -1,    -1,    -1,    10,
      -1,    12,    13,    -1,    15,    -1,    -1,    -1,    -1,    -1,
     160,   161,    -1,    24,    25,    26,   155,    -1,    29,    -1,
      31,    32,   172,    -1,    35,    -1,    37,    -1,    39,    40,
      41,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      -1,    52,    53,    54,     3,    -1,     5,    -1,    -1,    -1,
      -1,    10,    -1,    12,    13,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    25,    26,    -1,    -1,
      29,    -1,    31,    32,    -1,    -1,    35,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    -1,    52,    53,    54,     3,    -1,     5,    -1,
      -1,    -1,     3,    10,     5,    12,    13,     8,    15,    -1,
      -1,    12,    13,    -1,    15,    -1,    -1,    24,    25,    26,
      -1,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    -1,    40,
      41,    42,     3,    -1,     5,     6,    -1,    -1,     3,    -1,
       5,    12,    13,     8,    15,    -1,    -1,    12,    13,    -1,
      15,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,    24,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      41,    42,     3,     4,     5,    40,    41,    42,     3,    -1,
       5,    12,    13,    -1,    15,    -1,    -1,    12,    13,    -1,
      15,    -1,    -1,    24,    25,    26,     4,    -1,    -1,    24,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      41,    42,    11,    -1,    -1,    40,    41,    42,    -1,    -1,
      -1,    29,    -1,    31,    32,    -1,    -1,    35,    -1,    37,
      29,    39,    31,    32,    -1,     7,    35,     9,    37,    -1,
      39,    -1,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    -1,    -1,    -1,    27,    28
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    29,    31,    32,    35,    37,    39,    61,    62,    63,
      71,    72,   107,   108,    32,   107,     0,    62,     5,    58,
      73,    41,    64,    65,    66,    67,    68,     3,     6,    10,
      12,    13,    15,    24,    25,    26,    40,    41,    42,    45,
      48,    49,    52,    53,    54,    56,    58,    59,    60,    63,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    92,    93,
      95,    96,    97,    98,   100,   101,   102,   104,   105,   106,
     107,    56,     9,    60,    17,     7,    56,    56,    75,    60,
      60,    58,    60,   100,   100,    59,   100,   103,    59,    75,
       7,    17,    99,    88,    14,    15,    16,    91,    12,    13,
      18,    19,    20,    21,    94,    22,    23,    27,    28,    60,
      56,    42,    68,     4,    57,    69,    70,   107,    65,   100,
       8,   100,   100,   100,    48,    59,   100,    60,    57,     9,
      59,     8,   100,    88,    97,    88,    90,    90,    92,    93,
      93,    95,    96,    57,   103,     9,    57,    41,    67,     8,
      57,    57,    56,   100,     8,    57,    11,    70,    75,    75,
     100,    57,    46,    57,    75,    10
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    55,    56,    57,    58,    59,    60,    61,    61,    62,
      62,    63,    64,    64,    65,    65,    66,    66,    67,    67,
      67,    68,    68,    68,    69,    69,    70,    71,    72,    73,
      73,    74,    74,    75,    75,    75,    75,    75,    75,    76,
      76,    77,    78,    79,    79,    79,    79,    79,    80,    80,
      81,    82,    83,    84,    85,    86,    86,    86,    86,    86,
      87,    87,    87,    88,    88,    89,    89,    89,    89,    89,
      89,    90,    90,    91,    91,    91,    92,    92,    92,    93,
      93,    94,    94,    94,    94,    95,    95,    95,    96,    96,
      97,    97,    98,    98,    99,   100,   101,   102,   102,   103,
     103,   104,   104,   105,   105,   106,   106,   107,   107,   108,
     108,   108,   108,   108,   108
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     3,     1,     3,     1,     3,     1,     1,     1,     3,
       4,     4,     6,     3,     1,     3,     2,     2,     2,     2,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     1,     1,     1,     1,     1,     1,     5,     7,
       5,     7,     2,     2,     1,     1,     1,     3,     1,     1,
       1,     4,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     1,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     1,     1,     1,     3,     4,     1,
       3,     3,     2,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* l_paren: L_PAREN  */
#line 94 "/workspace/SYsU-lang/parser/parser.y"
            {
    scope_manager_p->_state_stack.push_back(ScopeManager::PARM_READY_ST);
  }
#line 1368 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 3: /* r_paren: R_PAREN  */
#line 98 "/workspace/SYsU-lang/parser/parser.y"
            {
    if (!(scope_manager_p->_state_stack.back() == ScopeManager::PARM_CONTINUE_ST)) {
      scope_manager_p->_state_stack.pop_back();
    }
  }
#line 1378 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 4: /* l_brace: L_BRACE  */
#line 104 "/workspace/SYsU-lang/parser/parser.y"
            {
    if (scope_manager_p->_state_stack.back() == ScopeManager::PARM_CONTINUE_ST) {
        scope_manager_p->_state_stack.back() = ScopeManager::CONTINUE_ST;
    } else {
        scope_manager_p->_state_stack.push_back(ScopeManager::READY_ST);
    }
  }
#line 1390 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 5: /* r_brace: R_BRACE  */
#line 112 "/workspace/SYsU-lang/parser/parser.y"
            {
    if (scope_manager_p->_state_stack.back() == ScopeManager::CONTINUE_ST) {
        scope_manager_p->finalizeScope();
    }
    scope_manager_p->_state_stack.pop_back();
  }
#line 1401 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 6: /* semi: SEMI  */
#line 120 "/workspace/SYsU-lang/parser/parser.y"
         {
    if (scope_manager_p->_state_stack.back() == ScopeManager::PARM_CONTINUE_ST){
      scope_manager_p->finalizeScope();
      scope_manager_p->_state_stack.pop_back();
    }
    else if(scope_manager_p->_state_stack.back() == ScopeManager::PARM_READY_ST){
        scope_manager_p->_state_stack.pop_back();
    }
  }
#line 1415 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 7: /* translation_unit_decl: external_decl  */
#line 131 "/workspace/SYsU-lang/parser/parser.y"
                        {
    trans_unit_decl_p->push_back((yyvsp[0].ExternalDecl));
  }
#line 1423 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 8: /* translation_unit_decl: translation_unit_decl external_decl  */
#line 134 "/workspace/SYsU-lang/parser/parser.y"
                                              {
    trans_unit_decl_p->push_back((yyvsp[0].ExternalDecl));
  }
#line 1431 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 9: /* external_decl: init_decl_list  */
#line 140 "/workspace/SYsU-lang/parser/parser.y"
                   {
    (yyval.ExternalDecl) = Mgr::g.make<ExternalDecl>(0,(yyvsp[0].InitDeclList),std::nullptr_t());
  }
#line 1439 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 10: /* external_decl: function_decl  */
#line 143 "/workspace/SYsU-lang/parser/parser.y"
                  {
    (yyval.ExternalDecl) = Mgr::g.make<ExternalDecl>(1,std::nullptr_t(),(yyvsp[0].FunctionDecl));

  }
#line 1448 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 11: /* init_decl_list: type_spec init_decltor_list semi  */
#line 151 "/workspace/SYsU-lang/parser/parser.y"
                                     {
    (yyval.InitDeclList) = Mgr::g.make<InitDeclList>((yyvsp[-2].TypeSpec)->type,(yyvsp[-1].InitDecltorList));
    
  }
#line 1457 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 12: /* init_decltor_list: init_decltor  */
#line 157 "/workspace/SYsU-lang/parser/parser.y"
                 {
    (yyval.InitDecltorList) = Mgr::g.make<InitDecltorList>();
    (yyval.InitDecltorList)->push_back((yyvsp[0].InitDecltor));
  }
#line 1466 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 13: /* init_decltor_list: init_decltor_list COMMA init_decltor  */
#line 161 "/workspace/SYsU-lang/parser/parser.y"
                                         {
    (yyvsp[-2].InitDecltorList)->push_back((yyvsp[0].InitDecltor));
    (yyval.InitDecltorList) = (yyvsp[-2].InitDecltorList);
  }
#line 1475 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 14: /* init_decltor: decltor  */
#line 167 "/workspace/SYsU-lang/parser/parser.y"
            {
    (yyval.InitDecltor) = Mgr::g.make<InitDecltor>(0,(yyvsp[0].Decltor),std::nullptr_t()); 
  }
#line 1483 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 15: /* init_decltor: decltor EQUAL expr  */
#line 170 "/workspace/SYsU-lang/parser/parser.y"
                       {
    (yyval.InitDecltor) = Mgr::g.make<InitDecltor>(1,(yyvsp[-2].Decltor),(yyvsp[0].Expr));   
  }
#line 1491 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 16: /* decltor: var_decltor  */
#line 176 "/workspace/SYsU-lang/parser/parser.y"
                {
    (yyval.Decltor) = (yyvsp[0].VarDecltor);
    (yyval.Decltor)->flag = 0;
  }
#line 1500 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 17: /* decltor: function_decltor  */
#line 180 "/workspace/SYsU-lang/parser/parser.y"
                     {
    (yyval.Decltor) = (yyvsp[0].FunctionDecltor);
    (yyval.Decltor)->flag = 1;
  }
#line 1509 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 18: /* var_decltor: IDENTIFIER  */
#line 186 "/workspace/SYsU-lang/parser/parser.y"
              {
    (yyval.VarDecltor) = Mgr::g.make<VarDecltor>((yyvsp[0].Str)->s);
    (yyval.VarDecltor)->is_ptr = false;
    delete (yyvsp[0].Str);
  }
#line 1519 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 19: /* var_decltor: var_decltor L_SQUARE R_SQUARE  */
#line 191 "/workspace/SYsU-lang/parser/parser.y"
                                 {
    (yyval.VarDecltor) = (yyvsp[-2].VarDecltor);
    //$$->is_ptr = true;
    (yyval.VarDecltor)->dims.push_back(0);// in parmdecl, it becomnse *
  }
#line 1529 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 20: /* var_decltor: var_decltor L_SQUARE expr R_SQUARE  */
#line 196 "/workspace/SYsU-lang/parser/parser.y"
                                      {
    (yyval.VarDecltor) = (yyvsp[-3].VarDecltor);
    (yyval.VarDecltor)->dims.push_back((yyvsp[-1].Expr)->value);
  }
#line 1538 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 21: /* function_decltor: IDENTIFIER l_paren parm_var_decl_list r_paren  */
#line 202 "/workspace/SYsU-lang/parser/parser.y"
                                                  {
    (yyval.FunctionDecltor) = Mgr::g.make<FunctionDecltor>(1,(yyvsp[-3].Str)->s,(yyvsp[-1].ParmVarDeclList));
    delete (yyvsp[-3].Str);
  }
#line 1547 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 22: /* function_decltor: IDENTIFIER l_paren parm_var_decl_list COMMA ELLIPSIS r_paren  */
#line 206 "/workspace/SYsU-lang/parser/parser.y"
                                                                 {
    (yyval.FunctionDecltor) = Mgr::g.make<FunctionDecltor>(1,(yyvsp[-5].Str)->s,(yyvsp[-3].ParmVarDeclList));
    (yyval.FunctionDecltor)->type.is_ellipsis = true;
    delete (yyvsp[-5].Str);
  }
#line 1557 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 23: /* function_decltor: IDENTIFIER l_paren r_paren  */
#line 211 "/workspace/SYsU-lang/parser/parser.y"
                               {
    (yyval.FunctionDecltor) = Mgr::g.make<FunctionDecltor>(0,(yyvsp[-2].Str)->s,std::nullptr_t());
    delete (yyvsp[-2].Str);
  }
#line 1566 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 24: /* parm_var_decl_list: parm_var_decl  */
#line 218 "/workspace/SYsU-lang/parser/parser.y"
                  {
    (yyval.ParmVarDeclList) = Mgr::g.make<ParmVarDeclList>();
    (yyval.ParmVarDeclList)->push_back((yyvsp[0].ParmVarDecl));
  }
#line 1575 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 25: /* parm_var_decl_list: parm_var_decl_list COMMA parm_var_decl  */
#line 222 "/workspace/SYsU-lang/parser/parser.y"
                                           {
    (yyvsp[-2].ParmVarDeclList)->push_back((yyvsp[0].ParmVarDecl));
    (yyval.ParmVarDeclList) = (yyvsp[-2].ParmVarDeclList);
  }
#line 1584 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 26: /* parm_var_decl: type_spec var_decltor  */
#line 228 "/workspace/SYsU-lang/parser/parser.y"
                          {
    (yyval.ParmVarDecl) = Mgr::g.make<ParmVarDecl>( (yyvsp[-1].TypeSpec)->type
                                      ,(yyvsp[0].VarDecltor));
  }
#line 1593 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 27: /* function_head: type_spec function_decltor  */
#line 234 "/workspace/SYsU-lang/parser/parser.y"
                               {
    (yyval.FunctionDecl) = Mgr::g.make<FunctionDecl>(0,std::nullptr_t(),(yyvsp[0].FunctionDecltor),(yyvsp[-1].TypeSpec)->type.base_type);
  }
#line 1601 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 28: /* function_decl: function_head compound_stmt  */
#line 239 "/workspace/SYsU-lang/parser/parser.y"
                                {
    (yyval.FunctionDecl) = (yyvsp[-1].FunctionDecl);
    (yyval.FunctionDecl)->flag = 1;
    (yyval.FunctionDecl)->compound_stmt_p = (yyvsp[0].CompoundStmt);
  }
#line 1611 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 29: /* compound_stmt: l_brace r_brace  */
#line 246 "/workspace/SYsU-lang/parser/parser.y"
                    {
    (yyval.CompoundStmt) = Mgr::g.make<CompoundStmt>(0,std::nullptr_t());
  }
#line 1619 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 30: /* compound_stmt: l_brace stmt_list r_brace  */
#line 249 "/workspace/SYsU-lang/parser/parser.y"
                              {
    (yyval.CompoundStmt) = Mgr::g.make<CompoundStmt>(1,(yyvsp[-1].StmtList));
  }
#line 1627 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 31: /* stmt_list: stmt  */
#line 254 "/workspace/SYsU-lang/parser/parser.y"
         {
    (yyval.StmtList) = Mgr::g.make<StmtList>();
    (yyval.StmtList)->push_back((yyvsp[0].Stmt));
  }
#line 1636 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 32: /* stmt_list: stmt_list stmt  */
#line 258 "/workspace/SYsU-lang/parser/parser.y"
                  {
    (yyvsp[-1].StmtList)->push_back((yyvsp[0].Stmt));
    (yyval.StmtList) = (yyvsp[-1].StmtList);
  }
#line 1645 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 33: /* stmt: return_stmt  */
#line 264 "/workspace/SYsU-lang/parser/parser.y"
                {
    (yyval.Stmt) = (yyvsp[0].ReturnStmt); 
  }
#line 1653 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 34: /* stmt: expr_stmt  */
#line 267 "/workspace/SYsU-lang/parser/parser.y"
             {
    (yyval.Stmt) = (yyvsp[0].ExprStmt);
  }
#line 1661 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 35: /* stmt: decl_stmt  */
#line 270 "/workspace/SYsU-lang/parser/parser.y"
             {
    (yyval.Stmt) = (yyvsp[0].DeclStmt);
  }
#line 1669 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 36: /* stmt: compound_stmt  */
#line 273 "/workspace/SYsU-lang/parser/parser.y"
                  {
    (yyval.Stmt) = (yyvsp[0].CompoundStmt);
  }
#line 1677 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 37: /* stmt: control_stmt  */
#line 276 "/workspace/SYsU-lang/parser/parser.y"
                 {
    (yyval.Stmt) = (yyvsp[0].Stmt);
  }
#line 1685 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 38: /* stmt: null_stmt  */
#line 279 "/workspace/SYsU-lang/parser/parser.y"
              {
    (yyval.Stmt) = (yyvsp[0].NullStmt);
  }
#line 1693 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 39: /* return_stmt: RETURN expr semi  */
#line 286 "/workspace/SYsU-lang/parser/parser.y"
  {
    (yyval.ReturnStmt) = Mgr::g.make<ReturnStmt>(1,(yyvsp[-1].Expr));
  }
#line 1701 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 40: /* return_stmt: RETURN semi  */
#line 289 "/workspace/SYsU-lang/parser/parser.y"
                {
    (yyval.ReturnStmt) = Mgr::g.make<ReturnStmt>(0,std::nullptr_t());
  }
#line 1709 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 41: /* expr_stmt: expr semi  */
#line 297 "/workspace/SYsU-lang/parser/parser.y"
              {
    (yyval.ExprStmt) = Mgr::g.make<ExprStmt>((yyvsp[-1].Expr));

  }
#line 1718 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 42: /* decl_stmt: init_decl_list  */
#line 303 "/workspace/SYsU-lang/parser/parser.y"
                   {
    (yyval.DeclStmt) = Mgr::g.make<DeclStmt>((yyvsp[0].InitDeclList));  
  }
#line 1726 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 43: /* control_stmt: if_stmt  */
#line 308 "/workspace/SYsU-lang/parser/parser.y"
           {
    (yyval.Stmt) = (yyvsp[0].IfStmt);
  }
#line 1734 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 44: /* control_stmt: while_stmt  */
#line 311 "/workspace/SYsU-lang/parser/parser.y"
              {
    (yyval.Stmt) = (yyvsp[0].WhileStmt);
  }
#line 1742 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 45: /* control_stmt: break_stmt  */
#line 314 "/workspace/SYsU-lang/parser/parser.y"
              {
    (yyval.Stmt) = (yyvsp[0].BreakStmt);
  }
#line 1750 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 46: /* control_stmt: continue_stmt  */
#line 317 "/workspace/SYsU-lang/parser/parser.y"
                 {
    (yyval.Stmt) = (yyvsp[0].ContinueStmt);
  }
#line 1758 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 47: /* control_stmt: do_stmt  */
#line 320 "/workspace/SYsU-lang/parser/parser.y"
            {
    (yyval.Stmt) = (yyvsp[0].DoStmt);
  }
#line 1766 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 48: /* if_stmt: IF l_paren expr r_paren stmt  */
#line 326 "/workspace/SYsU-lang/parser/parser.y"
                                 {
    (yyval.IfStmt) = Mgr::g.make<IfStmt>(0,(yyvsp[-2].Expr),(yyvsp[0].Stmt),std::nullptr_t());
  }
#line 1774 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 49: /* if_stmt: IF l_paren expr r_paren stmt ELSE stmt  */
#line 329 "/workspace/SYsU-lang/parser/parser.y"
                                           {
    (yyval.IfStmt) = Mgr::g.make<IfStmt>(1,(yyvsp[-4].Expr),(yyvsp[-2].Stmt),(yyvsp[0].Stmt));
  }
#line 1782 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 50: /* while_stmt: WHILE l_paren expr r_paren stmt  */
#line 334 "/workspace/SYsU-lang/parser/parser.y"
                                   {
    (yyval.WhileStmt) = Mgr::g.make<WhileStmt>((yyvsp[-2].Expr),(yyvsp[0].Stmt));
  }
#line 1790 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 51: /* do_stmt: DO stmt WHILE l_paren expr r_paren SEMI  */
#line 339 "/workspace/SYsU-lang/parser/parser.y"
                                           {
    (yyval.DoStmt) = Mgr::g.make<DoStmt>((yyvsp[-2].Expr),(yyvsp[-5].Stmt));
  }
#line 1798 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 52: /* break_stmt: BREAK semi  */
#line 344 "/workspace/SYsU-lang/parser/parser.y"
              {
  (yyval.BreakStmt) = Mgr::g.make<BreakStmt>();
}
#line 1806 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 53: /* continue_stmt: CONTINUE semi  */
#line 349 "/workspace/SYsU-lang/parser/parser.y"
                 {
  (yyval.ContinueStmt) = Mgr::g.make<ContinueStmt>();
}
#line 1814 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 54: /* null_stmt: semi  */
#line 354 "/workspace/SYsU-lang/parser/parser.y"
        {
  (yyval.NullStmt) = Mgr::g.make<NullStmt>();
}
#line 1822 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 55: /* primary_expr: decl_ref_expr  */
#line 362 "/workspace/SYsU-lang/parser/parser.y"
                        { (yyval.Expr) = (yyvsp[0].DeclRefExpr);}
#line 1828 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 56: /* primary_expr: literal  */
#line 363 "/workspace/SYsU-lang/parser/parser.y"
            { (yyval.Expr) = (yyvsp[0].Expr);}
#line 1834 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 57: /* primary_expr: l_paren expr r_paren  */
#line 364 "/workspace/SYsU-lang/parser/parser.y"
                               { 
    (yyval.Expr) = Mgr::g.make<ParenExpr>((yyvsp[-1].Expr));
  }
#line 1842 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 58: /* primary_expr: init_list_expr  */
#line 367 "/workspace/SYsU-lang/parser/parser.y"
                   { (yyval.Expr) = (yyvsp[0].InitListExpr);}
#line 1848 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 59: /* primary_expr: call_expr  */
#line 368 "/workspace/SYsU-lang/parser/parser.y"
              { (yyval.Expr) = (yyvsp[0].CallExpr); }
#line 1854 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 60: /* postfix_expr: primary_expr  */
#line 372 "/workspace/SYsU-lang/parser/parser.y"
                       { (yyval.Expr) = (yyvsp[0].Expr);}
#line 1860 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 61: /* postfix_expr: postfix_expr L_SQUARE expr R_SQUARE  */
#line 373 "/workspace/SYsU-lang/parser/parser.y"
                                              {
    (yyval.Expr) = Mgr::g.make<ArraySubscriptExpr>(1,(yyvsp[-3].Expr),(yyvsp[-1].Expr));
  }
#line 1868 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 62: /* postfix_expr: postfix_expr L_SQUARE R_SQUARE  */
#line 376 "/workspace/SYsU-lang/parser/parser.y"
                                   {
    (yyval.Expr) = Mgr::g.make<ArraySubscriptExpr>(0,(yyvsp[-2].Expr),std::nullptr_t());
  }
#line 1876 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 63: /* unary_expr: postfix_expr  */
#line 385 "/workspace/SYsU-lang/parser/parser.y"
                       {(yyval.Expr) = (yyvsp[0].Expr);}
#line 1882 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 64: /* unary_expr: unary_op unary_expr  */
#line 386 "/workspace/SYsU-lang/parser/parser.y"
                       {
    (yyval.Expr) = Mgr::g.make<UnaryOperator>( (yyvsp[-1].Str)->s,
                                      (yyvsp[0].Expr));
    delete (yyvsp[-1].Str);
  }
#line 1892 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 65: /* unary_op: AMP  */
#line 394 "/workspace/SYsU-lang/parser/parser.y"
              {(yyval.Str) = new Str("&");}
#line 1898 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 66: /* unary_op: STAR  */
#line 395 "/workspace/SYsU-lang/parser/parser.y"
         {(yyval.Str) = new Str("*");}
#line 1904 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 67: /* unary_op: PLUS  */
#line 396 "/workspace/SYsU-lang/parser/parser.y"
               {(yyval.Str) = new Str("+");}
#line 1910 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 68: /* unary_op: MINUS  */
#line 397 "/workspace/SYsU-lang/parser/parser.y"
                {(yyval.Str) = new Str("-");}
#line 1916 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 69: /* unary_op: TILDE  */
#line 398 "/workspace/SYsU-lang/parser/parser.y"
                {(yyval.Str) = new Str("~");}
#line 1922 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 70: /* unary_op: EXCLAIM  */
#line 399 "/workspace/SYsU-lang/parser/parser.y"
                  {(yyval.Str) = new Str("!");}
#line 1928 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 71: /* mul_div_expr: unary_expr  */
#line 406 "/workspace/SYsU-lang/parser/parser.y"
                     { (yyval.Expr) = (yyvsp[0].Expr);}
#line 1934 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 72: /* mul_div_expr: mul_div_expr mul_div_op unary_expr  */
#line 407 "/workspace/SYsU-lang/parser/parser.y"
                                             {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( (yyvsp[-1].Str)->s,
                                          (yyvsp[-2].Expr),(yyvsp[0].Expr));
    delete (yyvsp[-1].Str);
  }
#line 1944 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 73: /* mul_div_op: STAR  */
#line 415 "/workspace/SYsU-lang/parser/parser.y"
        { (yyval.Str) = new Str("*");}
#line 1950 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 74: /* mul_div_op: SLASH  */
#line 416 "/workspace/SYsU-lang/parser/parser.y"
         { (yyval.Str) = new Str("/");}
#line 1956 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 75: /* mul_div_op: PERCENT  */
#line 417 "/workspace/SYsU-lang/parser/parser.y"
            { (yyval.Str) = new Str("%");}
#line 1962 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 76: /* add_sub_expr: mul_div_expr  */
#line 421 "/workspace/SYsU-lang/parser/parser.y"
                       {(yyval.Expr) = (yyvsp[0].Expr);}
#line 1968 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 77: /* add_sub_expr: add_sub_expr PLUS mul_div_expr  */
#line 422 "/workspace/SYsU-lang/parser/parser.y"
                                         {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( std::string("+"),
                                          (yyvsp[-2].Expr),(yyvsp[0].Expr));
  }
#line 1977 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 78: /* add_sub_expr: add_sub_expr MINUS mul_div_expr  */
#line 426 "/workspace/SYsU-lang/parser/parser.y"
                                         {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( std::string("-"),
                                          (yyvsp[-2].Expr),(yyvsp[0].Expr));
  }
#line 1986 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 79: /* relational_expr: add_sub_expr  */
#line 434 "/workspace/SYsU-lang/parser/parser.y"
                       {(yyval.Expr) = (yyvsp[0].Expr);}
#line 1992 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 80: /* relational_expr: relational_expr relational_op add_sub_expr  */
#line 435 "/workspace/SYsU-lang/parser/parser.y"
                                                     {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( (yyvsp[-1].Str)->s,
                                          (yyvsp[-2].Expr),(yyvsp[0].Expr));
    delete (yyvsp[-1].Str);
  }
#line 2002 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 81: /* relational_op: GREATER  */
#line 443 "/workspace/SYsU-lang/parser/parser.y"
            {(yyval.Str) = new Str(">");}
#line 2008 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 82: /* relational_op: LESS  */
#line 444 "/workspace/SYsU-lang/parser/parser.y"
            {(yyval.Str) = new Str("<");}
#line 2014 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 83: /* relational_op: GREATEREQUAL  */
#line 445 "/workspace/SYsU-lang/parser/parser.y"
                 {(yyval.Str) = new Str(">=");}
#line 2020 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 84: /* relational_op: LESSEQUAL  */
#line 446 "/workspace/SYsU-lang/parser/parser.y"
                 {(yyval.Str) = new Str("<=");}
#line 2026 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 85: /* equality_expr: relational_expr  */
#line 450 "/workspace/SYsU-lang/parser/parser.y"
                         {(yyval.Expr) = (yyvsp[0].Expr);}
#line 2032 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 86: /* equality_expr: equality_expr EQUALEQUAL relational_expr  */
#line 451 "/workspace/SYsU-lang/parser/parser.y"
                                            {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( std::string("=="),
                                      (yyvsp[-2].Expr),(yyvsp[0].Expr));
  }
#line 2041 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 87: /* equality_expr: equality_expr EXCLAIMEQUAL relational_expr  */
#line 455 "/workspace/SYsU-lang/parser/parser.y"
                                                    {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( std::string("!="),
                                      (yyvsp[-2].Expr),(yyvsp[0].Expr));
  }
#line 2050 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 88: /* logical_and_expr: equality_expr  */
#line 464 "/workspace/SYsU-lang/parser/parser.y"
                       {(yyval.Expr) = (yyvsp[0].Expr);}
#line 2056 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 89: /* logical_and_expr: logical_and_expr AMPAMP equality_expr  */
#line 465 "/workspace/SYsU-lang/parser/parser.y"
                                               {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( std::string("&&"),
                                  (yyvsp[-2].Expr),(yyvsp[0].Expr));
  }
#line 2065 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 90: /* logical_or_expr: logical_and_expr  */
#line 473 "/workspace/SYsU-lang/parser/parser.y"
                          {(yyval.Expr) = (yyvsp[0].Expr);}
#line 2071 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 91: /* logical_or_expr: logical_or_expr PIPEPIPE logical_and_expr  */
#line 474 "/workspace/SYsU-lang/parser/parser.y"
                                                    {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( std::string("||"),
                                  (yyvsp[-2].Expr),(yyvsp[0].Expr));
  }
#line 2080 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 92: /* assign_expr: logical_or_expr  */
#line 481 "/workspace/SYsU-lang/parser/parser.y"
                          {(yyval.Expr) = (yyvsp[0].Expr);}
#line 2086 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 93: /* assign_expr: unary_expr assign_operator logical_or_expr  */
#line 482 "/workspace/SYsU-lang/parser/parser.y"
                                                    {
    (yyval.Expr) = Mgr::g.make<BinaryOperator>( std::string("="),
                                          (yyvsp[-2].Expr),
                                          (yyvsp[0].Expr));
  }
#line 2096 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 95: /* expr: assign_expr  */
#line 506 "/workspace/SYsU-lang/parser/parser.y"
                {(yyval.Expr) = (yyvsp[0].Expr); }
#line 2102 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 96: /* decl_ref_expr: IDENTIFIER  */
#line 511 "/workspace/SYsU-lang/parser/parser.y"
               {
    (yyval.DeclRefExpr) = Mgr::g.make<DeclRefExpr>((yyvsp[0].Str)->s);
    delete (yyvsp[0].Str);
  }
#line 2111 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 97: /* call_expr: decl_ref_expr l_paren r_paren  */
#line 517 "/workspace/SYsU-lang/parser/parser.y"
                                  {
    (yyval.CallExpr) = Mgr::g.make<CallExpr>(0,(yyvsp[-2].DeclRefExpr),std::nullptr_t());
  }
#line 2119 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 98: /* call_expr: decl_ref_expr l_paren expr_list r_paren  */
#line 520 "/workspace/SYsU-lang/parser/parser.y"
                                            {
    (yyval.CallExpr) = Mgr::g.make<CallExpr>(1,(yyvsp[-3].DeclRefExpr),(yyvsp[-1].ExprList));
  }
#line 2127 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 99: /* expr_list: expr  */
#line 525 "/workspace/SYsU-lang/parser/parser.y"
         {
    (yyval.ExprList) = Mgr::g.make<ExprList>();
    (yyval.ExprList)->push_back((yyvsp[0].Expr));
  }
#line 2136 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 100: /* expr_list: expr_list COMMA expr  */
#line 529 "/workspace/SYsU-lang/parser/parser.y"
                         {
    (yyvsp[-2].ExprList)->push_back((yyvsp[0].Expr));
    (yyval.ExprList) = (yyvsp[-2].ExprList);
  }
#line 2145 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 101: /* init_list_expr: l_brace expr_list r_brace  */
#line 536 "/workspace/SYsU-lang/parser/parser.y"
                              {
    (yyval.InitListExpr) = Mgr::g.make<InitListExpr>((yyvsp[-1].ExprList));
  }
#line 2153 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 102: /* init_list_expr: l_brace r_brace  */
#line 539 "/workspace/SYsU-lang/parser/parser.y"
                    {
    (yyval.InitListExpr) = Mgr::g.make<InitListExpr>(Mgr::g.make<ExprList>());
  }
#line 2161 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 103: /* literal: NUMERIC_CONSTANT  */
#line 544 "/workspace/SYsU-lang/parser/parser.y"
                     {
    std::string serial = (yyvsp[0].Str)->s;
    if (serial.find('.') != std::string::npos || 
        serial.find('p') != std::string::npos ||
        serial.find('e') != std::string::npos) {
        (yyval.Expr) = Mgr::g.make<FloatingLiteral>(serial);
    }
    else{
      (yyval.Expr) = Mgr::g.make<IntegerLiteral>(serial);
    }
    delete (yyvsp[0].Str);
  }
#line 2178 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 104: /* literal: string_literal  */
#line 556 "/workspace/SYsU-lang/parser/parser.y"
                   {
    (yyvsp[0].StringLiteral)->get_count();
    (yyval.Expr) = (yyvsp[0].StringLiteral);
  }
#line 2187 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 105: /* string_literal: STRING_LITERAL  */
#line 562 "/workspace/SYsU-lang/parser/parser.y"
                   {
    (yyval.StringLiteral) = Mgr::g.make<StringLiteral>();
    (yyval.StringLiteral)->serial = (yyvsp[0].Str)->s;
  }
#line 2196 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 106: /* string_literal: string_literal STRING_LITERAL  */
#line 566 "/workspace/SYsU-lang/parser/parser.y"
                                  {
    (yyvsp[-1].StringLiteral)->serial = (yyvsp[-1].StringLiteral)->serial.substr(0, (yyvsp[-1].StringLiteral)->serial.size() - 1) + (yyvsp[0].Str)->s.substr(1, (yyvsp[0].Str)->s.size() - 1);
    (yyval.StringLiteral) = (yyvsp[-1].StringLiteral);
    delete (yyvsp[0].Str);
  }
#line 2206 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 107: /* type_spec: base_type  */
#line 573 "/workspace/SYsU-lang/parser/parser.y"
              {
    (yyval.TypeSpec) = Mgr::g.make<TypeSpec>((yyvsp[0].BaseType),false);
  }
#line 2214 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 108: /* type_spec: CONST type_spec  */
#line 576 "/workspace/SYsU-lang/parser/parser.y"
                    {
    (yyval.TypeSpec) = (yyvsp[0].TypeSpec);
    (yyval.TypeSpec)->type.is_const = true;
  }
#line 2223 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 109: /* base_type: INT  */
#line 582 "/workspace/SYsU-lang/parser/parser.y"
        {
    (yyval.BaseType) = Type::INT;
  }
#line 2231 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 110: /* base_type: VOID  */
#line 585 "/workspace/SYsU-lang/parser/parser.y"
         {
    (yyval.BaseType) = Type::VOID;
  }
#line 2239 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 111: /* base_type: FLOAT  */
#line 588 "/workspace/SYsU-lang/parser/parser.y"
          {
    (yyval.BaseType) = Type::FLOAT;
  }
#line 2247 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 112: /* base_type: CHAR  */
#line 591 "/workspace/SYsU-lang/parser/parser.y"
         {
    (yyval.BaseType) = Type::CHAR;
  }
#line 2255 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 113: /* base_type: LONG  */
#line 594 "/workspace/SYsU-lang/parser/parser.y"
         {
    (yyval.BaseType) = Type::LONG;
  }
#line 2263 "/workspace/SYsU-lang/parser/parser.cc"
    break;

  case 114: /* base_type: LONG LONG  */
#line 597 "/workspace/SYsU-lang/parser/parser.y"
              {
    (yyval.BaseType) = Type::LONG_LONG;
  }
#line 2271 "/workspace/SYsU-lang/parser/parser.cc"
    break;


#line 2275 "/workspace/SYsU-lang/parser/parser.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 601 "/workspace/SYsU-lang/parser/parser.y"



