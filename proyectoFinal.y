%{
#include <stdio.h>
void yyerror(char *mensaje);
int yylex();
extern int yylineno;
%}

%token ALLOCATE MAIN TK_BEGIN END INPUT OUTPUT IF THEN ELSE AND OR
%token ASIGNACION MAYOR_IGUAL MENOR_IGUAL COMPARACION_IGUAL DIFERENTE POTENCIA
%token DOS_PUNTOS PUNTO_COMA MAS MENOS POR ENTRE PAR_IZQ PAR_DER MENOR MAYOR
%token ID NUMERO INVALIDO

%%

/* [Javier] modificado: estructura principal del programa*/
inicio: MAIN TK_BEGIN cuerpo END
        ;

/* [Javier] agregado: regla de cuerpo para procesar secuencias de sentencias separadas por punto y coma */
cuerpo: sentencia PUNTO_COMA cuerpo
      | sentencia PUNTO_COMA
      | sentencia
      ;

/* [Javier] agregado: regla general de sentencias que agrupa declaraciones, asignaciones, E/S y expresiones */
sentencia: declaracionVariables 
         | asignacion 
         | lectura 
         | escritura 
         | condicionLogica 
         | expresion
         ;

/* [Javier] modificado: regla de declaración de variables con la palabra reservada ALLOCATE */
declaracionVariables: listaDeNombres ALLOCATE
        ;

listaDeNombres: listaDeNombres DOS_PUNTOS ID 
              | ID
              ;

/* [Javier] agregado: regla para manejar las asignaciones de variables con el operador := */
asignacion: ID ASIGNACION expresion
          ;

/* [Javier] agregado: Regla para la lectura de datos desde la entrada estándar (INPUT) */
lectura: INPUT ID
       ;

/* [Javier] agregado: regla para la escritura de datos hacia la salida estándar (OUTPUT) */
escritura: OUTPUT ID
         ;

/* --- estructura logica --- */
condicionLogica: condicionLogica AND comparacionSimple 
               | condicionLogica OR comparacionSimple 
               | comparacionSimple
               ;

comparacionSimple: expresion MENOR expresion 
                 | expresion MAYOR expresion
                 | expresion MENOR_IGUAL expresion 
                 | expresion MAYOR_IGUAL expresion
                 | expresion COMPARACION_IGUAL expresion 
                 | expresion DIFERENTE expresion
                 ;

/* --- estructura aritmetica --- */
expresion: expresion MAS termino 
         | expresion MENOS termino 
         | termino
         ;

termino: termino POR potenciaNum 
       | termino ENTRE potenciaNum 
       | potenciaNum
       ;

/* --- potenciacion  --- */
potenciaNum: potenciaNum POTENCIA factorBasico 
           | factorBasico
           ;

factorBasico: NUMERO 
            | ID 
            | PAR_IZQ expresion PAR_DER
            ;
        
%%

int main() {
    printf("Compilando EasyLang...\n");
    if (yyparse() == 0) {
        printf("\n[CORRECTO] Sintaxis valida.\n");
    } else {
        printf("\n[ERROR] Sintaxis invalida.\n");
    }
    return 0;
}

void yyerror(char *mensaje) {
    extern int yylineno;
    extern char *yytext;
    fprintf(stderr, "Error: %s en la linea %i cerca de %s\n", mensaje, yylineno, yytext);
}