#include <algolib/lab06/binary_heap.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

static void heap_min_basico(void) {
    BinaryHeap *h = binary_heap_new(sizeof(int), cmp_int);
    CHECK_EQ(binary_heap_size(h), 0);
    CHECK(binary_heap_top(h) == NULL);
    int xs[] = {5, 1, 8, 3, 9, 2};
    for (int i = 0; i < 6; i++) binary_heap_push(h, &xs[i]);
    CHECK_EQ(binary_heap_size(h), 6);
    CHECK_EQ(*(int *)binary_heap_top(h), 1);
    int esperado[] = {1, 2, 3, 5, 8, 9};
    for (int i = 0; i < 6; i++) {
        int x;
        CHECK(binary_heap_pop(h, &x));
        CHECK_EQ(x, esperado[i]);
    }
    CHECK(!binary_heap_pop(h, NULL));
    binary_heap_free(h);
    binary_heap_free(NULL);
}

static void heap_comparador_invertido_es_max(void) {
    BinaryHeap *h = binary_heap_new(sizeof(int), cmp_int_desc);
    int xs[] = {2, 4, 1, 3};
    for (int i = 0; i < 4; i++) binary_heap_push(h, &xs[i]);
    for (int e = 4; e >= 1; e--) {
        int x;
        binary_heap_pop(h, &x);
        CHECK_EQ(x, e);
    }
    binary_heap_free(h);
}

static void heap_from_array(void) {
    Lcg rng = {10};
    size_t n = 50000;
    int *v = malloc(n * sizeof *v);
    for (size_t i = 0; i < n; i++) v[i] = (int)lcg_range(&rng, -100000, 100000);
    BinaryHeap *h = binary_heap_from_array(sizeof(int), cmp_int, v, n);
    CHECK_EQ(binary_heap_size(h), n);
    qsort(v, n, sizeof *v, cmp_int);
    for (size_t i = 0; i < n; i++) {
        int x;
        CHECK(binary_heap_pop(h, &x));
        CHECK_EQ(x, v[i]);
    }
    free(v);
    binary_heap_free(h);
}

static void heap_aleatorio(void) {
    /* Oráculo: conteo por valor (los valores están en [0, 500]). */
    size_t cuenta[501] = {0}, n = 0;
    Lcg rng = {11};
    BinaryHeap *h = binary_heap_new(sizeof(int), cmp_int);
    for (int op = 0; op < 50000; op++) {
        if (n == 0 || lcg_below(&rng, 3) != 0) {
            int x = (int)lcg_range(&rng, 0, 500);
            binary_heap_push(h, &x);
            cuenta[x]++;
            n++;
        } else {
            int mn = 0;
            while (cuenta[mn] == 0) mn++;
            CHECK_EQ(*(int *)binary_heap_top(h), mn);
            int x;
            binary_heap_pop(h, &x);
            CHECK_EQ(x, mn);
            cuenta[mn]--;
            n--;
        }
        CHECK_EQ(binary_heap_size(h), n);
    }
    binary_heap_free(h);
}

int main(void) {
    return algotest_run("lab06", (AlgoTest[]){
        T(heap_min_basico), T(heap_comparador_invertido_es_max), T(heap_from_array), T(heap_aleatorio), {0}});
}
