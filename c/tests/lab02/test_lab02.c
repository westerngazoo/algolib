#include <algolib/lab02/vector.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

static void vector_push_get(void) {
    Vector *v = vector_new(sizeof(long));
    for (long i = 0; i < 1000; i++) CHECK(vector_push_back(v, &(long){i * i}));
    CHECK_EQ(vector_size(v), 1000);
    for (long i = 0; i < 1000; i++) CHECK_EQ(*(long *)vector_get(v, (size_t)i), i * i);
    CHECK(vector_get(v, 1000) == NULL);
    long x;
    CHECK(vector_pop_back(v, &x));
    CHECK_EQ(x, 999L * 999L);
    vector_free(v);
    vector_free(NULL);
}

static void vector_data_con_qsort(void) {
    /* vector_data expone el bloque contiguo: sirve con qsort (el "slice" de C) */
    Vector *v = vector_new(sizeof(int));
    int xs[] = {5, 3, 8, 1};
    for (int i = 0; i < 4; i++) vector_push_back(v, &xs[i]);
    qsort(vector_data(v), vector_size(v), sizeof(int), cmp_int);
    int esperado[] = {1, 3, 5, 8};
    CHECK(memcmp(vector_data(v), esperado, sizeof esperado) == 0);
    vector_free(v);
}

static void vector_insert_erase(void) {
    Vector *v = vector_new(sizeof(int));
    vector_push_back(v, &(int){2});
    vector_push_back(v, &(int){4});
    CHECK(vector_insert(v, 0, &(int){1}));
    CHECK(vector_insert(v, 2, &(int){3}));
    CHECK(vector_insert(v, 4, &(int){5}));
    int e1[] = {1, 2, 3, 4, 5};
    CHECK_EQ(vector_size(v), 5);
    CHECK(memcmp(vector_data(v), e1, sizeof e1) == 0);
    int x;
    CHECK(vector_erase(v, 0, &x)); CHECK_EQ(x, 1);
    CHECK(vector_erase(v, 1, &x)); CHECK_EQ(x, 3);
    CHECK(vector_erase(v, 2, NULL));
    int e2[] = {2, 4};
    CHECK(memcmp(vector_data(v), e2, sizeof e2) == 0);
    CHECK(!vector_insert(v, 3, &x));     /* fuera de rango */
    CHECK(!vector_erase(v, 2, &x));
    CHECK_EQ(vector_size(v), 2);
    vector_free(v);
}

typedef struct { char s[48]; } Texto;

static void vector_insert_alias(void) {
    Vector *v = vector_new(sizeof(Texto));
    Texto t;
    memset(&t, 0, sizeof t);
    strcpy(t.s, "elemento original con varios bytes");
    vector_push_back(v, &t);
    CHECK(vector_shrink_to_fit(v));
    CHECK(vector_insert(v, 0, vector_get(v, 0)));   /* elem apunta DENTRO del bloque */
    CHECK_EQ(vector_size(v), 2);
    CHECK(strcmp(((Texto *)vector_get(v, 0))->s, t.s) == 0);
    CHECK(strcmp(((Texto *)vector_get(v, 1))->s, t.s) == 0);
    vector_free(v);
}

static void vector_reserve_shrink(void) {
    Vector *v = vector_new(sizeof(int));
    CHECK(vector_reserve(v, 100));
    CHECK(vector_capacity(v) >= 100);
    CHECK_EQ(vector_size(v), 0);
    for (int i = 0; i < 10; i++) vector_push_back(v, &i);
    size_t cap = vector_capacity(v);
    CHECK(vector_reserve(v, 5));
    CHECK_EQ(vector_capacity(v), cap);   /* no encoge */
    CHECK(vector_shrink_to_fit(v));
    CHECK_EQ(vector_capacity(v), 10);
    CHECK_EQ(*(int *)vector_get(v, 9), 9);
    vector_clear(v);
    CHECK_EQ(vector_size(v), 0);
    vector_free(v);
}

static void vector_aleatorio_vs_arreglo(void) {
    Lcg rng = {2};
    Vector *v = vector_new(sizeof(int));
    int *o = malloc(6000 * sizeof *o);
    size_t n = 0;
    for (int op = 0; op < 5000; op++) {
        uint64_t r = lcg_below(&rng, 10);
        if (r < 4 || n == 0) {
            int x = (int)lcg_range(&rng, -99, 99);
            vector_push_back(v, &x);
            o[n++] = x;
        } else if (r < 6) {
            size_t i = lcg_below(&rng, n + 1);
            int x = (int)lcg_range(&rng, -99, 99);
            CHECK(vector_insert(v, i, &x));
            memmove(o + i + 1, o + i, (n - i) * sizeof *o);
            o[i] = x;
            n++;
        } else if (r < 8) {
            size_t i = lcg_below(&rng, n);
            int x;
            CHECK(vector_erase(v, i, &x));
            CHECK_EQ(x, o[i]);
            memmove(o + i, o + i + 1, (n - i - 1) * sizeof *o);
            n--;
        } else {
            int x;
            CHECK(vector_pop_back(v, &x));
            CHECK_EQ(x, o[--n]);
        }
        CHECK_EQ(vector_size(v), n);
    }
    CHECK(n == 0 || memcmp(vector_data(v), o, n * sizeof *o) == 0);
    free(o);
    vector_free(v);
}

int main(void) {
    return algotest_run("lab02", (AlgoTest[]){
        T(vector_push_get), T(vector_data_con_qsort), T(vector_insert_erase), T(vector_insert_alias),
        T(vector_reserve_shrink), T(vector_aleatorio_vs_arreglo), {0}});
}
