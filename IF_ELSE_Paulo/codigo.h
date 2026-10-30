#ifndef CODIGO_H
#define CODIGO_H

/* Un nodo guarda una expresion o una instruccion del programa. */
typedef enum {
    VALOR, OPERACION, COMPARACION, LOGICO_AND, LOGICO_OR,
    SECUENCIA, DECLARACION, ASIGNAR, LEER, ESCRIBIR, CONDICIONAL
} TipoNodo;

typedef struct Nodo {
    TipoNodo tipo;
    char *texto;
    char temporal[32];
    struct Nodo *a, *b, *c;
    struct Nodo *siguienteReserva;
} Nodo;

Nodo *crearNodo(TipoNodo tipo, const char *texto, Nodo *a, Nodo *b, Nodo *c);
void generarCodigo(Nodo *programa);
void liberarNodos(void);
#endif
