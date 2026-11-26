#include <algolib/lab05/sorting.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

typedef void (*Ordenador)(void *, size_t, size_t, algolib_cmp);

/* Prueba un algoritmo con casos fijos y aleatorios, comparando contra qsort. */
static void probar(Ordenador ordenar, size_t n_rand) {
    Lcg rng = {6};
    int fijos[][6] = {{0}, {1}, {2, 1}, {1, 2}, {3, 3, 3}, {5, -1, 3, -1, 0}};
    size_t nf[] = {0, 1, 2, 2, 3, 5};
    for (int c = 0; c < 6; c++) {
        int a[6], e[6];
        memcpy(a, fijos[c], sizeof a);
        memcpy(e, fijos[c], sizeof e);
        qsort(e, nf[c], sizeof(int), cmp_int);
        ordenar(a, nf[c], sizeof(int), cmp_int);
        CHECK(memcmp(a, e, nf[c] * sizeof(int)) == 0);
    }
    size_t tams[] = {7, 16, 100, n_rand};
    int *a = malloc(n_rand * sizeof *a), *e = malloc(n_rand * sizeof *e);
    for (int t = 0; t < 4; t++) {
        size_t n = tams[t];
        for (int tipo = 0; tipo < 4; tipo++) {
            for (size_t i = 0; i < n; i++) {
                switch (tipo) {
                    case 0: a[i] = (int)lcg_range(&rng, -1000, 1000); break;   /* aleatorio */
                    case 1: a[i] = (int)lcg_range(&rng, 0, 3); break;          /* muchos repetidos */
                    case 2: a[i] = (int)i; break;                              /* ya ordenado */
                    default: a[i] = (int)(n - i); break;                       /* al revés */
                }
            }
            memcpy(e, a, n * sizeof *a);
            qsort(e, n, sizeof(int), cmp_int);
            ordenar(a, n, sizeof(int), cmp_int);
            CHECK(memcmp(a, e, n * sizeof *a) == 0);
        }
    }
    free(a);
    free(e);
}

typedef struct { int key, id; } Item;
static int cmp_item_key(const void *a, const void *b) {        /* ignora id */
    return cmp_int(&((const Item *)a)->key, &((const Item *)b)->key);
}
static int cmp_item_key_id(const void *a, const void *b) {     /* para construir el oráculo */
    int c = cmp_item_key(a, b);
    return c ? c : cmp_int(&((const Item *)a)->id, &((const Item *)b)->id);
}

static void probar_estable(Ordenador ordenar) {
    Lcg rng = {8};
    Item v[2000], e[2000];
    for (int i = 0; i < 2000; i++) v[i] = (Item){(int)lcg_range(&rng, 0, 20), i};
    memcpy(e, v, sizeof v);
    qsort(e, 2000, sizeof(Item), cmp_item_key_id);   /* orden estable = por (key, id original) */
    ordenar(v, 2000, sizeof(Item), cmp_item_key);
    for (int i = 0; i < 2000; i++) {
        CHECK_EQ(v[i].key, e[i].key);
        CHECK_EQ(v[i].id, e[i].id);
    }
}

static void insertion_sort_correcto(void) { probar(insertion_sort, 1000); }
static void insertion_sort_estable(void) { probar_estable(insertion_sort); }
static void merge_sort_correcto(void) { probar(merge_sort, 100000); }
static void merge_sort_estable(void) { probar_estable(merge_sort); }
static void quick_sort_correcto(void) { probar(quick_sort, 100000); }
static void heap_sort_correcto(void) { probar(heap_sort, 100000); }

static void quick_sort_adversarial(void) {
    /* Pivote fijo o partición en 2 vías → Θ(n²): no terminaría a tiempo. */
    size_t n = 200000;
    int *a = malloc(n * sizeof *a);
    for (size_t i = 0; i < n; i++) a[i] = (int)i;
    quick_sort(a, n, sizeof(int), cmp_int);
    for (size_t i = 0; i < n; i++) CHECK_EQ(a[i], (int)i);
    for (size_t i = 0; i < n; i++) a[i] = 7;
    quick_sort(a, n, sizeof(int), cmp_int);
    for (size_t i = 0; i < n; i++) CHECK_EQ(a[i], 7);
    free(a);
}

/* elementos de 64 bytes: prueban que los algoritmos mueven `size` bytes, no solo un int */
typedef struct { char relleno[56]; long clave; } Grande;
static int cmp_grande(const void *a, const void *b) {
    long x = ((const Grande *)a)->clave, y = ((const Grande *)b)->clave;
    return (x > y) - (x < y);
}
static int cmp_long(const void *a, const void *b) {
    long x = *(const long *)a, y = *(const long *)b;
    return (x > y) - (x < y);
}

static void sort_registros_grandes(void) {
    Lcg rng = {12};
    Grande g[300], h[300], k[300];
    for (int i = 0; i < 300; i++) {
        memset(&g[i], 'x', sizeof g[i]);
        g[i].clave = (long)lcg_range(&rng, -500, 500);
    }
    long claves[300];
    for (int i = 0; i < 300; i++) claves[i] = g[i].clave;
    qsort(claves, 300, sizeof(long), cmp_long);
    memcpy(h, g, sizeof g);
    memcpy(k, g, sizeof g);
    merge_sort(g, 300, sizeof(Grande), cmp_grande);
    quick_sort(h, 300, sizeof(Grande), cmp_grande);
    heap_sort(k, 300, sizeof(Grande), cmp_grande);
    for (int i = 0; i < 300; i++) {
        CHECK_EQ(g[i].clave, claves[i]);
        CHECK_EQ(h[i].clave, claves[i]);
        CHECK_EQ(k[i].clave, claves[i]);
        CHECK(g[i].relleno[0] == 'x' && g[i].relleno[55] == 'x');
    }
}

static void counting_y_radix(void) {
    Lcg rng = {9};
    size_t n = 100000;
    uint32_t *a = malloc(n * sizeof *a), *b = malloc(n * sizeof *b);
    uint32_t *ea = malloc(n * sizeof *ea), *eb = malloc(n * sizeof *eb);
    for (size_t i = 0; i < n; i++) a[i] = (uint32_t)lcg_below(&rng, 1000);
    for (size_t i = 0; i < n; i++) b[i] = (uint32_t)lcg_next(&rng) ^ ((uint32_t)lcg_next(&rng) << 31);
    memcpy(ea, a, n * sizeof *a);
    memcpy(eb, b, n * sizeof *b);
    qsort(ea, n, sizeof *ea, cmp_u32);
    qsort(eb, n, sizeof *eb, cmp_u32);
    counting_sort(a, n, 999);
    radix_sort(b, n);
    CHECK(memcmp(a, ea, n * sizeof *a) == 0);
    CHECK(memcmp(b, eb, n * sizeof *b) == 0);
    counting_sort(a, 0, 0);
    radix_sort(b, 0);
    free(a); free(b); free(ea); free(eb);
}

int main(void) {
    return algotest_run("lab05", (AlgoTest[]){
        T(insertion_sort_correcto), T(insertion_sort_estable), T(merge_sort_correcto),
        T(merge_sort_estable), T(quick_sort_correcto), T(heap_sort_correcto),
        T(quick_sort_adversarial), T(sort_registros_grandes), T(counting_y_radix), {0}});
}
