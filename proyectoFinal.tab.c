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
#line 1 "proyectoFinal.y"

#include <stdio.h>
#include <string.h>

/* Función nativa en C optimizada: declara solo las variables y temporales usados */
void traducirTacANasm(const char *archivoTac, const char *archivoNasm) {
    FILE *in = fopen(archivoTac, "r");
    FILE *out = fopen(archivoNasm, "w");
    if (!in || !out) {
        fprintf(stderr, "Error al abrir archivos para la traducción NASM.\n");
        return;
    }

    char variables[100][50];
    int num_variables = 0;
    int usados_t[500] = {0};

    char linea[256];

    /* PASO 1: Escanear el TAC y recolectar CUALQUIER variable automáticamente (filtrando palabras clave) */
    while (fgets(linea, sizeof(linea), in)) {
        linea[strcspn(linea, "\r\n")] = 0;
        if (strlen(linea) == 0) continue;

        /* A. Detectar temporales tX */
        for (int i = 1; i < 500; i++) {
            char t_str[20];
            sprintf(t_str, "t%d", i);
            if (strstr(linea, t_str)) {
                usados_t[i] = 1;
            }
        }

        /* B. Extraer tokens/palabras de la línea para detectar variables reales */
        char copia[256];
        strcpy(copia, linea);
        char *token = strtok(copia, " =+-*/<>:\t\r\n()");
        while (token != NULL) {
            if ((token[0] >= 'a' && token[0] <= 'z') || (token[0] >= 'A' && token[0] <= 'Z')) {
                // Filtrar si es un temporal t1..t499
                int es_temp = 0;
                if (token[0] == 't') {
                    int es_num = 1;
                    for (int k = 1; k < strlen(token); k++) {
                        if (token[k] < '0' || token[k] > '9') { es_num = 0; break; }
                    }
                    if (strlen(token) > 1 && es_num) es_temp = 1;
                }

                // Filtrar palabras reservadas de control e I/O
                if (!es_temp) {
                    if (strcmp(token, "goto") != 0 && strcmp(token, "ifZ") != 0 && 
                        strcmp(token, "call") != 0 && strcmp(token, "push") != 0 &&
                        strcmp(token, "pop") != 0  && strcmp(token, "input") != 0 && 
                        strcmp(token, "output") != 0) {
                        
                        int existe = 0;
                        for (int v = 0; v < num_variables; v++) {
                            if (strcmp(variables[v], token) == 0) {
                                existe = 1;
                                break;
                            }
                        }
                        if (!existe && num_variables < 100) {
                            strcpy(variables[num_variables++], token);
                        }
                    }
                }
            }
            token = strtok(NULL, " =+-*/<>:\t\r\n()");
        }
    }
    rewind(in);

    /* Cabecera NASM */
    fprintf(out, "bits 32\n\n");
    fprintf(out, "global main\n");
    fprintf(out, "extern printf\n");
    fprintf(out, "extern scanf\n\n");
    fprintf(out, "section .data\n");
    fprintf(out, "    fmt_print db \"%%d\", 10, 0\n");
    fprintf(out, "    fmt_scan  db \"%%d\", 0\n\n");
    
    fprintf(out, "section .bss\n");
    
    /* Declarar únicamente las variables limpias detectadas */
    for (int v = 0; v < num_variables; v++) {
        fprintf(out, "    %s resd 1\n", variables[v]);
    }

    /* Declarar los temporales usados */
    for (int i = 1; i < 500; i++) {
        if (usados_t[i]) {
            fprintf(out, "    t%d resd 1\n", i);
        }
    }

    fprintf(out, "\nsection .text\n");
    fprintf(out, "main:\n");
    fprintf(out, "    push ebp\n");
    fprintf(out, "    mov ebp, esp\n\n");

    /* PASO 2: Traducir instrucción por instrucción */
    while (fgets(linea, sizeof(linea), in)) {
        linea[strcspn(linea, "\r\n")] = 0;
        if (strlen(linea) == 0) continue;

        /* A. Etiquetas */
        if (linea[strlen(linea) - 1] == ':') {
            fprintf(out, "%s\n", linea);
        }
        /* B. Saltos incondicionales */
        else if (strncmp(linea, "goto ", 5) == 0) {
            fprintf(out, "    jmp %s\n", linea + 5);
        }
        /* C. Saltos condicionales */
        else if (strncmp(linea, "ifZ ", 4) == 0) {
            char cond[50], etiqueta[50];
            if (sscanf(linea, "ifZ %s goto %s", cond, etiqueta) == 2) {
                fprintf(out, "    mov eax, [%s]\n", cond);
                fprintf(out, "    cmp eax, 0\n");
                fprintf(out, "    je %s\n", etiqueta);
            }
        }
        /* D. Entrada / Salida */
        else if (strncmp(linea, "push ", 5) == 0) {
            char var[50];
            sscanf(linea, "push %s", var);
            if ((var[0] >= 'a' && var[0] <= 'z') || (var[0] >= 'A' && var[0] <= 'Z') || var[0] == 't') {
                fprintf(out, "    push dword [%s]\n", var);
            } else {
                fprintf(out, "    push %s\n", var);
            }
        }
        else if (strncmp(linea, "push_dir ", 9) == 0) {
            char var[50];
            sscanf(linea, "push_dir %s", var);
            fprintf(out, "    push %s\n", var);
        }
        else if (strncmp(linea, "call ", 5) == 0) {
            fprintf(out, "    %s\n", linea);
            fprintf(out, "    add esp, 4\n");
        }
        else if (strncmp(linea, "pop ", 4) == 0) {
            continue; 
        }
        /* E. Asignaciones y operaciones */
        else if (strchr(linea, '=')) {
            char dest[50], expr[100];
            char *eq = strchr(linea, '=');
            *eq = '\0';
            strcpy(dest, linea);
            strcpy(expr, eq + 1);

            if (strstr(expr, "<=")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^<]<=%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                if (op2[0] >= '0' && op2[0] <= '9') fprintf(out, "    cmp eax, %s\n", op2);
                else fprintf(out, "    cmp eax, [%s]\n", op2);

                fprintf(out, "    jle ._%s_true\n", dest);
                fprintf(out, "    mov dword [%s], 0\n", dest);
                fprintf(out, "    jmp ._%s_fin\n", dest);
                fprintf(out, "._%s_true:\n", dest);
                fprintf(out, "    mov dword [%s], 1\n", dest);
                fprintf(out, "._%s_fin:\n", dest);
            } else if (strstr(expr, ">=")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^>]>=%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                if (op2[0] >= '0' && op2[0] <= '9') fprintf(out, "    cmp eax, %s\n", op2);
                else fprintf(out, "    cmp eax, [%s]\n", op2);

                fprintf(out, "    jge ._%s_true\n", dest);
                fprintf(out, "    mov dword [%s], 0\n", dest);
                fprintf(out, "    jmp ._%s_fin\n", dest);
                fprintf(out, "._%s_true:\n", dest);
                fprintf(out, "    mov dword [%s], 1\n", dest);
                fprintf(out, "._%s_fin:\n", dest);
            } else if (strstr(expr, "<") && !strstr(expr, "<=")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^<]<%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                if (op2[0] >= '0' && op2[0] <= '9') fprintf(out, "    cmp eax, %s\n", op2);
                else fprintf(out, "    cmp eax, [%s]\n", op2);

                fprintf(out, "    jl ._%s_true\n", dest);
                fprintf(out, "    mov dword [%s], 0\n", dest);
                fprintf(out, "    jmp ._%s_fin\n", dest);
                fprintf(out, "._%s_true:\n", dest);
                fprintf(out, "    mov dword [%s], 1\n", dest);
                fprintf(out, "._%s_fin:\n", dest);
            } else if (strstr(expr, ">") && !strstr(expr, ">=")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^>]>%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                if (op2[0] >= '0' && op2[0] <= '9') fprintf(out, "    cmp eax, %s\n", op2);
                else fprintf(out, "    cmp eax, [%s]\n", op2);

                fprintf(out, "    jg ._%s_true\n", dest);
                fprintf(out, "    mov dword [%s], 0\n", dest);
                fprintf(out, "    jmp ._%s_fin\n", dest);
                fprintf(out, "._%s_true:\n", dest);
                fprintf(out, "    mov dword [%s], 1\n", dest);
                fprintf(out, "._%s_fin:\n", dest);
            } else if (strstr(expr, "+")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^+]+%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                if (op2[0] >= '0' && op2[0] <= '9') fprintf(out, "    add eax, %s\n", op2);
                else fprintf(out, "    add eax, [%s]\n", op2);

                fprintf(out, "    mov [%s], eax\n", dest);
            } else if (strstr(expr, "-")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^-]-%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                if (op2[0] >= '0' && op2[0] <= '9') fprintf(out, "    sub eax, %s\n", op2);
                else fprintf(out, "    sub eax, [%s]\n", op2);

                fprintf(out, "    mov [%s], eax\n", dest);
            } else if (strstr(expr, "*") && !strstr(expr, "**")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^*]*%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                if (op2[0] >= '0' && op2[0] <= '9') fprintf(out, "    imul eax, dword %s\n", op2);
                else fprintf(out, "    imul eax, dword [%s]\n", op2);

                fprintf(out, "    mov [%s], eax\n", dest);
            } else if (strstr(expr, "/")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^/]/%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                fprintf(out, "    cdq\n");
                if (op2[0] >= '0' && op2[0] <= '9') {
                    fprintf(out, "    mov ecx, %s\n", op2);
                    fprintf(out, "    idiv ecx\n");
                } else {
                    fprintf(out, "    idiv dword [%s]\n", op2);
                }
                fprintf(out, "    mov [%s], eax\n", dest);
            } else if (strstr(expr, "**")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^*]**%s", op1, op2);
                if (op1[0] >= '0' && op1[0] <= '9') fprintf(out, "    mov eax, %s\n", op1);
                else fprintf(out, "    mov eax, [%s]\n", op1);

                if (op2[0] >= '0' && op2[0] <= '9') fprintf(out, "    imul eax, %s\n", op2);
                else fprintf(out, "    imul eax, dword [%s]\n", op2);

                fprintf(out, "    mov [%s], eax\n", dest);
            } else {
                if (expr[0] >= '0' && expr[0] <= '9') {
                    fprintf(out, "    mov dword [%s], %s\n", dest, expr);
                } else {
                    fprintf(out, "    mov eax, [%s]\n", expr);
                    fprintf(out, "    mov [%s], eax\n", dest);
                }
            }
        }
    }

    /* Cierre estándar NASM */
    fprintf(out, "\n    mov esp, ebp\n");
    fprintf(out, "    pop ebp\n");
    fprintf(out, "    xor eax, eax\n");
    fprintf(out, "    ret\n");

    /* Rutinas I/O */
    fprintf(out, "\nglobal output\noutput:\n");
    fprintf(out, "    push ebp\n    mov ebp, esp\n");
    fprintf(out, "    push dword [ebp+8]\n    push fmt_print\n    call printf\n    add esp, 8\n");
    fprintf(out, "    mov esp, ebp\n    pop ebp\n    ret\n");

    fprintf(out, "\nglobal input\ninput:\n");
    fprintf(out, "    push ebp\n    mov ebp, esp\n");
    fprintf(out, "    push dword [ebp+8]\n    push fmt_scan\n    call scanf\n    add esp, 8\n");
    fprintf(out, "    mov esp, ebp\n    pop ebp\n    ret\n");

    fclose(in);
    fclose(out);
}

void yyerror(const char *mensaje);
int  yylex(void);
extern int  yylineno;
extern char *yytext;

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

#line 409 "proyectoFinal.tab.c"

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

#include "proyectoFinal.tab.h"
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
  YYSYMBOL_listaSentencias = 39,           /* listaSentencias  */
  YYSYMBOL_sentencia = 40,                 /* sentencia  */
  YYSYMBOL_declaracionVariables = 41,      /* declaracionVariables  */
  YYSYMBOL_listaDeNombres = 42,            /* listaDeNombres  */
  YYSYMBOL_asignacion = 43,                /* asignacion  */
  YYSYMBOL_lectura = 44,                   /* lectura  */
  YYSYMBOL_escritura = 45,                 /* escritura  */
  YYSYMBOL_condicional = 46,               /* condicional  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_cabeceraIf = 48,                /* cabeceraIf  */
  YYSYMBOL_bloque = 49,                    /* bloque  */
  YYSYMBOL_cicloWhile = 50,                /* cicloWhile  */
  YYSYMBOL_51_2 = 51,                      /* $@2  */
  YYSYMBOL_52_3 = 52,                      /* $@3  */
  YYSYMBOL_cicloDoWhile = 53,              /* cicloDoWhile  */
  YYSYMBOL_54_4 = 54,                      /* $@4  */
  YYSYMBOL_cicloFor = 55,                  /* cicloFor  */
  YYSYMBOL_cabeceraFor = 56,               /* cabeceraFor  */
  YYSYMBOL_limiteInferior = 57,            /* limiteInferior  */
  YYSYMBOL_limiteSuperior = 58,            /* limiteSuperior  */
  YYSYMBOL_condicionLogica = 59,           /* condicionLogica  */
  YYSYMBOL_comparacionSimple = 60,         /* comparacionSimple  */
  YYSYMBOL_expresion = 61,                 /* expresion  */
  YYSYMBOL_termino = 62,                   /* termino  */
  YYSYMBOL_potenciaNum = 63,               /* potenciaNum  */
  YYSYMBOL_factorBasico = 64               /* factorBasico  */
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
#define YYLAST   114

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  55
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  112

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
       0,   361,   361,   368,   369,   373,   374,   375,   376,   377,
     378,   379,   380,   385,   389,   390,   395,   401,   409,   418,
     425,   424,   443,   450,   460,   466,   473,   465,   493,   492,
     514,   527,   552,   553,   557,   558,   563,   564,   566,   571,
     573,   575,   577,   579,   581,   587,   589,   591,   596,   598,
     600,   605,   607,   612,   614,   616
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
  "FOR", "TO", "ID", "NUMERO", "INVALIDO", "$accept", "inicio",
  "listaSentencias", "sentencia", "declaracionVariables", "listaDeNombres",
  "asignacion", "lectura", "escritura", "condicional", "$@1", "cabeceraIf",
  "bloque", "cicloWhile", "$@2", "$@3", "cicloDoWhile", "$@4", "cicloFor",
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

#define YYPACT_NINF (-28)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       1,     2,    35,   -28,   -28,    11,   -28,   -18,    10,   -20,
     -28,   -28,    18,    46,   -28,   -28,     6,   -28,   -28,   -28,
     -28,    51,   -28,   -28,   -28,    51,    56,    61,    23,   -28,
     -28,    49,   -28,    73,   -14,    65,   -28,    51,    68,    89,
      23,    83,    29,   -28,    59,   -28,   -28,   -28,    23,    24,
      58,   -28,    23,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    74,    23,    -3,    43,   -28,
     -28,    16,   -28,   -19,    96,   -28,   -28,   -28,    45,    45,
      45,    45,   -14,   -14,    45,    45,    65,    65,   -28,    81,
      26,   -28,   -28,    75,   -28,   -28,    51,   -28,    23,    79,
      44,   -28,    42,   -28,   -28,   -28,    80,    90,    51,   -28,
     -28,   -28
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     3,     1,     0,     2,     0,     0,     0,
      28,    25,     0,    14,     4,     5,     0,     6,     7,     8,
       9,     0,    10,    11,    12,     0,     0,     0,     0,    54,
      53,     0,    36,     0,    47,    50,    52,     0,     0,     0,
       0,     0,     0,     3,    19,    30,    17,    18,     0,     0,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    13,
      15,     0,    20,     0,     0,    55,    37,    38,    42,    41,
      43,    44,    45,    46,    39,    40,    48,    49,    51,     0,
       0,    33,    32,     0,    16,    24,     0,    22,     0,     0,
       0,    21,     0,    26,    35,    34,     0,     0,     0,    31,
      29,    27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -28,   -28,    69,   -28,   -28,   -28,   -28,   -28,   -28,   -28,
     -28,   -28,   -25,   -28,   -28,   -28,   -28,   -28,   -28,   -28,
     -28,   -28,   -26,    40,   -27,    39,    37,    50
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     5,    14,    15,    16,    17,    18,    19,    20,
      96,    21,    44,    22,    38,   108,    23,    37,    24,    25,
      93,   106,    31,    32,    33,    34,    35,    36
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      45,    50,    49,    58,    59,     1,    28,     3,    75,    41,
      62,    63,    65,    68,    29,    30,    26,     6,     7,     8,
       9,    73,    95,     7,     8,     9,    42,    78,    79,    80,
      81,    91,    92,    84,    85,     4,    52,    53,    52,    53,
      90,    10,    11,    12,    27,    13,    10,    11,    12,    48,
      13,    74,    39,    99,    52,    53,    43,    29,    30,    51,
      40,    52,    53,    70,    94,    58,    59,    58,    59,   107,
      72,   101,   102,    54,    55,    56,    57,    46,   104,   105,
      58,    59,    47,   111,    64,    75,    60,    61,    54,    55,
      56,    57,    76,    77,    66,    58,    59,    82,    83,    86,
      87,    60,    61,    67,    69,    89,    97,    98,   100,   103,
     109,   110,    71,     0,    88
};

static const yytype_int8 yycheck[] =
{
      25,    28,    28,    22,    23,     4,    26,     5,    27,     3,
      24,    25,    37,    40,    34,    35,    34,     6,     7,     8,
       9,    48,     6,     7,     8,     9,    20,    54,    55,    56,
      57,    34,    35,    60,    61,     0,    12,    13,    12,    13,
      66,    30,    31,    32,    34,    34,    30,    31,    32,    26,
      34,    27,    34,    27,    12,    13,     5,    34,    35,    10,
      14,    12,    13,    34,    21,    22,    23,    22,    23,    27,
      11,    96,    98,    15,    16,    17,    18,    21,    34,    35,
      22,    23,    21,   108,    19,    27,    28,    29,    15,    16,
      17,    18,    52,    53,    26,    22,    23,    58,    59,    62,
      63,    28,    29,    14,    21,    31,    10,    26,    33,    30,
      30,    21,    43,    -1,    64
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    38,     5,     0,    39,     6,     7,     8,     9,
      30,    31,    32,    34,    40,    41,    42,    43,    44,    45,
      46,    48,    50,    53,    55,    56,    34,    34,    26,    34,
      35,    59,    60,    61,    62,    63,    64,    54,    51,    34,
      14,     3,    20,     5,    49,    49,    21,    21,    26,    59,
      61,    10,    12,    13,    15,    16,    17,    18,    22,    23,
      28,    29,    24,    25,    19,    49,    26,    14,    61,    21,
      34,    39,    11,    61,    27,    27,    60,    60,    61,    61,
      61,    61,    62,    62,    61,    61,    63,    63,    64,    31,
      59,    34,    35,    57,    21,     6,    47,    10,    26,    27,
      33,    49,    59,    30,    34,    35,    58,    27,    52,    30,
      21,    49
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    39,    39,    40,    40,    40,    40,    40,
      40,    40,    40,    41,    42,    42,    43,    44,    45,    46,
      47,    46,    48,    48,    49,    51,    52,    50,    54,    53,
      55,    56,    57,    57,    58,    58,    59,    59,    59,    60,
      60,    60,    60,    60,    60,    61,    61,    61,    62,    62,
      62,    63,    63,    64,    64,    64
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     3,     4,     3,     3,     2,
       0,     5,     5,     3,     3,     0,     0,     8,     0,     8,
       2,     7,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     1,     1,     1,     3
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
  case 2: /* inicio: MAIN INICIO_BLOQUE listaSentencias END  */
#line 362 "proyectoFinal.y"
        {
            fprintf(stderr,
                "\nTDC generado correctamente\n");
        }
#line 1537 "proyectoFinal.tab.c"
    break;

  case 16: /* asignacion: ID ASIGNACION expresion PUNTO_COMA  */
#line 396 "proyectoFinal.y"
        { printf("%s=%s\n", (yyvsp[-3].cadena), (yyvsp[-1].cadena)); }
#line 1543 "proyectoFinal.tab.c"
    break;

  case 17: /* lectura: INPUT ID PUNTO_COMA  */
#line 402 "proyectoFinal.y"
        {
            printf("push_dir %s\n", (yyvsp[-1].cadena));
            printf("call input\n");
        }
#line 1552 "proyectoFinal.tab.c"
    break;

  case 18: /* escritura: OUTPUT ID PUNTO_COMA  */
#line 410 "proyectoFinal.y"
        {
            printf("push %s\n", (yyvsp[-1].cadena));
            printf("call output\n");
        }
#line 1561 "proyectoFinal.tab.c"
    break;

  case 19: /* condicional: cabeceraIf bloque  */
#line 419 "proyectoFinal.y"
        {
            char etiqueta[50];
            desapilarEtiqueta(etiqueta);
            printf("%s:\n", etiqueta);
        }
#line 1571 "proyectoFinal.tab.c"
    break;

  case 20: /* $@1: %empty  */
#line 425 "proyectoFinal.y"
        {
            char etiquetaSalida[50];
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            printf("goto %s\n", etiquetaFin);
            desapilarEtiqueta(etiquetaSalida);
            printf("%s:\n", etiquetaSalida);
            apilarEtiqueta(etiquetaFin);
        }
#line 1585 "proyectoFinal.tab.c"
    break;

  case 21: /* condicional: cabeceraIf bloque ELSE $@1 bloque  */
#line 435 "proyectoFinal.y"
        {
            char etiqueta[50];
            desapilarEtiqueta(etiqueta);
            printf("%s:\n", etiqueta);
        }
#line 1595 "proyectoFinal.tab.c"
    break;

  case 22: /* cabeceraIf: IF PAR_IZQ condicionLogica PAR_DER THEN  */
#line 444 "proyectoFinal.y"
        {
            char etiqueta[50];
            nuevaEtiqueta(etiqueta);
            apilarEtiqueta(etiqueta);
            printf("ifZ %s goto %s\n", (yyvsp[-2].cadena), etiqueta);
        }
#line 1606 "proyectoFinal.tab.c"
    break;

  case 23: /* cabeceraIf: IF condicionLogica THEN  */
#line 451 "proyectoFinal.y"
        {
            char etiqueta[50];
            nuevaEtiqueta(etiqueta);
            apilarEtiqueta(etiqueta);
            printf("ifZ %s goto %s\n", (yyvsp[-1].cadena), etiqueta);
        }
#line 1617 "proyectoFinal.tab.c"
    break;

  case 25: /* $@2: %empty  */
#line 466 "proyectoFinal.y"
        {
            char etiquetaInicio[50];
            nuevaEtiqueta(etiquetaInicio);
            printf("%s:\n", etiquetaInicio);
            apilarEtiqueta(etiquetaInicio);
        }
#line 1628 "proyectoFinal.tab.c"
    break;

  case 26: /* $@3: %empty  */
#line 473 "proyectoFinal.y"
        {
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            printf("ifZ %s goto %s\n", (yyvsp[-2].cadena), etiquetaFin);
            apilarEtiqueta(etiquetaFin);
        }
#line 1639 "proyectoFinal.tab.c"
    break;

  case 27: /* cicloWhile: WHILE $@2 PAR_IZQ condicionLogica PAR_DER DO $@3 bloque  */
#line 480 "proyectoFinal.y"
        {
            char etiquetaFin[50];
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaFin);
            desapilarEtiqueta(etiquetaInicio);
            printf("goto %s\n", etiquetaInicio);
            printf("%s:\n", etiquetaFin);
        }
#line 1652 "proyectoFinal.tab.c"
    break;

  case 28: /* $@4: %empty  */
#line 493 "proyectoFinal.y"
        {
            char etiquetaInicio[50];
            nuevaEtiqueta(etiquetaInicio);
            printf("%s:\n", etiquetaInicio);
            apilarEtiqueta(etiquetaInicio);
        }
#line 1663 "proyectoFinal.tab.c"
    break;

  case 29: /* cicloDoWhile: DO $@4 bloque WHILE PAR_IZQ condicionLogica PAR_DER PUNTO_COMA  */
#line 500 "proyectoFinal.y"
        {
            char etiquetaInicio[50];
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            desapilarEtiqueta(etiquetaInicio);

            printf("ifZ %s goto %s\n", (yyvsp[-2].cadena), etiquetaFin);
            printf("goto %s\n", etiquetaInicio);
            printf("%s:\n", etiquetaFin);
        }
#line 1678 "proyectoFinal.tab.c"
    break;

  case 30: /* cicloFor: cabeceraFor bloque  */
#line 515 "proyectoFinal.y"
        {
            char etiquetaFin[50];
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaFin);
            desapilarEtiqueta(etiquetaInicio);
            printf("%s=%s+1\n", (yyvsp[-1].cadena), (yyvsp[-1].cadena));
            printf("goto %s\n", etiquetaInicio);
            printf("%s:\n", etiquetaFin);
        }
#line 1692 "proyectoFinal.tab.c"
    break;

  case 31: /* cabeceraFor: FOR ID ASIGNACION limiteInferior TO limiteSuperior DO  */
#line 528 "proyectoFinal.y"
        {
            char etiquetaInicio[50];
            char etiquetaFin[50];
            char temporal[50];
            char tempCond[50];

            strcpy((yyval.cadena), (yyvsp[-5].cadena));
            nuevaTemp(temporal);
            nuevaEtiqueta(etiquetaInicio);
            nuevaEtiqueta(etiquetaFin);
            nuevaTemp(tempCond);

            printf("%s=%s\n", (yyvsp[-5].cadena), (yyvsp[-3].cadena));
            printf("%s=%s\n", temporal, (yyvsp[-1].cadena));
            printf("%s:\n", etiquetaInicio);
            printf("%s=%s<=%s\n", tempCond, (yyvsp[-5].cadena), temporal);
            printf("ifZ %s goto %s\n", tempCond, etiquetaFin);

            apilarEtiqueta(etiquetaInicio);
            apilarEtiqueta(etiquetaFin);
        }
#line 1718 "proyectoFinal.tab.c"
    break;

  case 32: /* limiteInferior: NUMERO  */
#line 552 "proyectoFinal.y"
               { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1724 "proyectoFinal.tab.c"
    break;

  case 33: /* limiteInferior: ID  */
#line 553 "proyectoFinal.y"
               { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1730 "proyectoFinal.tab.c"
    break;

  case 34: /* limiteSuperior: NUMERO  */
#line 557 "proyectoFinal.y"
               { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1736 "proyectoFinal.tab.c"
    break;

  case 35: /* limiteSuperior: ID  */
#line 558 "proyectoFinal.y"
               { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1742 "proyectoFinal.tab.c"
    break;

  case 37: /* condicionLogica: condicionLogica AND comparacionSimple  */
#line 565 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s and %s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1748 "proyectoFinal.tab.c"
    break;

  case 38: /* condicionLogica: condicionLogica OR comparacionSimple  */
#line 567 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s or %s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1754 "proyectoFinal.tab.c"
    break;

  case 39: /* comparacionSimple: expresion MENOR expresion  */
#line 572 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s<%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1760 "proyectoFinal.tab.c"
    break;

  case 40: /* comparacionSimple: expresion MAYOR expresion  */
#line 574 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s>%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1766 "proyectoFinal.tab.c"
    break;

  case 41: /* comparacionSimple: expresion MENOR_IGUAL expresion  */
#line 576 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s<=%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1772 "proyectoFinal.tab.c"
    break;

  case 42: /* comparacionSimple: expresion MAYOR_IGUAL expresion  */
#line 578 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s>=%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1778 "proyectoFinal.tab.c"
    break;

  case 43: /* comparacionSimple: expresion COMPARACION_IGUAL expresion  */
#line 580 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s==%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1784 "proyectoFinal.tab.c"
    break;

  case 44: /* comparacionSimple: expresion DIFERENTE expresion  */
#line 582 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s<>%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1790 "proyectoFinal.tab.c"
    break;

  case 45: /* expresion: expresion MAS termino  */
#line 588 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s+%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1796 "proyectoFinal.tab.c"
    break;

  case 46: /* expresion: expresion MENOS termino  */
#line 590 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s-%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1802 "proyectoFinal.tab.c"
    break;

  case 47: /* expresion: termino  */
#line 592 "proyectoFinal.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1808 "proyectoFinal.tab.c"
    break;

  case 48: /* termino: termino POR potenciaNum  */
#line 597 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s*%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1814 "proyectoFinal.tab.c"
    break;

  case 49: /* termino: termino ENTRE potenciaNum  */
#line 599 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s/%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1820 "proyectoFinal.tab.c"
    break;

  case 50: /* termino: potenciaNum  */
#line 601 "proyectoFinal.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1826 "proyectoFinal.tab.c"
    break;

  case 51: /* potenciaNum: potenciaNum POTENCIA factorBasico  */
#line 606 "proyectoFinal.y"
        { nuevaTemp((yyval.cadena)); printf("%s=%s**%s\n", (yyval.cadena), (yyvsp[-2].cadena), (yyvsp[0].cadena)); }
#line 1832 "proyectoFinal.tab.c"
    break;

  case 52: /* potenciaNum: factorBasico  */
#line 608 "proyectoFinal.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1838 "proyectoFinal.tab.c"
    break;

  case 53: /* factorBasico: NUMERO  */
#line 613 "proyectoFinal.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1844 "proyectoFinal.tab.c"
    break;

  case 54: /* factorBasico: ID  */
#line 615 "proyectoFinal.y"
        { strcpy((yyval.cadena), (yyvsp[0].cadena)); }
#line 1850 "proyectoFinal.tab.c"
    break;

  case 55: /* factorBasico: PAR_IZQ expresion PAR_DER  */
#line 617 "proyectoFinal.y"
        { strcpy((yyval.cadena), (yyvsp[-1].cadena)); }
#line 1856 "proyectoFinal.tab.c"
    break;


#line 1860 "proyectoFinal.tab.c"

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

#line 620 "proyectoFinal.y"


int main(void) {
    FILE *f_tac = freopen("codigo.tac", "w", stdout);
    if (!f_tac) {
        fprintf(stderr, "Error crítico: No se pudo crear el archivo codigo.tac\n");
        return 1;
    }

    int resultado = yyparse();
    fclose(f_tac);

    if (resultado == 0) {
        fprintf(stderr, "\n[OK] Programa sintacticamente correcto.\n");
        fprintf(stderr, "[OK] Codigo Intermedio (TAC) generado en 'codigo.tac'.\n");
        
        traducirTacANasm("codigo.tac", "codigo.asm");
        fprintf(stderr, "[OK] Ensamblador NASM generado en 'codigo.asm'.\n");
    } else {
        fprintf(stderr, "\nEl programa contiene errores sintacticos.\n");
    }

    return resultado;
}

void yyerror(const char *mensaje)
{
    fprintf(stderr,
        "Error sintactico en linea %d cerca de '%s': %s\n",
        yylineno, yytext, mensaje);
}
