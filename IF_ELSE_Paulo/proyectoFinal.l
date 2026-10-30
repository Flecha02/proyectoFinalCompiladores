%{
#include <stdio.h>
#include "proyectoFinal.tab.h"
%}

%option noyywrap yylineno noinput nounput never-interactive

%%

[ \t\r\n]+                  ;

"***"([^*]|\*[^*]|\*\*[^*])*"***"  ;

"ALLOCATE"                  { return ALLOCATE; }
"MAIN"                      { return MAIN; }
"BEGIN"                     { return INICIO_BLOQUE; }
"END"                       { return END; }

"INPUT"                     { return INPUT; }
"OUTPUT"                    { return OUTPUT; }

"IF"                        { return IF; }
"THEN"                      { return THEN; }
"ELSE"                      { return ELSE; }

"AND"                       { return AND; }
"OR"                        { return OR; }

":="                        { return ASIGNACION; }

">="                        { return MAYOR_IGUAL; }
"<="                        { return MENOR_IGUAL; }
"=="                        { return COMPARACION_IGUAL; }
"<>"                        { return DIFERENTE; }

"**"                        { return POTENCIA; }

":"                         { return DOS_PUNTOS; }
";"                         { return PUNTO_COMA; }

"+"                         { return MAS; }
"-"                         { return MENOS; }
"*"                         { return POR; }
"/"                         { return ENTRE; }

"("                         { return PAR_IZQ; }
")"                         { return PAR_DER; }

"<"                         { return MENOR; }
">"                         { return MAYOR; }

[a-zA-Z_][a-zA-Z0-9_]*       { yylval.nodo = crearNodo(VALOR, yytext, NULL, NULL, NULL); return ID; }

[0-9]+                      { yylval.nodo = crearNodo(VALOR, yytext, NULL, NULL, NULL); return NUMERO; }

.                           { return INVALIDO; }

%%