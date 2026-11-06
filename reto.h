#ifndef RETO_H
#define RETO_H

/* PG210 - Combos alternantes en un videojuego
 * Matricula 2025-1269 - 20251269@itla.edu.do */

#define MAX_UNIDADES 20
#define MAX_MUESTRAS 8
#define MAX_NOMBRE   16      /* 15 caracteres + '\0' */
#define MAX_ID       9999
#define MAX_VALOR    100

#define ORDEN_POR_SCORE 0    /* score descendente, empate: id ascendente */
#define ORDEN_POR_ID    1    /* id ascendente */

typedef struct {
    int tipo;                /* 0 = paridad, 1 = umbral x >= p */
    int p;                   /* umbral, 0..100 */
} Config;

typedef struct {
    int id;
    char nombre[MAX_NOMBRE]; /* se guarda ya en mayusculas */
    Config config;           /* estructura anidada */
    int score;
    int estado;
} Registro;

/* Clase (0 o 1) de una muestra segun la configuracion de la unidad. */
int clase_de(int x, int tipo, int p);

/* Devuelve el score de una unidad y entrega el estado por referencia.
 * v: muestras de la unidad, m: cantidad de muestras. */
int calcular(const int v[], int m, int tipo, int p,
             const char *nombre, int *estado);

/* Suma recursiva de los scores de r[0..n-1]; caso base n == 0. */
int total_rec(const Registro r[], int n);

/* Insercion sobre r[0..n-1] segun criterio (ORDEN_POR_SCORE / ORDEN_POR_ID). */
void ordenar(Registro r[], int n, int criterio);

/* Busqueda binaria de id q en r[0..n-1] ordenado por id. Indice o -1. */
int buscar(const Registro r[], int n, int q);

/* Lee y valida toda la entrada. 1 si es valida, 0 si hay error. */
int leer_entrada(int *n, int *m, int *q, Registro reg[],
                 int muestras[][MAX_MUESTRAS]);

#endif /* RETO_H */
