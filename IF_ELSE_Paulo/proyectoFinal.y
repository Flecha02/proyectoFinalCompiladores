%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *mensaje);

extern int yylineno;
extern char *yytext;
%}

%token ALLOCATE MAIN INICIO_BLOQUE END INPUT OUTPUT IF THEN ELSE AND OR
%token ASIGNACION MAYOR_IGUAL MENOR_IGUAL COMPARACION_IGUAL DIFERENTE POTENCIA
%token DOS_PUNTOS PUNTO_COMA MAS MENOS POR ENTRE PAR_IZQ PAR_DER MENOR MAYOR
%token ID NUMERO INVALIDO

%start inicio

%%

/* Programa principal */
inicio:
    MAIN INICIO_BLOQUE listaSentencias END
    {
        printf("\nPrograma EasyLang sintacticamente correcto.\n");
    }
    ;

/* Lista de instrucciones */
listaSentencias:
      /* vacio */
    | listaSentencias sentencia
    ;

/* Instrucciones permitidas */
sentencia:
      declaracionVariables
    | asignacion
    | lectura
    | escritura
    | condicional
    ;

/* Declaracion de variables */
declaracionVariables:
    listaDeNombres ALLOCATE PUNTO_COMA
    ;

listaDeNombres:
      ID
    | listaDeNombres DOS_PUNTOS ID
    ;

/* Asignacion */
asignacion:
    ID ASIGNACION expresion PUNTO_COMA
    ;

/* Entrada */
lectura:
    INPUT ID PUNTO_COMA
    ;

/* Salida */
escritura:
    OUTPUT ID PUNTO_COMA
    ;

/* IF sencillo, IF-ELSE e IF anidados */
condicional:
      IF PAR_IZQ condicionLogica PAR_DER THEN bloque
    | IF PAR_IZQ condicionLogica PAR_DER THEN bloque ELSE bloque
    ;

/* Bloques de instrucciones */
bloque:
    INICIO_BLOQUE listaSentencias END
    ;

/* Condiciones logicas */
condicionLogica:
      comparacionSimple
    | condicionLogica AND comparacionSimple
    | condicionLogica OR comparacionSimple
    ;

/* Comparaciones */
comparacionSimple:
      expresion MENOR expresion
    | expresion MAYOR expresion
    | expresion MENOR_IGUAL expresion
    | expresion MAYOR_IGUAL expresion
    | expresion COMPARACION_IGUAL expresion
    | expresion DIFERENTE expresion
    ;

/* Operaciones matematicas */
expresion:
      expresion MAS termino
    | expresion MENOS termino
    | termino
    ;

termino:
      termino POR potenciaNum
    | termino ENTRE potenciaNum
    | potenciaNum
    ;

/* Potenciacion */
potenciaNum:
      potenciaNum POTENCIA factorBasico
    | factorBasico
    ;

/* Numeros, variables y parentesis */
factorBasico:
      NUMERO
    | ID
    | PAR_IZQ expresion PAR_DER
    ;

%%

int main(void)
{
    int resultado = yyparse();

    if (resultado != 0)
    {
        printf("\nEl programa contiene errores sintacticos.\n");
    }

    return resultado;
}

void yyerror(const char *mensaje)
{
    fprintf(stderr,
        "Error sintactico en linea %d cerca de '%s': %s\n",
        yylineno,
        yytext,
        mensaje
    );
}