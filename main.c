#include <stdio.h>
#include <string.h>
#include "reto.h"

int clase_de(int x, int tipo, int p)
{
    if (tipo == 0) {
        return x % 2;            /* 0 = par, 1 = impar (x >= 0) */
    }
    return (x >= p) ? 1 : 0;     /* 1 si x >= p, 0 si x < p */
}

int calcular(const int v[], int m, int tipo, int p,
             const char *nombre, int *estado)
{
    int score = 0;
    int racha = 0;   /* alternancias consecutivas que terminan en i */
    int i;

    (void)nombre;    /* la regla de este reto no usa el nombre */
    *estado = 0;

    for (i = 1; i < m; i++) {
        if (clase_de(v[i], tipo, p) != clase_de(v[i - 1], tipo, p)) {
            racha++;
        } else {
            racha = 0;
        }
        /* racha = segmentos de longitud >= 2 que terminan en i */
        score += racha;
        if (racha >= 2) {
            *estado = 1;     /* segmento alternante de longitud >= 3 */
        }
    }
    return score;
}

int total_rec(const Registro r[], int n)
{
    if (n == 0) {
        return 0;
    }
    return r[n - 1].score + total_rec(r, n - 1);
}

static int va_antes(const Registro *a, const Registro *b, int criterio)
{
    if (criterio == ORDEN_POR_ID) {
        return a->id < b->id;
    }
    if (a->score != b->score) {
        return a->score > b->score;
    }
    return a->id < b->id;
}

void ordenar(Registro r[], int n, int criterio)
{
    int i, j;
    Registro clave;

    for (i = 1; i < n; i++) {
        clave = r[i];
        j = i - 1;
        while (j >= 0 && va_antes(&clave, &r[j], criterio)) {
            r[j + 1] = r[j];
            j--;
        }
        r[j + 1] = clave;
    }
}

int buscar(const Registro r[], int n, int q)
{
    int ini = 0;
    int fin = n - 1;
    int mitad;

    while (ini <= fin) {
        mitad = ini + (fin - ini) / 2;
        if (r[mitad].id == q) {
            return mitad;
        }
        if (r[mitad].id < q) {
            ini = mitad + 1;
        } else {
            fin = mitad - 1;
        }
    }
    return -1;
}

static int nombre_valido(const char *s)
{
    size_t len = strlen(s);
    size_t i;
    char c;

    if (len < 1 || len > MAX_NOMBRE - 1) {
        return 0;
    }
    for (i = 0; i < len; i++) {
        c = s[i];
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
              (c >= 'a' && c <= 'z') || c == '_')) {
            return 0;
        }
    }
    return 1;
}

static void a_mayusculas(char *s)
{
    for (; *s != '\0'; s++) {
        if (*s >= 'a' && *s <= 'z') {
            *s = (char)(*s - 'a' + 'A');
        }
    }
}

int leer_entrada(int *n, int *m, int *q, Registro reg[],
                 int muestras[][MAX_MUESTRAS])
{
    char token[64];
    int i, j, k;

    if (scanf("%d %d %d", n, m, q) != 3) return 0;
    if (*n < 1 || *n > MAX_UNIDADES) return 0;
    if (*m < 1 || *m > MAX_MUESTRAS) return 0;
    if (*q < 1 || *q > MAX_ID) return 0;

    for (i = 0; i < *n; i++) {
        if (scanf("%d", &reg[i].id) != 1) return 0;
        if (reg[i].id < 1 || reg[i].id > MAX_ID) return 0;

        if (scanf("%63s", token) != 1) return 0;
        if (!nombre_valido(token)) return 0;
        strcpy(reg[i].nombre, token);
        a_mayusculas(reg[i].nombre);

        if (scanf("%d %d", &reg[i].config.tipo, &reg[i].config.p) != 2) return 0;
        if (reg[i].config.tipo != 0 && reg[i].config.tipo != 1) return 0;
        if (reg[i].config.p < 0 || reg[i].config.p > MAX_VALOR) return 0;

        for (j = 0; j < *m; j++) {
            if (scanf("%d", &muestras[i][j]) != 1) return 0;
            if (muestras[i][j] < 0 || muestras[i][j] > MAX_VALOR) return 0;
        }
        reg[i].score = 0;
        reg[i].estado = 0;
    }

    for (i = 0; i < *n; i++) {           /* id repetidos */
        for (k = i + 1; k < *n; k++) {
            if (reg[i].id == reg[k].id) return 0;
        }
    }
    return 1;
}

int main(void)
{
    int muestras[MAX_UNIDADES][MAX_MUESTRAS];
    Registro reg[MAX_UNIDADES];
    Registro copia[MAX_UNIDADES];
    int n = 0, m = 0, q = 0;
    int i, total, pos;

    if (!leer_entrada(&n, &m, &q, reg, muestras)) {
        printf("ERROR\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        reg[i].score = calcular(muestras[i], m, reg[i].config.tipo,
                                reg[i].config.p, reg[i].nombre,
                                &reg[i].estado);
    }
    total = total_rec(reg, n);

#ifdef MOSTRAR_MEDIA                      /* solo para evidencias (stderr) */
    {
        double media = total / (double)n;
        fprintf(stderr, "media=%.2f\n", media);
    }
#endif

    for (i = 0; i < n; i++) {
        printf("%d %s %d %d\n", reg[i].id, reg[i].nombre,
               reg[i].score, reg[i].estado);
    }
    printf("TOTAL %d\n", total);

    memcpy(copia, reg, sizeof(Registro) * (size_t)n);
    ordenar(copia, n, ORDEN_POR_SCORE);
    printf("ORDER");
    for (i = 0; i < n; i++) {
        printf(" %d", copia[i].id);
    }
    printf("\n");

    memcpy(copia, reg, sizeof(Registro) * (size_t)n);
    ordenar(copia, n, ORDEN_POR_ID);
    pos = buscar(copia, n, q);
    printf("FIND %d %d\n", q, (pos >= 0) ? copia[pos].score : -1);

    return 0;
}
