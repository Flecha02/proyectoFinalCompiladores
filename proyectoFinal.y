%{
#include <stdio.h>
void yyerror(char *mensaje);
int yylex();
%}
%token ALLOCATE MAIN BEGIN END INPUT OUTPUT IF THEN ELSE AND OR
%token ASIGNACION MAYOR_IGUAL MENOR_IGUAL COMPARACION_IGUAL DIFERENTE POTENCIA
%token DOS_PUNTOS PUNTO_COMA MAS MENOS POR ENTRE PAR_IZQ PAR_DER MENOR MAYOR
%token ID NUMERO INVALIDO
%%
inicio: declaracionVariables | expresion | condicionLogica
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