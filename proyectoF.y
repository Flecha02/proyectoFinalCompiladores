%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);
%}

/* Declaración de todos los Tokens (incluyendo los tuyos y los de tus compañeros) */
%token ALLOCATE MAIN TOK_BEGIN END INPUT OUTPUT IF THEN ELSE AND OR
%token ASIGNACION MAYOR_IGUAL MENOR_IGUAL COMPARACION_IGUAL DIFERENTE POTENCIA
%token DOS_PUNTOS PUNTO_Y_COMA MAS MENOS POR ENTRE PAR_IZQ PAR_DER MENOR MAYOR
%token ID NUMERO

%%

/* Regla inicial genérica para que puedas probar tu parte de forma independiente */
inicio
    : declaracionVariables
    | expresion
    | condicionLogica
    ;

/* ==========================================
   1. DECLARACIÓN DE VARIABLES (Tu parte)
   Ejemplo: a:b:c ALLOCATE;
   ========================================== */
declaracionVariables
    : listaDeNombres ALLOCATE PUNTO_Y_COMA
    {
        printf("¡Reconocida con éxito: Declaración de variables!\n");
    }
    ;

listaDeNombres
    : listaDeNombres DOS_PUNTOS ID
    | ID
    ;


/* ==========================================
   2. COMPARACIONES Y LÓGICA AND / OR (Tu parte)
   Ejemplo: x < 25 AND y >= 0 OR z <> 10
   ========================================== */
condicionLogica
    : condicionLogica AND comparacionSimple
    {
        printf("Reconocida operación lógica: AND\n");
    }
    | condicionLogica OR comparacionSimple
    {
        printf("Reconocida operación lógica: OR\n");
    }
    | comparacionSimple
    ;

comparacionSimple
    : expresion MENOR expresion
    {
        printf("Comparación: Menor que (<)\n");
    }
    | expresion MAYOR expresion
    {
        printf("Comparación: Mayor que (>)\n");
    }
    | expresion MENOR_IGUAL expresion
    {
        printf("Comparación: Menor o igual (<=)\n");
    }
    | expresion MAYOR_IGUAL expresion
    {
        printf("Comparación: Mayor o igual (>=)\n");
    }
    | expresion COMPARACION_IGUAL expresion
    {
        printf("Comparación: Igual (==)\n");
    }
    | expresion DIFERENTE expresion
    {
        printf("Comparación: Diferente (<>)\n");
    }
    ;


/* ==========================================
   3. OPERACIONES MATEMÁTICAS (Tu parte)
   Jerarquía: Potencia -> Mult/Div -> Suma/Resta
   ========================================== */
expresion
    : expresion MAS termino
    | expresion MENOS termino
    | termino
    ;

termino
    : termino POR potenciaNum
    | termino ENTRE potenciaNum
    | potenciaNum
    ;

potenciaNum
    : potenciaNum POTENCIA factorBasico
    | factorBasico
    ;

factorBasico
    : NUMERO
    | ID
    | PAR_IZQ expresion PAR_DER
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error Sintáctico: %s\n", s);
}