%{
    #include <stdio.h>    
%}
%%
[ \t\n\r]+      {}
"***"[^*]*"***" {return COMENTARIO;}
"ALLOCATE"      {return ALLOCATE;}
"MAIN"          {return MAIN;}
"BEGIN"         {return BEGIN;}
"END"           {return END;}
"INPUT"         {return INPUT;}
"OUTPUT"        {return OUTPUT;}
"IF"            {return IF;}
"THEN"          {return THEN;}
"ELSE"          {return ELSE;}
"AND"           {return AND;}
"OR"            {return OR;}

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