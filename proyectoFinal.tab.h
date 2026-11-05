/* A Bison parser, made by GNU Bison 3.8.2.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PROYECTOFINAL_TAB_H_INCLUDED
# define YY_YY_PROYECTOFINAL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ALLOCATE = 258,                /* ALLOCATE  */
    MAIN = 259,                    /* MAIN  */
    INICIO_BLOQUE = 260,           /* INICIO_BLOQUE  */
    END = 261,                     /* END  */
    INPUT = 262,                   /* INPUT  */
    OUTPUT = 263,                  /* OUTPUT  */
    IF = 264,                      /* IF  */
    THEN = 265,                    /* THEN  */
    ELSE = 266,                    /* ELSE  */
    AND = 267,                     /* AND  */
    OR = 268,                      /* OR  */
    ASIGNACION = 269,              /* ASIGNACION  */
    MAYOR_IGUAL = 270,             /* MAYOR_IGUAL  */
    MENOR_IGUAL = 271,             /* MENOR_IGUAL  */
    COMPARACION_IGUAL = 272,       /* COMPARACION_IGUAL  */
    DIFERENTE = 273,               /* DIFERENTE  */
    POTENCIA = 274,                /* POTENCIA  */
    DOS_PUNTOS = 275,              /* DOS_PUNTOS  */
    PUNTO_COMA = 276,              /* PUNTO_COMA  */
    MAS = 277,                     /* MAS  */
    MENOS = 278,                   /* MENOS  */
    POR = 279,                     /* POR  */
    ENTRE = 280,                   /* ENTRE  */
    PAR_IZQ = 281,                 /* PAR_IZQ  */
    PAR_DER = 282,                 /* PAR_DER  */
    MENOR = 283,                   /* MENOR  */
    MAYOR = 284,                   /* MAYOR  */
    DO = 285,                      /* DO  */
    WHILE = 286,                   /* WHILE  */
    FOR = 287,                     /* FOR  */
    TO = 288,                      /* TO  */
    ID = 289,                      /* ID  */
    NUMERO = 290,                  /* NUMERO  */
    INVALIDO = 291                 /* INVALIDO  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 339 "proyectoFinal.y"

    char cadena[50];

#line 104 "proyectoFinal.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PROYECTOFINAL_TAB_H_INCLUDED  */
