#include "codigo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Nodo *reservas = NULL;
static int temporales = 0;
static int etiquetas = 0;

static void *reservar(size_t tamano)
{
    void *memoria = calloc(1, tamano);
    if (!memoria) {
        fprintf(stderr, "Error: memoria insuficiente.\n");
        liberarNodos();
        exit(EXIT_FAILURE);
    }
    return memoria;
}

Nodo *crearNodo(TipoNodo tipo, const char *texto, Nodo *a, Nodo *b, Nodo *c)
{
    Nodo *nodo = reservar(sizeof(Nodo));
    nodo->tipo = tipo;
    nodo->a = a;
    nodo->b = b;
    nodo->c = c;
    nodo->siguienteReserva = reservas;
    reservas = nodo;
    if (texto) {
        nodo->texto = reservar(strlen(texto) + 1);
        strcpy(nodo->texto, texto);
    }
    return nodo;
}

/* Tambien libera los nodos de un programa con errores sintacticos. */
void liberarNodos(void)
{
    while (reservas) {
        Nodo *actual = reservas;
        reservas = actual->siguienteReserva;
        free(actual->texto);
        free(actual);
    }
}

static int nuevaEtiqueta(void)
{
    return ++etiquetas;
}

static void colocarEtiqueta(int etiqueta)
{
    printf("L%d:\n", etiqueta);
}

/* Cada operacion aritmetica produce un solo temporal. */
static const char *generarExpresion(Nodo *nodo)
{
    if (nodo->tipo == VALOR)
        return nodo->texto;
    const char *izquierda = generarExpresion(nodo->a);
    const char *derecha = generarExpresion(nodo->b);
    /* El signo $ evita confundir temporales con variables como t1. */
    snprintf(nodo->temporal, sizeof(nodo->temporal), "$t%d", ++temporales);
    printf("%s = %s %s %s\n", nodo->temporal, izquierda, nodo->texto, derecha);
    return nodo->temporal;
}

/* Cortocircuito: el segundo operando solo se visita cuando hace falta. */
static void generarCondicion(Nodo *nodo, int verdadero, int falso)
{
    if (nodo->tipo == LOGICO_AND || nodo->tipo == LOGICO_OR) {
        int siguiente = nuevaEtiqueta();
        if (nodo->tipo == LOGICO_AND)
            generarCondicion(nodo->a, siguiente, falso);
        else
            generarCondicion(nodo->a, verdadero, siguiente);
        colocarEtiqueta(siguiente);
        generarCondicion(nodo->b, verdadero, falso);
        return;
    }
    const char *izquierda = generarExpresion(nodo->a);
    const char *derecha = generarExpresion(nodo->b);
    printf("if %s %s %s goto L%d\n", izquierda, nodo->texto, derecha, verdadero);
    printf("goto L%d\n", falso);
}

static void generarSentencia(Nodo *nodo);

static void generarIf(Nodo *nodo)
{
    int verdadero = nuevaEtiqueta();
    int falso = nuevaEtiqueta();
    int fin = nodo->c ? nuevaEtiqueta() : falso;
    generarCondicion(nodo->a, verdadero, falso);
    colocarEtiqueta(verdadero);
    generarSentencia(nodo->b);
    if (nodo->c) {
        printf("goto L%d\n", fin);
        colocarEtiqueta(falso);
        generarSentencia(nodo->c);
    }
    colocarEtiqueta(fin);
}

static void generarSentencia(Nodo *nodo)
{
    if (!nodo)
        return;
    switch (nodo->tipo) {
    case SECUENCIA:
        generarSentencia(nodo->a);
        generarSentencia(nodo->b);
        break;
    case DECLARACION:
        printf("allocate %s\n", nodo->a->texto);
        break;
    case ASIGNAR: {
        const char *valor = generarExpresion(nodo->b);
        printf("%s = %s\n", nodo->a->texto, valor);
        break;
    }
    case LEER:
        printf("input %s\n", nodo->a->texto);
        break;
    case ESCRIBIR:
        printf("output %s\n", nodo->a->texto);
        break;
    case CONDICIONAL:
        generarIf(nodo);
        break;
    default:
        break;
    }
}

void generarCodigo(Nodo *programa)
{
    temporales = 0;
    etiquetas = 0;
    generarSentencia(programa);
}
