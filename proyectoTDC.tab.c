/* A Bison parser, made by GNU Bison 3.8.2.  */

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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "proyectoTDC.y"

#include <stdio.h>
#include <string.h>

void yyerror(const char *mensaje);
int  yylex(void);
extern int  yylineno;
extern char *yytext;

/* Archivos de salida en paralelo */
FILE *tac_out = NULL;
FILE *asm_out = NULL;

static int temp_count = 1;    /* contador de temporales: t1, t2, ...  */
static int label_count = 1;   /* contador de etiquetas:  l1, l2, ...  */

void nuevaTemp(char *temporal)
{
    sprintf(temporal, "t%d", temp_count++);
}

void nuevaEtiqueta(char *etiqueta)
{
    sprintf(etiqueta, "l%d", label_count++);
}

/* Pila LIFO de etiquetas */
#define MAX_ETIQUETAS 100
static char pilaEtiquetas[MAX_ETIQUETAS][50];
static int  topeEtiquetas = 0;

void apilarEtiqueta(const char *etiqueta)
{
    if (topeEtiquetas < MAX_ETIQUETAS)
        strcpy(pilaEtiquetas[topeEtiquetas++], etiqueta);
}

void desapilarEtiqueta(char *etiqueta)
{
    if (topeEtiquetas > 0)
        strcpy(etiqueta, pilaEtiquetas[--topeEtiquetas]);
    else
        etiqueta[0] = '\0';
}

#line 117 "proyectoTDC.tab.c"

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

#include "proyectoTDC.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ALLOCATE = 3,                   /* ALLOCATE  */
  YYSYMBOL_MAIN = 4,                       /* MAIN  */
  YYSYMBOL_INICIO_BLOQUE = 5,              /* INICIO_BLOQUE  */
  YYSYMBOL_END = 6,                        /* END  */
  YYSYMBOL_INPUT = 7,                      /* INPUT  */
  YYSYMBOL_OUTPUT = 8,                     /* OUTPUT  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_THEN = 10,                      /* THEN  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_AND = 12,                       /* AND  */
  YYSYMBOL_OR = 13,                        /* OR  */
  YYSYMBOL_ASIGNACION = 14,                /* ASIGNACION  */
  YYSYMBOL_MAYOR_IGUAL = 15,               /* MAYOR_IGUAL  */
  YYSYMBOL_MENOR_IGUAL = 16,               /* MENOR_IGUAL  */
  YYSYMBOL_COMPARACION_IGUAL = 17,         /* COMPARACION_IGUAL  */
  YYSYMBOL_DIFERENTE = 18,                 /* DIFERENTE  */
  YYSYMBOL_POTENCIA = 19,                  /* POTENCIA  */
  YYSYMBOL_DOS_PUNTOS = 20,                /* DOS_PUNTOS  */
  YYSYMBOL_PUNTO_COMA = 21,                /* PUNTO_COMA  */
  YYSYMBOL_MAS = 22,                       /* MAS  */
  YYSYMBOL_MENOS = 23,                     /* MENOS  */
  YYSYMBOL_POR = 24,                       /* POR  */
  YYSYMBOL_ENTRE = 25,                     /* ENTRE  */
  YYSYMBOL_PAR_IZQ = 26,                   /* PAR_IZQ  */
  YYSYMBOL_PAR_DER = 27,                   /* PAR_DER  */
  YYSYMBOL_MENOR = 28,                     /* MENOR  */
  YYSYMBOL_MAYOR = 29,                     /* MAYOR  */
  YYSYMBOL_DO = 30,                        /* DO  */
  YYSYMBOL_WHILE = 31,                     /* WHILE  */
  YYSYMBOL_FOR = 32,                       /* FOR  */
  YYSYMBOL_TO = 33,                        /* TO  */
  YYSYMBOL_ID = 34,                        /* ID  */
  YYSYMBOL_NUMERO = 35,                    /* NUMERO  */
  YYSYMBOL_INVALIDO = 36,                  /* INVALIDO  */
  YYSYMBOL_YYACCEPT = 37,                  /* $accept  */
  YYSYMBOL_inicio = 38,                    /* inicio  */
  YYSYMBOL_39_1 = 39,                      /* $@1  */
  YYSYMBOL_listaSentencias = 40,           /* listaSentencias  */
  YYSYMBOL_sentencia = 41,                 /* sentencia  */
  YYSYMBOL_declaracionVariables = 42,      /* declaracionVariables  */
  YYSYMBOL_listaDeNombres = 43,            /* listaDeNombres  */
  YYSYMBOL_asignacion = 44,                /* asignacion  */
  YYSYMBOL_lectura = 45,                   /* lectura  */
  YYSYMBOL_escritura = 46,                 /* escritura  */
  YYSYMBOL_condicional = 47,               /* condicional  */
  YYSYMBOL_48_2 = 48,                      /* $@2  */
  YYSYMBOL_cabeceraIf = 49,                /* cabeceraIf  */
  YYSYMBOL_bloque = 50,                    /* bloque  */
  YYSYMBOL_cicloWhile = 51,                /* cicloWhile  */
  YYSYMBOL_52_3 = 52,                      /* $@3  */
  YYSYMBOL_53_4 = 53,                      /* $@4  */
  YYSYMBOL_cicloDoWhile = 54,              /* cicloDoWhile  */
  YYSYMBOL_55_5 = 55,                      /* $@5  */
  YYSYMBOL_cicloFor = 56,                  /* cicloFor  */
  YYSYMBOL_cabeceraFor = 57,               /* cabeceraFor  */
  YYSYMBOL_limiteInferior = 58,            /* limiteInferior  */
  YYSYMBOL_limiteSuperior = 59,            /* limiteSuperior  */
  YYSYMBOL_condicionLogica = 60,           /* condicionLogica  */
  YYSYMBOL_comparacionSimple = 61,         /* comparacionSimple  */
  YYSYMBOL_expresion = 62,                 /* expresion  */
  YYSYMBOL_termino = 63,                   /* termino  */
  YYSYMBOL_potenciaNum = 64,               /* potenciaNum  */
  YYSYMBOL_factorBasico = 65               /* factorBasico  */
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
typedef yytype_int8 yy_state_t;

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

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   113

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  56
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  113

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


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
      35,    36
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    70,    70,    69,   127,   128,   132,   133,   134,   135,
     136,   137,   138,   139,   144,   148,   153,   161,   174,   189,
     204,   213,   212,   239,   250,   264,   270,   280,   269,   309,
     308,   332,   352,   390,   395,   399,   400,   405,   406,   418,
     432,   448,   464,   480,   496,   512,   532,   543,   554,   559,
     570,   582,   587,   606,   611,   613,   615
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
  "\"end of file\"", "error", "\"invalid token\"", "ALLOCATE", "MAIN",
  "INICIO_BLOQUE", "END", "INPUT", "OUTPUT", "IF", "THEN", "ELSE", "AND",
  "OR", "ASIGNACION", "MAYOR_IGUAL", "MENOR_IGUAL", "COMPARACION_IGUAL",
  "DIFERENTE", "POTENCIA", "DOS_PUNTOS", "PUNTO_COMA", "MAS", "MENOS",
  "POR", "ENTRE", "PAR_IZQ", "PAR_DER", "MENOR", "MAYOR", "DO", "WHILE",
  "FOR", "TO", "ID", "NUMERO", "INVALIDO", "$accept", "inicio", "$@1",
  "listaSentencias", "sentencia", "declaracionVariables", "listaDeNombres",
  "asignacion", "lectura", "escritura", "condicional", "$@2", "cabeceraIf",
  "bloque", "cicloWhile", "$@3", "$@4", "cicloDoWhile", "$@5", "cicloFor",
  "cabeceraFor", "limiteInferior", "limiteSuperior", "condicionLogica",
  "comparacionSimple", "expresion", "termino", "potenciaNum",
  "factorBasico", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-29)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       1,     2,    35,   -29,   -29,   -29,    11,   -29,   -18,    10,
     -20,   -29,   -29,    18,    46,   -29,   -29,     6,   -29,   -29,
     -29,   -29,    51,   -29,   -29,   -29,    51,    56,    61,    23,
     -29,   -29,    49,   -29,    73,   -14,    65,   -29,    51,    37,
      80,    23,    82,    70,   -29,    59,   -29,   -29,   -29,    23,
      24,    58,   -29,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    74,    23,    -3,    43,
     -29,   -29,    16,   -29,   -19,    96,   -29,   -29,   -29,    45,
      45,    45,    45,   -14,   -14,    45,    45,    65,    65,   -29,
      81,    26,   -29,   -29,    75,   -29,   -29,    51,   -29,    23,
      79,    44,   -29,    42,   -29,   -29,   -29,    83,    89,    51,
     -29,   -29,   -29
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     1,     4,     0,     3,     0,     0,
       0,    29,    26,     0,    15,     5,     6,     0,     7,     8,
       9,    10,     0,    11,    12,    13,     0,     0,     0,     0,
      55,    54,     0,    37,     0,    48,    51,    53,     0,     0,
       0,     0,     0,     0,     4,    20,    31,    18,    19,     0,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,    16,     0,    21,     0,     0,    56,    38,    39,    43,
      42,    44,    45,    46,    47,    40,    41,    49,    50,    52,
       0,     0,    34,    33,     0,    17,    25,     0,    23,     0,
       0,     0,    22,     0,    27,    36,    35,     0,     0,     0,
      32,    30,    28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -29,   -29,   -29,    67,   -29,   -29,   -29,   -29,   -29,   -29,
     -29,   -29,   -29,   -26,   -29,   -29,   -29,   -29,   -29,   -29,
     -29,   -29,   -29,   -27,    39,   -28,    38,    36,    47
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     5,     6,    15,    16,    17,    18,    19,    20,
      21,    97,    22,    45,    23,    39,   109,    24,    38,    25,
      26,    94,   107,    32,    33,    34,    35,    36,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      46,    51,    50,    59,    60,     1,    29,     3,    76,    42,
      63,    64,    66,    69,    30,    31,    27,     7,     8,     9,
      10,    74,    96,     8,     9,    10,    43,    79,    80,    81,
      82,    92,    93,    85,    86,     4,    53,    54,    53,    54,
      91,    11,    12,    13,    28,    14,    11,    12,    13,    49,
      14,    75,    40,   100,    53,    54,    44,    30,    31,    52,
      41,    53,    54,    67,    95,    59,    60,    59,    60,   108,
      73,   102,   103,    55,    56,    57,    58,    47,   105,   106,
      59,    60,    48,   112,    65,    76,    61,    62,    55,    56,
      57,    58,    77,    78,    68,    59,    60,    83,    84,    87,
      88,    61,    62,    70,    71,    90,    98,    99,   101,   104,
     111,    72,    89,   110
};

static const yytype_int8 yycheck[] =
{
      26,    29,    29,    22,    23,     4,    26,     5,    27,     3,
      24,    25,    38,    41,    34,    35,    34,     6,     7,     8,
       9,    49,     6,     7,     8,     9,    20,    55,    56,    57,
      58,    34,    35,    61,    62,     0,    12,    13,    12,    13,
      67,    30,    31,    32,    34,    34,    30,    31,    32,    26,
      34,    27,    34,    27,    12,    13,     5,    34,    35,    10,
      14,    12,    13,    26,    21,    22,    23,    22,    23,    27,
      11,    97,    99,    15,    16,    17,    18,    21,    34,    35,
      22,    23,    21,   109,    19,    27,    28,    29,    15,    16,
      17,    18,    53,    54,    14,    22,    23,    59,    60,    63,
      64,    28,    29,    21,    34,    31,    10,    26,    33,    30,
      21,    44,    65,    30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    38,     5,     0,    39,    40,     6,     7,     8,
       9,    30,    31,    32,    34,    41,    42,    43,    44,    45,
      46,    47,    49,    51,    54,    56,    57,    34,    34,    26,
      34,    35,    60,    61,    62,    63,    64,    65,    55,    52,
      34,    14,     3,    20,     5,    50,    50,    21,    21,    26,
      60,    62,    10,    12,    13,    15,    16,    17,    18,    22,
      23,    28,    29,    24,    25,    19,    50,    26,    14,    62,
      21,    34,    40,    11,    62,    27,    27,    61,    61,    62,
      62,    62,    62,    63,    63,    62,    62,    64,    64,    65,
      31,    60,    34,    35,    58,    21,     6,    48,    10,    26,
      27,    33,    50,    60,    30,    34,    35,    59,    27,    53,
      30,    21,    50
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    39,    38,    40,    40,    41,    41,    41,    41,
      41,    41,    41,    41,    42,    43,    43,    44,    45,    46,
      47,    48,    47,    49,    49,    50,    52,    53,    51,    55,
      54,    56,    57,    58,    58,    59,    59,    60,    60,    60,
      61,    61,    61,    61,    61,    61,    62,    62,    62,    63,
      63,    63,    64,    64,    65,    65,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     5,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     3,     4,     3,     3,
       2,     0,     5,     5,     3,     3,     0,     0,     8,     0,
       8,     2,     7,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     1,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
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
  case 2: /* $@1: %empty  */
#line 70 "proyectoTDC.y"
        {
            /* 1. Cabecera NASM inicial */
            fprintf(asm_out, "bits 32\n\n");
            fprintf(asm_out, "global main\n");
            fprintf(asm_out, "extern printf\n");
            fprintf(asm_out, "extern scanf\n\n");
            fprintf(asm_out, "section .data\n");
            fprintf(asm_out, "    fmt_print db \"%%d\", 10, 0\n");
            fprintf(asm_out, "    fmt_scan  db \"%%d\", 0\n\n");
            fprintf(asm_out, "section .bss\n");
        }
#line 1253 "proyectoTDC.tab.c"
    break;

  case 3: /* inicio: MAIN INICIO_BLOQUE $@1 listaSentencias END  */
#line 82 "proyectoTDC.y"
        {
            /* 2. Cierre de ejecución y rutinas de I/O en NASM */
            fprintf(asm_out, "\nsection .text\n");
            fprintf(asm_out, "main:\n");
            fprintf(asm_out, "    push ebp\n");
            fprintf(asm_out, "    mov ebp, esp\n\n");
            
            /* Nota: En un modelo de pasada única, si prefieres que las instrucciones 
               se acumulen en un archivo temporal de texto para el .text, puedes hacerlo. 
               Aquí cerramos el bloque principal del programa estándar: */
            
            fprintf(asm_out, "\n    mov esp, ebp\n");
            fprintf(asm_out, "    pop ebp\n");
            fprintf(asm_out, "    xor eax, eax\n");
            fprintf(asm_out, "    ret\n\n");

            /* Rutinas auxiliares para input / output compatible con C */
            fprintf(asm_out, "global output\n");
            fprintf(asm_out, "output:\n");
            fprintf(asm_out, "    push ebp\n");
            fprintf(asm_out, "    mov ebp, esp\n");
            fprintf(asm_out, "    push dword [ebp+8]\n");
            fprintf(asm_out, "    push fmt_print\n");
            fprintf(asm_out, "    call printf\n");
            fprintf(asm_out, "    add esp, 8\n");
            fprintf(asm_out, "    mov esp, ebp\n");
            fprintf(asm_out, "    pop ebp\n");
            fprintf(asm_out, "    ret\n\n");

            fprintf(asm_out, "global input\n");
            fprintf(asm_out, "input:\n");
            fprintf(asm_out, "    push ebp\n");
            fprintf(asm_out, "    mov ebp, esp\n");
            fprintf(asm_out, "    push dword [ebp+8]\n");
            fprintf(asm_out, "    push fmt_scan\n");
            fprintf(asm_out, "    call scanf\n");
            fprintf(asm_out, "    add esp, 8\n");
            fprintf(asm_out, "    mov esp, ebp\n");
            fprintf(asm_out, "    pop ebp\n");
            fprintf(asm_out, "    ret\n");

            fprintf(stderr, "\nTDC y NASM generados correctamente en paralelo.\n");
        }
#line 1301 "proyectoTDC.tab.c"
    break;

  case 15: /* listaDeNombres: ID  */
#line 149 "proyectoTDC.y"
        { 
            /* Registramos cada variable en la sección .bss de NASM */
            fprintf(asm_out, "    %s resd 1\n", (yyvsp[0].cadena)); 
        }
#line 1310 "proyectoTDC.tab.c"
    break;

  case 16: /* listaDeNombres: listaDeNombres DOS_PUNTOS ID  */
#line 154 "proyectoTDC.y"
        { 
            fprintf(asm_out, "    %s resd 1\n", (yyvsp[0].cadena)); 
        }
#line 1318 "proyectoTDC.tab.c"
    break;

  case 17: /* asignacion: ID ASIGNACION expresion PUNTO_COMA  */
#line 162 "proyectoTDC.y"
        { 
            /* TAC */
            fprintf(tac_out, "%s=%s\n", (yyvsp[-3].cadena), (yyvsp[-1].cadena)); 
            
            /* NASM Paralelo */
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-1].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyvsp[-3].cadena));
        }
#line 1331 "proyectoTDC.tab.c"
    break;

  case 18: /* lectura: INPUT ID PUNTO_COMA  */
#line 175 "proyectoTDC.y"
        {
            /* TAC */
            fprintf(tac_out, "push %s\n", (yyvsp[-1].cadena));
            fprintf(tac_out, "call input\n");
            fprintf(tac_out, "pop %s\n", (yyvsp[-1].cadena));

            /* NASM Paralelo */
            fprintf(asm_out, "    push %s\n", (yyvsp[-1].cadena));
            fprintf(asm_out, "    call input\n");
            fprintf(asm_out, "    add esp, 4\n");
        }
#line 1347 "proyectoTDC.tab.c"
    break;

  case 19: /* escritura: OUTPUT ID PUNTO_COMA  */
#line 190 "proyectoTDC.y"
        {
            /* TAC */
            fprintf(tac_out, "push %s\n", (yyvsp[-1].cadena));
            fprintf(tac_out, "call output\n");

            /* NASM Paralelo */
            fprintf(asm_out, "    push dword [%s]\n", (yyvsp[-1].cadena));
            fprintf(asm_out, "    call output\n");
            fprintf(asm_out, "    add esp, 4\n");
        }
#line 1362 "proyectoTDC.tab.c"
    break;

  case 20: /* condicional: cabeceraIf bloque  */
#line 205 "proyectoTDC.y"
        {
            char etiqueta[50];
            desapilarEtiqueta(etiqueta);
            
            fprintf(tac_out, "%s:\n", etiqueta);
            fprintf(asm_out, "%s:\n", etiqueta);
        }
#line 1374 "proyectoTDC.tab.c"
    break;

  case 21: /* $@2: %empty  */
#line 213 "proyectoTDC.y"
        {
            char etiquetaSalida[50];
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            
            fprintf(tac_out, "goto %s\n", etiquetaFin);
            fprintf(asm_out, "    jmp %s\n", etiquetaFin);
            
            desapilarEtiqueta(etiquetaSalida);
            
            fprintf(tac_out, "%s:\n", etiquetaSalida);
            fprintf(asm_out, "%s:\n", etiquetaSalida);
            
            apilarEtiqueta(etiquetaFin);
        }
#line 1394 "proyectoTDC.tab.c"
    break;

  case 22: /* condicional: cabeceraIf bloque ELSE $@2 bloque  */
#line 229 "proyectoTDC.y"
        {
            char etiqueta[50];
            desapilarEtiqueta(etiqueta);
            
            fprintf(tac_out, "%s:\n", etiqueta);
            fprintf(asm_out, "%s:\n", etiqueta);
        }
#line 1406 "proyectoTDC.tab.c"
    break;

  case 23: /* cabeceraIf: IF PAR_IZQ condicionLogica PAR_DER THEN  */
#line 240 "proyectoTDC.y"
        {
            char etiqueta[50];
            nuevaEtiqueta(etiqueta);
            apilarEtiqueta(etiqueta);
            
            fprintf(tac_out, "ifZ %s goto %s\n", (yyvsp[-2].cadena), etiqueta);
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, 0\n");
            fprintf(asm_out, "    je %s\n", etiqueta);
        }
#line 1421 "proyectoTDC.tab.c"
    break;

  case 24: /* cabeceraIf: IF condicionLogica THEN  */
#line 251 "proyectoTDC.y"
        {
            char etiqueta[50];
            nuevaEtiqueta(etiqueta);
            apilarEtiqueta(etiqueta);
            
            fprintf(tac_out, "ifZ %s goto %s\n", (yyvsp[-1].cadena), etiqueta);
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-1].cadena));
            fprintf(asm_out, "    cmp eax, 0\n");
            fprintf(asm_out, "    je %s\n", etiqueta);
        }
#line 1436 "proyectoTDC.tab.c"
    break;

  case 26: /* $@3: %empty  */
#line 270 "proyectoTDC.y"
        {
            char etiquetaInicio[50];
            nuevaEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "%s:\n", etiquetaInicio);
            fprintf(asm_out, "%s:\n", etiquetaInicio);
            
            apilarEtiqueta(etiquetaInicio);
        }
#line 1450 "proyectoTDC.tab.c"
    break;

  case 27: /* $@4: %empty  */
#line 280 "proyectoTDC.y"
        {
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            
            fprintf(tac_out, "ifZ %s goto %s\n", (yyvsp[-2].cadena), etiquetaFin);
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, 0\n");
            fprintf(asm_out, "    je %s\n", etiquetaFin);
            
            apilarEtiqueta(etiquetaFin);
        }
#line 1466 "proyectoTDC.tab.c"
    break;

  case 28: /* cicloWhile: WHILE $@3 PAR_IZQ condicionLogica PAR_DER DO $@4 bloque  */
#line 292 "proyectoTDC.y"
        {
            char etiquetaFin[50];
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaFin);
            desapilarEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "goto %s\n", etiquetaInicio);
            fprintf(tac_out, "%s:\n", etiquetaFin);
            
            fprintf(asm_out, "    jmp %s\n", etiquetaInicio);
            fprintf(asm_out, "%s:\n", etiquetaFin);
        }
#line 1483 "proyectoTDC.tab.c"
    break;

  case 29: /* $@5: %empty  */
#line 309 "proyectoTDC.y"
        {
            char etiquetaInicio[50];
            nuevaEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "%s:\n", etiquetaInicio);
            fprintf(asm_out, "%s:\n", etiquetaInicio);
            
            apilarEtiqueta(etiquetaInicio);
        }
#line 1497 "proyectoTDC.tab.c"
    break;

  case 30: /* cicloDoWhile: DO $@5 bloque WHILE PAR_IZQ condicionLogica PAR_DER PUNTO_COMA  */
#line 319 "proyectoTDC.y"
        {
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "ifZ %s goto %s\n", (yyvsp[-2].cadena), etiquetaInicio);
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, 0\n");
            fprintf(asm_out, "    je %s\n", etiquetaInicio);
        }
#line 1511 "proyectoTDC.tab.c"
    break;

  case 31: /* cicloFor: cabeceraFor bloque  */
#line 333 "proyectoTDC.y"
        {
            char etiquetaFin[50];
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaFin);
            desapilarEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "%s=%s+1\n", (yyvsp[-1].cadena), (yyvsp[-1].cadena));
            fprintf(tac_out, "goto %s\n", etiquetaInicio);
            fprintf(tac_out, "%s:\n", etiquetaFin);

            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-1].cadena));
            fprintf(asm_out, "    add eax, 1\n");
            fprintf(asm_out, "    mov [%s], eax\n", (yyvsp[-1].cadena));
            fprintf(asm_out, "    jmp %s\n", etiquetaInicio);
            fprintf(asm_out, "%s:\n", etiquetaFin);
        }
#line 1532 "proyectoTDC.tab.c"
    break;

  case 32: /* cabeceraFor: FOR ID ASIGNACION limiteInferior TO limiteSuperior DO  */
#line 353 "proyectoTDC.y"
        {
            char etiquetaInicio[50];
            char etiquetaFin[50];
            char temporal[50];

            strcpy((yyval.cadena), (yyvsp[-5].cadena));                    
            nuevaTemp(temporal);
            
            /* Registramos el temporal del For en la sección .bss de NASM */
            fprintf(asm_out, "    %s resd 1\n", temporal);

            nuevaEtiqueta(etiquetaInicio);
            nuevaEtiqueta(etiquetaFin);

            fprintf(tac_out, "%s=%s\n", (yyvsp[-5].cadena), (yyvsp[-3].cadena));         
            fprintf(tac_out, "%s=%s\n", temporal, (yyvsp[-1].cadena));   
            fprintf(tac_out, "%s:\n", etiquetaInicio);
            fprintf(tac_out, "ifZ %s<=%s goto %s\n", (yyvsp[-5].cadena), temporal, etiquetaFin);

            /* Traducción NASM equivalente para la cabecera del FOR */
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-3].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyvsp[-5].cadena));
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-1].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", temporal);
            fprintf(asm_out, "%s:\n", etiquetaInicio);
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-5].cadena));
            fprintf(asm_out, "    cmp eax, [%s]\n", temporal);
            fprintf(asm_out, "    jle %%_for_ok_%s\n", etiquetaInicio); /* si <= salta a continuar */
            fprintf(asm_out, "    jmp %s\n", etiquetaFin);             /* sino, sale del for */
            fprintf(asm_out, "%%_for_ok_%s:\n", etiquetaInicio);

            apilarEtiqueta(etiquetaInicio);
            apilarEtiqueta(etiquetaFin);
        }
#line 1571 "proyectoTDC.tab.c"
    break;

  case 33: /* limiteInferior: NUMERO  */
#line 391 "proyectoTDC.y"
        { 
            strcpy((yyval.cadena), (yyvsp[0].cadena)); 
            /* Si es un número constante, podemos crear una variable auxiliar temporal o manejarlo directo */
        }
#line 1580 "proyectoTDC.tab.c"
    break;

  case 34: /* limiteInferior: ID  */
#line 395 "proyectoTDC.y"
               { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1586 "proyectoTDC.tab.c"
    break;

  case 35: /* limiteSuperior: NUMERO  */
#line 399 "proyectoTDC.y"
               { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1592 "proyectoTDC.tab.c"
    break;

  case 36: /* limiteSuperior: ID  */
#line 400 "proyectoTDC.y"
               { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1598 "proyectoTDC.tab.c"
    break;

  case 38: /* condicionLogica: condicionLogica AND comparacionSimple  */
#line 407 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            /* Registramos el temporal booleano en .bss */
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s and %s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    and eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyval.cadena));
        }
#line 1614 "proyectoTDC.tab.c"
    break;

  case 39: /* condicionLogica: condicionLogica OR comparacionSimple  */
#line 419 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s or %s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    or eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyval.cadena));
        }
#line 1629 "proyectoTDC.tab.c"
    break;

  case 40: /* comparacionSimple: expresion MENOR expresion  */
#line 433 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s<%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    jl %%_lt_%s\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 0\n", (yyval.cadena));
            fprintf(asm_out, "    jmp %%_end_lt_%s\n", (yyval.cadena));
            fprintf(asm_out, "%%_lt_%s:\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 1\n", (yyval.cadena));
            fprintf(asm_out, "%%_end_lt_%s:\n", (yyval.cadena));
        }
#line 1649 "proyectoTDC.tab.c"
    break;

  case 41: /* comparacionSimple: expresion MAYOR expresion  */
#line 449 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s>%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    jg %%_gt_%s\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 0\n", (yyval.cadena));
            fprintf(asm_out, "    jmp %%_end_gt_%s\n", (yyval.cadena));
            fprintf(asm_out, "%%_gt_%s:\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 1\n", (yyval.cadena));
            fprintf(asm_out, "%%_end_gt_%s:\n", (yyval.cadena));
        }
#line 1669 "proyectoTDC.tab.c"
    break;

  case 42: /* comparacionSimple: expresion MENOR_IGUAL expresion  */
#line 465 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s<=%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    jle %%_le_%s\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 0\n", (yyval.cadena));
            fprintf(asm_out, "    jmp %%_end_le_%s\n", (yyval.cadena));
            fprintf(asm_out, "%%_le_%s:\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 1\n", (yyval.cadena));
            fprintf(asm_out, "%%_end_le_%s:\n", (yyval.cadena));
        }
#line 1689 "proyectoTDC.tab.c"
    break;

  case 43: /* comparacionSimple: expresion MAYOR_IGUAL expresion  */
#line 481 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s>=%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    jge %%_ge_%s\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 0\n", (yyval.cadena));
            fprintf(asm_out, "    jmp %%_end_ge_%s\n", (yyval.cadena));
            fprintf(asm_out, "%%_ge_%s:\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 1\n", (yyval.cadena));
            fprintf(asm_out, "%%_end_ge_%s:\n", (yyval.cadena));
        }
#line 1709 "proyectoTDC.tab.c"
    break;

  case 44: /* comparacionSimple: expresion COMPARACION_IGUAL expresion  */
#line 497 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s==%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    je %%_eq_%s\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 0\n", (yyval.cadena));
            fprintf(asm_out, "    jmp %%_end_eq_%s\n", (yyval.cadena));
            fprintf(asm_out, "%%_eq_%s:\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 1\n", (yyval.cadena));
            fprintf(asm_out, "%%_end_eq_%s:\n", (yyval.cadena));
        }
#line 1729 "proyectoTDC.tab.c"
    break;

  case 45: /* comparacionSimple: expresion DIFERENTE expresion  */
#line 513 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s<>%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cmp eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    jne %%_ne_%s\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 0\n", (yyval.cadena));
            fprintf(asm_out, "    jmp %%_end_ne_%s\n", (yyval.cadena));
            fprintf(asm_out, "%%_ne_%s:\n", (yyval.cadena));
            fprintf(asm_out, "    mov dword [%s], 1\n", (yyval.cadena));
            fprintf(asm_out, "%%_end_ne_%s:\n", (yyval.cadena));
        }
#line 1749 "proyectoTDC.tab.c"
    break;

  case 46: /* expresion: expresion MAS termino  */
#line 533 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s+%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    add eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyval.cadena));
        }
#line 1764 "proyectoTDC.tab.c"
    break;

  case 47: /* expresion: expresion MENOS termino  */
#line 544 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s-%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    sub eax, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyval.cadena));
        }
#line 1779 "proyectoTDC.tab.c"
    break;

  case 48: /* expresion: termino  */
#line 555 "proyectoTDC.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1785 "proyectoTDC.tab.c"
    break;

  case 49: /* termino: termino POR potenciaNum  */
#line 560 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s*%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    imul eax, dword [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyval.cadena));
        }
#line 1800 "proyectoTDC.tab.c"
    break;

  case 50: /* termino: termino ENTRE potenciaNum  */
#line 571 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s/%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    cdq\n");
            fprintf(asm_out, "    idiv dword [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyval.cadena));
        }
#line 1816 "proyectoTDC.tab.c"
    break;

  case 51: /* termino: potenciaNum  */
#line 583 "proyectoTDC.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1822 "proyectoTDC.tab.c"
    break;

  case 52: /* potenciaNum: potenciaNum POTENCIA factorBasico  */
#line 588 "proyectoTDC.y"
        { 
            nuevaTemp((yyval.cadena)); 
            fprintf(asm_out, "    %s resd 1\n", (yyval.cadena));
            
            fprintf(tac_out, "%s=%s**%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); 
            
            /* Simulación de potencia por bucle en NASM */
            fprintf(asm_out, "    mov eax, 1\n");
            fprintf(asm_out, "    mov ecx, [%s]\n", (yyvsp[0].cadena));
            fprintf(asm_out, "%%_pow_loop_%s:\n", (yyval.cadena));
            fprintf(asm_out, "    cmp ecx, 0\n");
            fprintf(asm_out, "    jle %%_pow_fin_%s\n", (yyval.cadena));
            fprintf(asm_out, "    imul eax, dword [%s]\n", (yyvsp[-2].cadena));
            fprintf(asm_out, "    dec ecx\n");
            fprintf(asm_out, "    jmp %%_pow_loop_%s\n", (yyval.cadena));
            fprintf(asm_out, "%%_pow_fin_%s:\n", (yyval.cadena));
            fprintf(asm_out, "    mov [%s], eax\n", (yyval.cadena));
        }
#line 1845 "proyectoTDC.tab.c"
    break;

  case 53: /* potenciaNum: factorBasico  */
#line 607 "proyectoTDC.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1851 "proyectoTDC.tab.c"
    break;

  case 54: /* factorBasico: NUMERO  */
#line 612 "proyectoTDC.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1857 "proyectoTDC.tab.c"
    break;

  case 55: /* factorBasico: ID  */
#line 614 "proyectoTDC.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1863 "proyectoTDC.tab.c"
    break;

  case 56: /* factorBasico: PAR_IZQ expresion PAR_DER  */
#line 616 "proyectoTDC.y"
        { strcpy((yyval.cadena), (yyvsp[-1].cadena)); }
#line 1869 "proyectoTDC.tab.c"
    break;


#line 1873 "proyectoTDC.tab.c"

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
  ++yynerrs;

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
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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

#line 619 "proyectoTDC.y"


int main(void)
{
    /* Abrimos ambos archivos en paralelo */
    tac_out = fopen("codigo.tac", "w");
    asm_out = fopen("codigo.asm", "w");

    if (!tac_out || !asm_out) {
        fprintf(stderr, "Error al crear los archivos de salida TAC o ASM.\n");
        return 1;
    }

    int resultado = yyparse();

    fclose(tac_out);
    fclose(asm_out);

    if (resultado == 0)
        fprintf(stderr, "\nAnalisis completado: Se generaron 'codigo.tac' y 'codigo.asm' en paralelo.\n");
    else
        fprintf(stderr, "\nEl programa contiene errores sintacticos.\n");

    return resultado;
}

void yyerror(const char *mensaje)
{
    fprintf(stderr,
        "Error sintactico en linea %d cerca de '%s': %s\n",
        yylineno, yytext, mensaje);
}
