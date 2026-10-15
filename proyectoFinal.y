%{
#include <stdio.h>
void yyerror(char *mensaje);
int yylex();
%}
%token ALLOCATE MAIN R_BEGIN END INPUT OUTPUT IF THEN ELSE AND OR DO WHILE FOR TO DO_KW
%token ASIGNACION MAYOR_IGUAL MENOR_IGUAL COMPARACION_IGUAL DIFERENTE POTENCIA
%token DOS_PUNTOS PUNTO_COMA MAS MENOS POR ENTRE PAR_IZQ PAR_DER MENOR MAYOR
%token ID NUMERO INVALIDO
%%
inicio: declaracionVariables | expresion | condicionLogica | cicloDoWhile | cicloFor
    
        ;

declaracionVariables: listaDeNombres ALLOCATE PUNTO_COMA
        ;

listaDeNombres: listaDeNombres DOS_PUNTOS ID | ID
        ;

condicionLogica: condicionLogica AND comparacionSimple | condicionLogica OR comparacionSimple | comparacionSimple
        ;

comparacionSimple: expresion MENOR expresion | expresion MAYOR expresion
        | expresion MENOR_IGUAL expresion | expresion MAYOR_IGUAL expresion
        | expresion COMPARACION_IGUAL expresion | expresion DIFERENTE expresion
        ;

expresion: expresion MAS termino | expresion MENOS termino | termino
        ;

termino: termino POR potenciaNum | termino ENTRE potenciaNum | potenciaNum
        ;

potenciaNum: potenciaNum POTENCIA factorBasico | factorBasico
        ;

factorBasico: NUMERO | ID | PAR_IZQ expresion PAR_DER
        ;

/* Ciclo Do-While */
cicloDoWhile: DO sentenciaWhileDo WHILE PAR_IZQ condicionLogica PAR_DER
            ;

sentenciaWhileDo: asignacion PUNTO_COMA
                | escritura PUNTO_COMA
                | lectura PUNTO_COMA
                ;

/* Ciclo For */
cicloFor: FOR ID ASIGNACION limiteInferior TO limiteSuperior DO R_BEGIN listaSentencias END
        ;

limiteInferior: NUMERO
              | ID
              ;

limiteSuperior: NUMERO
              | ID
              ;

listaSentencias: listaSentencias sentenciaPuntoComa
               | sentenciaPuntoComa
               | /* vacío */
               ;

sentenciaPuntoComa: asignacion PUNTO_COMA
                  | lectura PUNTO_COMA
                  | escritura PUNTO_COMA
                  | cicloDoWhile
                  | cicloFor
                  ;

asignacion: ID ASIGNACION expresion
          ;

lectura: INPUT ID
       ;

escritura: OUTPUT ID
         ;
%%
int main(){
    yyparse();
    return 0;
}
void yyerror(char *mensaje){
        extern int yylineno;
        extern char *yytext;
    fprintf(stderr, "Error:  %s en la linea %i cerca de %s\n", mensaje, yylineno, yytext);
}