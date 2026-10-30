%{
#include <stdio.h>
#include "codigo.h"

int yylex(void);
int yylex_destroy(void);
void yyerror(const char *mensaje);
extern int yylineno;
extern char *yytext;
static Nodo *programa = NULL;
%}

%code requires {
#include "codigo.h"
}
%union { Nodo *nodo; }
%token ALLOCATE MAIN INICIO_BLOQUE END INPUT OUTPUT IF THEN ELSE AND OR
%token ASIGNACION MAYOR_IGUAL MENOR_IGUAL COMPARACION_IGUAL DIFERENTE POTENCIA
%token DOS_PUNTOS PUNTO_COMA MAS MENOS POR ENTRE PAR_IZQ PAR_DER MENOR MAYOR
%token <nodo> ID NUMERO
%token INVALIDO
%type <nodo> listaSentencias sentencia declaracionVariables listaDeNombres
%type <nodo> asignacion lectura escritura condicional bloque
%type <nodo> condicionLogica condicionAnd comparacionSimple
%type <nodo> expresion termino potenciaNum factorBasico
%start inicio
%%
/* Se reutiliza la estructura de programa de IF-ELSE-Paulo. */
inicio: MAIN INICIO_BLOQUE listaSentencias END { programa = $3; };

listaSentencias:
      %empty { $$ = NULL; }
    | listaSentencias sentencia { $$ = crearNodo(SECUENCIA, NULL, $1, $2, NULL); }
    ;
sentencia:
      declaracionVariables | asignacion | lectura | escritura | condicional
    ;
declaracionVariables: listaDeNombres ALLOCATE PUNTO_COMA { $$ = $1; };
listaDeNombres:
      ID { $$ = crearNodo(DECLARACION, NULL, $1, NULL, NULL); }
    | listaDeNombres DOS_PUNTOS ID {
        Nodo *declaracion = crearNodo(DECLARACION, NULL, $3, NULL, NULL);
        $$ = crearNodo(SECUENCIA, NULL, $1, declaracion, NULL);
      }
    ;
asignacion: ID ASIGNACION expresion PUNTO_COMA
    { $$ = crearNodo(ASIGNAR, NULL, $1, $3, NULL); };
lectura: INPUT ID PUNTO_COMA { $$ = crearNodo(LEER, NULL, $2, NULL, NULL); };
escritura: OUTPUT ID PUNTO_COMA { $$ = crearNodo(ESCRIBIR, NULL, $2, NULL, NULL); };

/* Cada IF conserva su condicion y sus propios bloques. */
condicional:
      IF PAR_IZQ condicionLogica PAR_DER THEN bloque
        { $$ = crearNodo(CONDICIONAL, NULL, $3, $6, NULL); }
    | IF PAR_IZQ condicionLogica PAR_DER THEN bloque ELSE bloque
        { $$ = crearNodo(CONDICIONAL, NULL, $3, $6, $8); }
    ;
/* Un bloque vacio tiene un nodo para distinguir ELSE vacio de ausencia de ELSE. */
bloque: INICIO_BLOQUE listaSentencias END
    { $$ = crearNodo(SECUENCIA, NULL, $2, NULL, NULL); };

/* AND tiene mayor prioridad que OR. Se permite agrupar condiciones. */
condicionLogica:
      condicionAnd
    | condicionLogica OR condicionAnd { $$ = crearNodo(LOGICO_OR, NULL, $1, $3, NULL); }
    ;
condicionAnd:
      comparacionSimple
    | condicionAnd AND comparacionSimple { $$ = crearNodo(LOGICO_AND, NULL, $1, $3, NULL); }
    ;
comparacionSimple:
      expresion MENOR expresion { $$ = crearNodo(COMPARACION, "<", $1, $3, NULL); }
    | expresion MAYOR expresion { $$ = crearNodo(COMPARACION, ">", $1, $3, NULL); }
    | expresion MENOR_IGUAL expresion { $$ = crearNodo(COMPARACION, "<=", $1, $3, NULL); }
    | expresion MAYOR_IGUAL expresion { $$ = crearNodo(COMPARACION, ">=", $1, $3, NULL); }
    | expresion COMPARACION_IGUAL expresion { $$ = crearNodo(COMPARACION, "==", $1, $3, NULL); }
    | expresion DIFERENTE expresion { $$ = crearNodo(COMPARACION, "<>", $1, $3, NULL); }
    | PAR_IZQ condicionLogica PAR_DER { $$ = $2; }
    ;
/* Se conserva la jerarquia y la potencia asociativa a izquierda de Javier. */
expresion:
      expresion MAS termino { $$ = crearNodo(OPERACION, "+", $1, $3, NULL); }
    | expresion MENOS termino { $$ = crearNodo(OPERACION, "-", $1, $3, NULL); }
    | termino
    ;
termino:
      termino POR potenciaNum { $$ = crearNodo(OPERACION, "*", $1, $3, NULL); }
    | termino ENTRE potenciaNum { $$ = crearNodo(OPERACION, "/", $1, $3, NULL); }
    | potenciaNum
    ;
potenciaNum:
      potenciaNum POTENCIA factorBasico { $$ = crearNodo(OPERACION, "**", $1, $3, NULL); }
    | factorBasico
    ;
factorBasico: NUMERO | ID | PAR_IZQ expresion PAR_DER { $$ = $2; };
%%
int main(void)
{
    int resultado = yyparse();
    /* No se imprime codigo parcial si el archivo tiene errores. */
    if (resultado == 0)
        generarCodigo(programa);
    liberarNodos();
    yylex_destroy();
    return resultado;
}

void yyerror(const char *mensaje)
{
    fprintf(stderr, "Error sintactico en linea %d cerca de '%s': %s\n",
            yylineno, yytext, mensaje);
}
