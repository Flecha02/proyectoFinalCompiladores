%{
#include <stdio.h>
#include <string.h>
#include "proyectoFinal.tab.h"
%}

%option noyywrap yylineno

%%

[ \t\r\n]+                  ; /* Ignorar espacios, tabulaciones y saltos de linea */

"***"([^*]|\*[^*]|\*\*[^*])*"***"  ; /* Ignorar comentarios */

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

"DO"                        { return DO; }
"WHILE"                     { return WHILE; }
"FOR"                       { return FOR; }
"TO"                        { return TO; }
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

[a-zA-Z][a-zA-Z0-9_]*       { strcpy(yylval.cadena, yytext); return ID; }

[0-9]+                      { strcpy(yylval.cadena, yytext); return NUMERO; }

.                           {
                                fprintf(stderr,
                                    "Token invalido: ('%s') en linea %d\n",
                                    yytext, yylineno);
                                return INVALIDO;
                            }

%%