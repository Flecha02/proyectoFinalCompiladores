%{
#include <stdio.h>
#include <string.h>

void yyerror(const char *mensaje);
int  yylex(void);
extern int  yylineno;
extern char *yytext;

/* Archivos de salida en paralelo */
FILE *tac_out = NULL;
FILE *asm_out = NULL;

static int temp_count = 1;    /* contador de temporales: t1, t2, ...  */
static int label_count = 1;   /* contador de etiquetas:  l1, l2, ...  */

void nuevaTemp(char *temporal)
{
    sprintf(temporal, "t%d", temp_count++);
}

void nuevaEtiqueta(char *etiqueta)
{
    sprintf(etiqueta, "l%d", label_count++);
}

/* Pila LIFO de etiquetas */
#define MAX_ETIQUETAS 100
static char pilaEtiquetas[MAX_ETIQUETAS][50];
static int  topeEtiquetas = 0;

void apilarEtiqueta(const char *etiqueta)
{
    if (topeEtiquetas < MAX_ETIQUETAS)
        strcpy(pilaEtiquetas[topeEtiquetas++], etiqueta);
}

void desapilarEtiqueta(char *etiqueta)
{
    if (topeEtiquetas > 0)
        strcpy(etiqueta, pilaEtiquetas[--topeEtiquetas]);
    else
        etiqueta[0] = '\0';
}
%}

%union{
    char cadena[50];
}

%token ALLOCATE MAIN INICIO_BLOQUE END INPUT OUTPUT IF THEN ELSE AND OR
%token ASIGNACION MAYOR_IGUAL MENOR_IGUAL COMPARACION_IGUAL DIFERENTE POTENCIA
%token DOS_PUNTOS PUNTO_COMA MAS MENOS POR ENTRE PAR_IZQ PAR_DER MENOR MAYOR
%token DO WHILE FOR TO
%token <cadena> ID NUMERO
%token INVALIDO

%type <cadena> expresion termino potenciaNum factorBasico
%type <cadena> condicionLogica comparacionSimple
%type <cadena> limiteInferior limiteSuperior
%type <cadena> cabeceraFor

%start inicio

%%

/*Programa principal*/
inicio:
      MAIN INICIO_BLOQUE 
        {
            /* 1. Cabecera NASM inicial */
            fprintf(asm_out, "bits 32\n\n");
            fprintf(asm_out, "global main\n");
            fprintf(asm_out, "extern printf\n");
            fprintf(asm_out, "extern scanf\n\n");
            fprintf(asm_out, "section .data\n");
            fprintf(asm_out, "    fmt_print db \"%%d\", 10, 0\n");
            fprintf(asm_out, "    fmt_scan  db \"%%d\", 0\n\n");
            fprintf(asm_out, "section .bss\n");
        }
      listaSentencias END
        {
            /* 2. Cierre de ejecución y rutinas de I/O en NASM */
            fprintf(asm_out, "\nsection .text\n");
            fprintf(asm_out, "main:\n");
            fprintf(asm_out, "    push ebp\n");
            fprintf(asm_out, "    mov ebp, esp\n\n");
            
            /* Nota: En un modelo de pasada única, si prefieres que las instrucciones 
               se acumulen en un archivo temporal de texto para el .text, puedes hacerlo. 
               Aquí cerramos el bloque principal del programa estándar: */
            
            fprintf(asm_out, "\n    mov esp, ebp\n");
            fprintf(asm_out, "    pop ebp\n");
            fprintf(asm_out, "    xor eax, eax\n");
            fprintf(asm_out, "    ret\n\n");

            /* Rutinas auxiliares para input / output compatible con C */
            fprintf(asm_out, "global output\n");
            fprintf(asm_out, "output:\n");
            fprintf(asm_out, "    push ebp\n");
            fprintf(asm_out, "    mov ebp, esp\n");
            fprintf(asm_out, "    push dword [ebp+8]\n");
            fprintf(asm_out, "    push fmt_print\n");
            fprintf(asm_out, "    call printf\n");
            fprintf(asm_out, "    add esp, 8\n");
            fprintf(asm_out, "    mov esp, ebp\n");
            fprintf(asm_out, "    pop ebp\n");
            fprintf(asm_out, "    ret\n\n");

            fprintf(asm_out, "global input\n");
            fprintf(asm_out, "input:\n");
            fprintf(asm_out, "    push ebp\n");
            fprintf(asm_out, "    mov ebp, esp\n");
            fprintf(asm_out, "    push dword [ebp+8]\n");
            fprintf(asm_out, "    push fmt_scan\n");
            fprintf(asm_out, "    call scanf\n");
            fprintf(asm_out, "    add esp, 8\n");
            fprintf(asm_out, "    mov esp, ebp\n");
            fprintf(asm_out, "    pop ebp\n");
            fprintf(asm_out, "    ret\n");

            fprintf(stderr, "\nTDC y NASM generados correctamente en paralelo.\n");
        }
    ;

listaSentencias:
    | listaSentencias sentencia
    ;

sentencia:
      declaracionVariables
    | asignacion
    | lectura
    | escritura
    | condicional
    | cicloWhile
    | cicloDoWhile
    | cicloFor
    ;

/*Declaraciones*/
declaracionVariables:
      listaDeNombres ALLOCATE PUNTO_COMA
    ;

listaDeNombres:
      ID 
        { 
            /* Registramos cada variable en la sección .bss de NASM */
            fprintf(asm_out, "    %s resd 1\n", $1); 
        }
    | listaDeNombres DOS_PUNTOS ID
        { 
            fprintf(asm_out, "    %s resd 1\n", $3); 
        }
    ;

/*Asignacion*/
asignacion:
      ID ASIGNACION expresion PUNTO_COMA
        { 
            /* TAC */
            fprintf(tac_out, "%s=%s\n", $1, $3); 
            
            /* NASM Paralelo */
            fprintf(asm_out, "    mov eax, [%s]\n", $3);
            fprintf(asm_out, "    mov [%s], eax\n", $1);
        }
    ;

/*Entrada / Salida*/
lectura:
      INPUT ID PUNTO_COMA
        {
            /* TAC */
            fprintf(tac_out, "push %s\n", $2);
            fprintf(tac_out, "call input\n");
            fprintf(tac_out, "pop %s\n", $2);

            /* NASM Paralelo */
            fprintf(asm_out, "    push %s\n", $2);
            fprintf(asm_out, "    call input\n");
            fprintf(asm_out, "    add esp, 4\n");
        }
    ;

escritura:
      OUTPUT ID PUNTO_COMA
        {
            /* TAC */
            fprintf(tac_out, "push %s\n", $2);
            fprintf(tac_out, "call output\n");

            /* NASM Paralelo */
            fprintf(asm_out, "    push dword [%s]\n", $2);
            fprintf(asm_out, "    call output\n");
            fprintf(asm_out, "    add esp, 4\n");
        }
    ;

/*Condicional IF*/
condicional:
      cabeceraIf bloque
        {
            char etiqueta[50];
            desapilarEtiqueta(etiqueta);
            
            fprintf(tac_out, "%s:\n", etiqueta);
            fprintf(asm_out, "%s:\n", etiqueta);
        }
    | cabeceraIf bloque ELSE
        {
            char etiquetaSalida[50];
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            
            fprintf(tac_out, "goto %s\n", etiquetaFin);
            fprintf(asm_out, "    jmp %s\n", etiquetaFin);
            
            desapilarEtiqueta(etiquetaSalida);
            
            fprintf(tac_out, "%s:\n", etiquetaSalida);
            fprintf(asm_out, "%s:\n", etiquetaSalida);
            
            apilarEtiqueta(etiquetaFin);
        }
      bloque
        {
            char etiqueta[50];
            desapilarEtiqueta(etiqueta);
            
            fprintf(tac_out, "%s:\n", etiqueta);
            fprintf(asm_out, "%s:\n", etiqueta);
        }
    ;

cabeceraIf:
      IF PAR_IZQ condicionLogica PAR_DER THEN
        {
            char etiqueta[50];
            nuevaEtiqueta(etiqueta);
            apilarEtiqueta(etiqueta);
            
            fprintf(tac_out, "ifZ %s goto %s\n", $3, etiqueta);
            fprintf(asm_out, "    mov eax, [%s]\n", $3);
            fprintf(asm_out, "    cmp eax, 0\n");
            fprintf(asm_out, "    je %s\n", etiqueta);
        }
    | IF condicionLogica THEN
        {
            char etiqueta[50];
            nuevaEtiqueta(etiqueta);
            apilarEtiqueta(etiqueta);
            
            fprintf(tac_out, "ifZ %s goto %s\n", $2, etiqueta);
            fprintf(asm_out, "    mov eax, [%s]\n", $2);
            fprintf(asm_out, "    cmp eax, 0\n");
            fprintf(asm_out, "    je %s\n", etiqueta);
        }
    ;

bloque:
      INICIO_BLOQUE listaSentencias END
    ;

/*Ciclo WHILE*/
cicloWhile:
      WHILE
        {
            char etiquetaInicio[50];
            nuevaEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "%s:\n", etiquetaInicio);
            fprintf(asm_out, "%s:\n", etiquetaInicio);
            
            apilarEtiqueta(etiquetaInicio);
        }
      PAR_IZQ condicionLogica PAR_DER DO
        {
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            
            fprintf(tac_out, "ifZ %s goto %s\n", $4, etiquetaFin);
            fprintf(asm_out, "    mov eax, [%s]\n", $4);
            fprintf(asm_out, "    cmp eax, 0\n");
            fprintf(asm_out, "    je %s\n", etiquetaFin);
            
            apilarEtiqueta(etiquetaFin);
        }
      bloque
        {
            char etiquetaFin[50];
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaFin);
            desapilarEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "goto %s\n", etiquetaInicio);
            fprintf(tac_out, "%s:\n", etiquetaFin);
            
            fprintf(asm_out, "    jmp %s\n", etiquetaInicio);
            fprintf(asm_out, "%s:\n", etiquetaFin);
        }
    ;

/*Ciclo DO-WHILE*/
cicloDoWhile:
      DO
        {
            char etiquetaInicio[50];
            nuevaEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "%s:\n", etiquetaInicio);
            fprintf(asm_out, "%s:\n", etiquetaInicio);
            
            apilarEtiqueta(etiquetaInicio);
        }
      bloque WHILE PAR_IZQ condicionLogica PAR_DER PUNTO_COMA
        {
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "ifZ %s goto %s\n", $6, etiquetaInicio);
            fprintf(asm_out, "    mov eax, [%s]\n", $6);
            fprintf(asm_out, "    cmp eax, 0\n");
            fprintf(asm_out, "    je %s\n", etiquetaInicio);
        }
    ;

/*Ciclo FOR*/
cicloFor:
      cabeceraFor bloque
        {
            char etiquetaFin[50];
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaFin);
            desapilarEtiqueta(etiquetaInicio);
            
            fprintf(tac_out, "%s=%s+1\n", $1, $1);
            fprintf(tac_out, "goto %s\n", etiquetaInicio);
            fprintf(tac_out, "%s:\n", etiquetaFin);

            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    add eax, 1\n");
            fprintf(asm_out, "    mov [%s], eax\n", $1);
            fprintf(asm_out, "    jmp %s\n", etiquetaInicio);
            fprintf(asm_out, "%s:\n", etiquetaFin);
        }
    ;

cabeceraFor:
      FOR ID ASIGNACION limiteInferior TO limiteSuperior DO
        {
            char etiquetaInicio[50];
            char etiquetaFin[50];
            char temporal[50];

            strcpy($$, $2);                    
            nuevaTemp(temporal);
            
            /* Registramos el temporal del For en la sección .bss de NASM */
            fprintf(asm_out, "    %s resd 1\n", temporal);

            nuevaEtiqueta(etiquetaInicio);
            nuevaEtiqueta(etiquetaFin);

            fprintf(tac_out, "%s=%s\n", $2, $4);         
            fprintf(tac_out, "%s=%s\n", temporal, $6);   
            fprintf(tac_out, "%s:\n", etiquetaInicio);
            fprintf(tac_out, "ifZ %s<=%s goto %s\n", $2, temporal, etiquetaFin);

            /* Traducción NASM equivalente para la cabecera del FOR */
            fprintf(asm_out, "    mov eax, [%s]\n", $4);
            fprintf(asm_out, "    mov [%s], eax\n", $2);
            fprintf(asm_out, "    mov eax, [%s]\n", $6);
            fprintf(asm_out, "    mov [%s], eax\n", temporal);
            fprintf(asm_out, "%s:\n", etiquetaInicio);
            fprintf(asm_out, "    mov eax, [%s]\n", $2);
            fprintf(asm_out, "    cmp eax, [%s]\n", temporal);
            fprintf(asm_out, "    jle %%_for_ok_%s\n", etiquetaInicio); /* si <= salta a continuar */
            fprintf(asm_out, "    jmp %s\n", etiquetaFin);             /* sino, sale del for */
            fprintf(asm_out, "%%_for_ok_%s:\n", etiquetaInicio);

            apilarEtiqueta(etiquetaInicio);
            apilarEtiqueta(etiquetaFin);
        }
    ;

limiteInferior:
      NUMERO   
        { 
            strcpy($$, $1); 
            /* Si es un número constante, podemos crear una variable auxiliar temporal o manejarlo directo */
        }
    | ID       { strcpy($$, $1); }
    ;

limiteSuperior:
      NUMERO   { strcpy($$, $1); }
    | ID       { strcpy($$, $1); }
    ;

/*Condiciones logicas*/
condicionLogica:
      comparacionSimple
    | condicionLogica AND comparacionSimple
        { 
            nuevaTemp($$); 
            /* Registramos el temporal booleano en .bss */
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s and %s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    and eax, [%s]\n", $3);
            fprintf(asm_out, "    mov [%s], eax\n", $$);
        }
    | condicionLogica OR comparacionSimple
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s or %s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    or eax, [%s]\n", $3);
            fprintf(asm_out, "    mov [%s], eax\n", $$);
        }
    ;

comparacionSimple:
      expresion MENOR expresion
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s<%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    cmp eax, [%s]\n", $3);
            fprintf(asm_out, "    jl %%_lt_%s\n", $$);
            fprintf(asm_out, "    mov dword [%s], 0\n", $$);
            fprintf(asm_out, "    jmp %%_end_lt_%s\n", $$);
            fprintf(asm_out, "%%_lt_%s:\n", $$);
            fprintf(asm_out, "    mov dword [%s], 1\n", $$);
            fprintf(asm_out, "%%_end_lt_%s:\n", $$);
        }
    | expresion MAYOR expresion
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s>%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    cmp eax, [%s]\n", $3);
            fprintf(asm_out, "    jg %%_gt_%s\n", $$);
            fprintf(asm_out, "    mov dword [%s], 0\n", $$);
            fprintf(asm_out, "    jmp %%_end_gt_%s\n", $$);
            fprintf(asm_out, "%%_gt_%s:\n", $$);
            fprintf(asm_out, "    mov dword [%s], 1\n", $$);
            fprintf(asm_out, "%%_end_gt_%s:\n", $$);
        }
    | expresion MENOR_IGUAL expresion
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s<=%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    cmp eax, [%s]\n", $3);
            fprintf(asm_out, "    jle %%_le_%s\n", $$);
            fprintf(asm_out, "    mov dword [%s], 0\n", $$);
            fprintf(asm_out, "    jmp %%_end_le_%s\n", $$);
            fprintf(asm_out, "%%_le_%s:\n", $$);
            fprintf(asm_out, "    mov dword [%s], 1\n", $$);
            fprintf(asm_out, "%%_end_le_%s:\n", $$);
        }
    | expresion MAYOR_IGUAL expresion
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s>=%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    cmp eax, [%s]\n", $3);
            fprintf(asm_out, "    jge %%_ge_%s\n", $$);
            fprintf(asm_out, "    mov dword [%s], 0\n", $$);
            fprintf(asm_out, "    jmp %%_end_ge_%s\n", $$);
            fprintf(asm_out, "%%_ge_%s:\n", $$);
            fprintf(asm_out, "    mov dword [%s], 1\n", $$);
            fprintf(asm_out, "%%_end_ge_%s:\n", $$);
        }
    | expresion COMPARACION_IGUAL expresion
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s==%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    cmp eax, [%s]\n", $3);
            fprintf(asm_out, "    je %%_eq_%s\n", $$);
            fprintf(asm_out, "    mov dword [%s], 0\n", $$);
            fprintf(asm_out, "    jmp %%_end_eq_%s\n", $$);
            fprintf(asm_out, "%%_eq_%s:\n", $$);
            fprintf(asm_out, "    mov dword [%s], 1\n", $$);
            fprintf(asm_out, "%%_end_eq_%s:\n", $$);
        }
    | expresion DIFERENTE expresion
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s<>%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    cmp eax, [%s]\n", $3);
            fprintf(asm_out, "    jne %%_ne_%s\n", $$);
            fprintf(asm_out, "    mov dword [%s], 0\n", $$);
            fprintf(asm_out, "    jmp %%_end_ne_%s\n", $$);
            fprintf(asm_out, "%%_ne_%s:\n", $$);
            fprintf(asm_out, "    mov dword [%s], 1\n", $$);
            fprintf(asm_out, "%%_end_ne_%s:\n", $$);
        }
    ;

/*Expresiones aritmeticas*/
expresion:
      expresion MAS termino
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s+%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    add eax, [%s]\n", $3);
            fprintf(asm_out, "    mov [%s], eax\n", $$);
        }
    | expresion MENOS termino
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s-%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    sub eax, [%s]\n", $3);
            fprintf(asm_out, "    mov [%s], eax\n", $$);
        }
    | termino
        { strcpy($$, $1); }
    ;

termino:
      termino POR potenciaNum
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s*%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    imul eax, dword [%s]\n", $3);
            fprintf(asm_out, "    mov [%s], eax\n", $$);
        }
    | termino ENTRE potenciaNum
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s/%s\n", $$, $1, $3); 
            
            fprintf(asm_out, "    mov eax, [%s]\n", $1);
            fprintf(asm_out, "    cdq\n");
            fprintf(asm_out, "    idiv dword [%s]\n", $3);
            fprintf(asm_out, "    mov [%s], eax\n", $$);
        }
    | potenciaNum
        { strcpy($$, $1); }
    ;

potenciaNum:
      potenciaNum POTENCIA factorBasico
        { 
            nuevaTemp($$); 
            fprintf(asm_out, "    %s resd 1\n", $$);
            
            fprintf(tac_out, "%s=%s**%s\n", $$, $1, $3); 
            
            /* Simulación de potencia por bucle en NASM */
            fprintf(asm_out, "    mov eax, 1\n");
            fprintf(asm_out, "    mov ecx, [%s]\n", $3);
            fprintf(asm_out, "%%_pow_loop_%s:\n", $$);
            fprintf(asm_out, "    cmp ecx, 0\n");
            fprintf(asm_out, "    jle %%_pow_fin_%s\n", $$);
            fprintf(asm_out, "    imul eax, dword [%s]\n", $1);
            fprintf(asm_out, "    dec ecx\n");
            fprintf(asm_out, "    jmp %%_pow_loop_%s\n", $$);
            fprintf(asm_out, "%%_pow_fin_%s:\n", $$);
            fprintf(asm_out, "    mov [%s], eax\n", $$);
        }
    | factorBasico
        { strcpy($$, $1); }
    ;

factorBasico:
      NUMERO
        { strcpy($$, $1); }
    | ID
        { strcpy($$, $1); }
    | PAR_IZQ expresion PAR_DER
        { strcpy($$, $2); }
    ;

%%

int main(void)
{
    /* Abrimos ambos archivos en paralelo */
    tac_out = fopen("codigo.tac", "w");
    asm_out = fopen("codigo.asm", "w");

    if (!tac_out || !asm_out) {
        fprintf(stderr, "Error al crear los archivos de salida TAC o ASM.\n");
        return 1;
    }

    int resultado = yyparse();

    fclose(tac_out);
    fclose(asm_out);

    if (resultado == 0)
        fprintf(stderr, "\nAnalisis completado: Se generaron 'codigo.tac' y 'codigo.asm' en paralelo.\n");
    else
        fprintf(stderr, "\nEl programa contiene errores sintacticos.\n");

    return resultado;
}

void yyerror(const char *mensaje)
{
    fprintf(stderr,
        "Error sintactico en linea %d cerca de '%s': %s\n",
        yylineno, yytext, mensaje);
}