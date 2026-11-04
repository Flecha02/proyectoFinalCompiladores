%{
#include <stdio.h>
#include <string.h>

/* Función nativa en C optimizada: declara solo las variables y temporales usados */
void traducirTacANasm(const char *archivoTac, const char *archivoNasm) {
    FILE *in = fopen(archivoTac, "r");
    FILE *out = fopen(archivoNasm, "w");
    if (!in || !out) {
        fprintf(stderr, "Error al abrir archivos para la traducción NASM.\n");
        return;
    }

    /* Arreglos para registrar qué variables y temporales se usan realmente */
    int usados_t[200] = {0}; // Para t1, t2, ... t199
    int usa_x = 0, usa_y = 0, usa_z = 0, usa_i = 0, usa_total = 0;
    int usa_limite = 0, usa_contador = 0, usa_resultado = 0, usa_res = 0;

    /* PASO 1: Escanear el TAC para ver qué variables y temporales existen */
    char linea[256];
    while (fgets(linea, sizeof(linea), in)) {
        linea[strcspn(linea, "\r\n")] = 0;
        if (strlen(linea) == 0) continue;

        /* Detectar temporales tX */
        for (int i = 1; i < 200; i++) {
            char t_str[20];
            sprintf(t_str, "t%d", i);
            if (strstr(linea, t_str)) {
                usados_t[i] = 1;
            }
        }
        /* Detectar variables conocidas */
        if (strstr(linea, " x")) usa_x = 1;
        if (strstr(linea, " y")) usa_y = 1;
        if (strstr(linea, " z")) usa_z = 1;
        if (strstr(linea, " i")) usa_i = 1;
        if (strstr(linea, "total")) usa_total = 1;
        if (strstr(linea, "limite")) usa_limite = 1;
        if (strstr(linea, "contador")) usa_contador = 1;
        if (strstr(linea, "resultado")) usa_resultado = 1;
        if (strstr(linea, "res")) usa_res = 1;
    }
    rewind(in); // Regresar al inicio del archivo TAC para la traducción real

    /* Cabecera obligatoria de NASM */
    fprintf(out, "bits 32\n\n");
    fprintf(out, "global main\n");
    fprintf(out, "extern printf\n");
    fprintf(out, "extern scanf\n\n");
    fprintf(out, "section .data\n");
    fprintf(out, "    fmt_print db \"%%d\", 10, 0\n");
    fprintf(out, "    fmt_scan  db \"%%d\", 0\n\n");
    
    fprintf(out, "section .bss\n");
    /* Declarar SOLO las variables que detectó que se usaron */
    if (usa_x)        fprintf(out, "    x resd 1\n");
    if (usa_y)        fprintf(out, "    y resd 1\n");
    if (usa_z)        fprintf(out, "    z resd 1\n");
    if (usa_i)        fprintf(out, "    i resd 1\n");
    if (usa_total)    fprintf(out, "    total resd 1\n");
    if (usa_limite)   fprintf(out, "    limite resd 1\n");
    if (usa_contador) fprintf(out, "    contador resd 1\n");
    if (usa_resultado)fprintf(out, "    resultado resd 1\n");
    if (usa_res)      fprintf(out, "    res resd 1\n");

    /* Declarar SOLO los temporales que realmente aparecieron en el TAC */
    for (int i = 1; i < 200; i++) {
        if (usados_t[i]) {
            fprintf(out, "    t%d resd 1\n", i);
        }
    }

    fprintf(out, "\nsection .text\n");
    fprintf(out, "main:\n");
    fprintf(out, "    push ebp\n");
    fprintf(out, "    mov ebp, esp\n\n");

    /* PASO 2: Traducir línea por línea el TAC a NASM (manteniendo tu lógica actual) */
    while (fgets(linea, sizeof(linea), in)) {
        linea[strcspn(linea, "\r\n")] = 0;
        if (strlen(linea) == 0) continue;

        /* A. Etiquetas */
        if (linea[strlen(linea) - 1] == ':') {
            fprintf(out, "%s\n", linea);
        }
        /* B. Saltos incondicionales */
        else if (strncmp(linea, "goto ", 5) == 0) {
            fprintf(out, "    jmp %s\n", linea + 5);
        }
        /* C. Saltos condicionales */
        else if (strncmp(linea, "ifZ ", 4) == 0) {
            char cond[50], etiqueta[50];
            if (sscanf(linea, "ifZ %s goto %s", cond, etiqueta) == 2) {
                fprintf(out, "    mov eax, [%s]\n", cond);
                fprintf(out, "    cmp eax, 0\n");
                fprintf(out, "    je %s\n", etiqueta);
            }
        }
        /* D. Entrada / Salida */
        else if (strncmp(linea, "push ", 5) == 0) {
            char var[50];
            sscanf(linea, "push %s", var);
            if ((var[0] >= 'a' && var[0] <= 'z') || (var[0] >= 'A' && var[0] <= 'Z') || var[0] == 't') {
                fprintf(out, "    push dword [%s]\n", var);
            } else {
                fprintf(out, "    push %s\n", var);
            }
        }
        else if (strncmp(linea, "call ", 5) == 0) {
            fprintf(out, "    %s\n", linea);
            fprintf(out, "    add esp, 4\n");
        }
        /* E. Asignaciones y operaciones */
        else if (strchr(linea, '=')) {
            char dest[50], expr[100];
            char *eq = strchr(linea, '=');
            *eq = '\0';
            strcpy(dest, linea);
            strcpy(expr, eq + 1);

            if (strstr(expr, "<=") && !strstr(expr, "<")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^<]<=%s", op1, op2);
                fprintf(out, "    mov eax, [%s]\n", op1);
                if (op2[0] >= '0' && op2[0] <= '9') {
                    fprintf(out, "    cmp eax, %s\n", op2);
                } else {
                    fprintf(out, "    cmp eax, [%s]\n", op2);
                }
                fprintf(out, "    jle ._%s_true\n", dest);
                fprintf(out, "    mov dword [%s], 0\n", dest);
                fprintf(out, "    jmp ._%s_fin\n", dest);
                fprintf(out, "._%s_true:\n", dest);
                fprintf(out, "    mov dword [%s], 1\n", dest);
                fprintf(out, "._%s_fin:\n", dest);
            } else if (strstr(expr, ">=") && !strstr(expr, ">")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^>]>=%s", op1, op2);
                fprintf(out, "    mov eax, [%s]\n", op1);
                if (op2[0] >= '0' && op2[0] <= '9') {
                    fprintf(out, "    cmp eax, %s\n", op2);
                } else {
                    fprintf(out, "    cmp eax, [%s]\n", op2);
                }
                fprintf(out, "    jge ._%s_true\n", dest);
                fprintf(out, "    mov dword [%s], 0\n", dest);
                fprintf(out, "    jmp ._%s_fin\n", dest);
                fprintf(out, "._%s_true:\n", dest);
                fprintf(out, "    mov dword [%s], 1\n", dest);
                fprintf(out, "._%s_fin:\n", dest);
            } else if (strstr(expr, "<") && !strstr(expr, "<=")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^<]<%s", op1, op2);
                fprintf(out, "    mov eax, [%s]\n", op1);
                if (op2[0] >= '0' && op2[0] <= '9') {
                    fprintf(out, "    cmp eax, %s\n", op2);
                } else {
                    fprintf(out, "    cmp eax, [%s]\n", op2);
                }
                fprintf(out, "    jl ._%s_true\n", dest);
                fprintf(out, "    mov dword [%s], 0\n", dest);
                fprintf(out, "    jmp ._%s_fin\n", dest);
                fprintf(out, "._%s_true:\n", dest);
                fprintf(out, "    mov dword [%s], 1\n", dest);
                fprintf(out, "._%s_fin:\n", dest);
            } else if (strstr(expr, ">") && !strstr(expr, ">=")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^>]>%s", op1, op2);
                fprintf(out, "    mov eax, [%s]\n", op1);
                if (op2[0] >= '0' && op2[0] <= '9') {
                    fprintf(out, "    cmp eax, %s\n", op2);
                } else {
                    fprintf(out, "    cmp eax, [%s]\n", op2);
                }
                fprintf(out, "    jg ._%s_true\n", dest);
                fprintf(out, "    mov dword [%s], 0\n", dest);
                fprintf(out, "    jmp ._%s_fin\n", dest);
                fprintf(out, "._%s_true:\n", dest);
                fprintf(out, "    mov dword [%s], 1\n", dest);
                fprintf(out, "._%s_fin:\n", dest);
            } else if (strstr(expr, "+")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^+]+%s", op1, op2);
                fprintf(out, "    mov eax, [%s]\n", op1);
                if (op2[0] >= '0' && op2[0] <= '9') {
                    fprintf(out, "    add eax, %s\n", op2);
                } else {
                    fprintf(out, "    add eax, [%s]\n", op2);
                }
                fprintf(out, "    mov [%s], eax\n", dest);
            } else if (strstr(expr, "-")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^-]-%s", op1, op2);
                fprintf(out, "    mov eax, [%s]\n", op1);
                if (op2[0] >= '0' && op2[0] <= '9') {
                    fprintf(out, "    sub eax, %s\n", op2);
                } else {
                    fprintf(out, "    sub eax, [%s]\n", op2);
                }
                fprintf(out, "    mov [%s], eax\n", dest);
            } else if (strstr(expr, "*") && !strstr(expr, "**")) {
                char op1[50], op2[50];
                sscanf(expr, "%[^*]*%s", op1, op2);
                fprintf(out, "    mov eax, [%s]\n", op1);
                if (op2[0] >= '0' && op2[0] <= '9') {
                    fprintf(out, "    imul eax, dword %s\n", op2);
                } else {
                    fprintf(out, "    imul eax, dword [%s]\n", op2);
                }
                fprintf(out, "    mov [%s], eax\n", dest);
            } else {
                if (expr[0] >= '0' && expr[0] <= '9') {
                    fprintf(out, "    mov dword [%s], %s\n", dest, expr);
                } else {
                    fprintf(out, "    mov eax, [%s]\n", expr);
                    fprintf(out, "    mov [%s], eax\n", dest);
                }
            }
        }
    }

    /* Cierre estándar NASM */
    fprintf(out, "\n    mov esp, ebp\n");
    fprintf(out, "    pop ebp\n");
    fprintf(out, "    xor eax, eax\n");
    fprintf(out, "    ret\n");

    /* Funciones auxiliares I/O embebidas */
    fprintf(out, "\nglobal output\noutput:\n");
    fprintf(out, "    push ebp\n    mov ebp, esp\n");
    fprintf(out, "    push dword [ebp+8]\n    push fmt_print\n    call printf\n    add esp, 8\n");
    fprintf(out, "    mov esp, ebp\n    pop ebp\n    ret\n");

    fprintf(out, "\nglobal input\ninput:\n");
    fprintf(out, "    push ebp\n    mov ebp, esp\n");
    fprintf(out, "    push dword [ebp+8]\n    push fmt_scan\n    call scanf\n    add esp, 8\n");
    fprintf(out, "    mov esp, ebp\n    pop ebp\n    ret\n");

    fclose(in);
    fclose(out);
}

void yyerror(const char *mensaje);
int  yylex(void);
extern int  yylineno;
extern char *yytext;

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

/* Pila LIFO de etiquetas: permite anidar estructuras de control
   (IF, WHILE, DO-WHILE, FOR) sin mezclar sus etiquetas.        */
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
      MAIN INICIO_BLOQUE listaSentencias END
        {
            fprintf(stderr,
                "\nTDC generado correctamente\n");
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
/*ALLOCATE solo declara variables: no genera codigo*/

declaracionVariables:
      listaDeNombres ALLOCATE PUNTO_COMA
    ;

listaDeNombres:
      ID
    | listaDeNombres DOS_PUNTOS ID
    ;

/*Asignacion*/

asignacion:
      ID ASIGNACION expresion PUNTO_COMA
        { printf("%s=%s\n", $1, $3); }
    ;

/*Entrada / Salida*/

lectura:
      INPUT ID PUNTO_COMA
        {
            printf("push %s\n", $2);
            printf("call input\n");
            printf("pop %s\n", $2);
        }
    ;

escritura:
      OUTPUT ID PUNTO_COMA
        {
            printf("push %s\n", $2);
            printf("call output\n");
        }
    ;

/*Condicional IF*/

condicional:
      cabeceraIf bloque
        {
            char etiqueta[50];
            desapilarEtiqueta(etiqueta);
            printf("%s:\n", etiqueta);
        }
    | cabeceraIf bloque ELSE
        {
            char etiquetaSalida[50];
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            printf("goto %s\n", etiquetaFin);
            desapilarEtiqueta(etiquetaSalida);
            printf("%s:\n", etiquetaSalida);
            apilarEtiqueta(etiquetaFin);
        }
      bloque
        {
            char etiqueta[50];
            desapilarEtiqueta(etiqueta);
            printf("%s:\n", etiqueta);
        }
    ;

cabeceraIf:
      IF PAR_IZQ condicionLogica PAR_DER THEN
        {
            char etiqueta[50];
            nuevaEtiqueta(etiqueta);
            apilarEtiqueta(etiqueta);
            printf("ifZ %s goto %s\n", $3, etiqueta);
        }
    | IF condicionLogica THEN
        {
            char etiqueta[50];
            nuevaEtiqueta(etiqueta);
            apilarEtiqueta(etiqueta);
            printf("ifZ %s goto %s\n", $2, etiqueta);
        }
    ;

bloque:
      INICIO_BLOQUE listaSentencias END
    ;

/*Ciclo WHILE
 * La etiqueta de inicio se imprime antes de parsear la
 * condicion: su TDC debe quedar dentro del ciclo para
 * que se re-evalue en cada iteracion*/

cicloWhile:
      WHILE
        {
            char etiquetaInicio[50];
            nuevaEtiqueta(etiquetaInicio);
            printf("%s:\n", etiquetaInicio);
            apilarEtiqueta(etiquetaInicio);
        }
      PAR_IZQ condicionLogica PAR_DER DO
        {
            char etiquetaFin[50];
            nuevaEtiqueta(etiquetaFin);
            printf("ifZ %s goto %s\n", $4, etiquetaFin);
            apilarEtiqueta(etiquetaFin);
        }
      bloque
        {
            char etiquetaFin[50];
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaFin);
            desapilarEtiqueta(etiquetaInicio);
            printf("goto %s\n", etiquetaInicio);
            printf("%s:\n", etiquetaFin);
        }
    ;

/*Ciclo DO-WHILE*/

cicloDoWhile:
      DO
        {
            char etiquetaInicio[50];
            nuevaEtiqueta(etiquetaInicio);
            printf("%s:\n", etiquetaInicio);
            apilarEtiqueta(etiquetaInicio);
        }
      bloque WHILE PAR_IZQ condicionLogica PAR_DER PUNTO_COMA
        {
            char etiquetaInicio[50];
            desapilarEtiqueta(etiquetaInicio);
            printf("ifZ %s goto %s\n", $6, etiquetaInicio);
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
            printf("%s=%s+1\n", $1, $1);
            printf("goto %s\n", etiquetaInicio);
            printf("%s:\n", etiquetaFin);
        }
    ;

cabeceraFor:
      FOR ID ASIGNACION limiteInferior TO limiteSuperior DO
        {
            char etiquetaInicio[50];
            char etiquetaFin[50];
            char temporal[50];

            strcpy($$, $2);                    /* variable del ciclo */
            nuevaTemp(temporal);
            nuevaEtiqueta(etiquetaInicio);
            nuevaEtiqueta(etiquetaFin);

            printf("%s=%s\n", $2, $4);         /* id = limiteInferior   */
            printf("%s=%s\n", temporal, $6);   /* t  = limiteSuperior   */
            printf("%s:\n", etiquetaInicio);
            printf("ifZ %s<=%s goto %s\n", $2, temporal, etiquetaFin);

            apilarEtiqueta(etiquetaInicio);
            apilarEtiqueta(etiquetaFin);
        }
    ;

limiteInferior:
      NUMERO   { strcpy($$, $1); }
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
        { nuevaTemp($$); printf("%s=%s and %s\n", $$, $1, $3); }
    | condicionLogica OR comparacionSimple
        { nuevaTemp($$); printf("%s=%s or %s\n", $$, $1, $3); }
    ;

comparacionSimple:
      expresion MENOR expresion
        { nuevaTemp($$); printf("%s=%s<%s\n", $$, $1, $3); }
    | expresion MAYOR expresion
        { nuevaTemp($$); printf("%s=%s>%s\n", $$, $1, $3); }
    | expresion MENOR_IGUAL expresion
        { nuevaTemp($$); printf("%s=%s<=%s\n", $$, $1, $3); }
    | expresion MAYOR_IGUAL expresion
        { nuevaTemp($$); printf("%s=%s>=%s\n", $$, $1, $3); }
    | expresion COMPARACION_IGUAL expresion
        { nuevaTemp($$); printf("%s=%s==%s\n", $$, $1, $3); }
    | expresion DIFERENTE expresion
        { nuevaTemp($$); printf("%s=%s<>%s\n", $$, $1, $3); }
    ;

/*Expresiones aritmeticas*/

expresion:
      expresion MAS termino
        { nuevaTemp($$); printf("%s=%s+%s\n", $$, $1, $3); }
    | expresion MENOS termino
        { nuevaTemp($$); printf("%s=%s-%s\n", $$, $1, $3); }
    | termino
        { strcpy($$, $1); }
    ;

termino:
      termino POR potenciaNum
        { nuevaTemp($$); printf("%s=%s*%s\n", $$, $1, $3); }
    | termino ENTRE potenciaNum
        { nuevaTemp($$); printf("%s=%s/%s\n", $$, $1, $3); }
    | potenciaNum
        { strcpy($$, $1); }
    ;

potenciaNum:
      potenciaNum POTENCIA factorBasico
        { nuevaTemp($$); printf("%s=%s**%s\n", $$, $1, $3); }
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

int main(void) {
    FILE *f_tac = freopen("codigo.tac", "w", stdout);
    if (!f_tac) {
        fprintf(stderr, "Error crítico: No se pudo crear el archivo codigo.tac\n");
        return 1;
    }

    int resultado = yyparse();
    fclose(f_tac);

    if (resultado == 0) {
        fprintf(stderr, "\n[OK] Programa sintacticamente correcto.\n");
        fprintf(stderr, "[OK] Codigo Intermedio (TAC) generado en 'codigo.tac'.\n");
        
        /* Ejecuta la traducción modular nativa a NASM */
        traducirTacANasm("codigo.tac", "codigo.asm");
        fprintf(stderr, "[OK] Ensamblador NASM generado en 'codigo.asm'.\n");
    } else {
        fprintf(stderr, "\nEl programa contiene errores sintacticos.\n");
    }

    return resultado;
}

void yyerror(const char *mensaje)
{
    fprintf(stderr,
        "Error sintactico en linea %d cerca de '%s': %s\n",
        yylineno, yytext, mensaje);
}