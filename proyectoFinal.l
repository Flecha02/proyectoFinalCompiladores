%{
     #include "proyectoFinal.tab.h"
    #include <stdio.h>    
%}
%%
[ \t\n\r]+      {}
"***"[^*]*"***" {/*Ignorar comentarios por el momento*/;}
"ALLOCATE"      {return ALLOCATE;}
"MAIN"          {return MAIN;}
"BEGIN"         {return R_BEGIN;}
"END"           {return END;}
"INPUT"         {return INPUT;}
"OUTPUT"        {return OUTPUT;}
"IF"            {return IF;}
"THEN"          {return THEN;}
"ELSE"          {return ELSE;}
"AND"           {return AND;}
"OR"            {return OR;}
"DO"            {return DO;}
"WHILE"         {return WHILE;}
"FOR"           {return FOR;}
"TO"            {return TO;}
"DO_KW"          {return DO_KW;}

":="            {return ASIGNACION;}
">="            {return MAYOR_IGUAL;}
"<="            {return MENOR_IGUAL;}
"=="            {return COMPARACION_IGUAL;}
"<>"            {return DIFERENTE;}
"**"            {return POTENCIA;}

":"             {return DOS_PUNTOS;}
";"             {return PUNTO_COMA;}
"+"             {return MAS;}
"-"             {return MENOS;}
"*"             {return POR;}
"/"             {return ENTRE;}
"("             {return PAR_IZQ;}
")"             {return PAR_DER;}
"<"             {return MENOR;}
">"             {return MAYOR;}

[a-zA-Z_][a-zA-Z0-9_]+  {return ID;}
[0-9]+          {return NUMERO;}

.               {return INVALIDO;}
%%