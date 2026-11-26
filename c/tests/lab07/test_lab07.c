#include <algolib/lab07/avl_map.h>

#include "algotest.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

static bool cumple_cota_avl(int h, size_t n) {
    return h < 1.4405 * log2((double)n + 2) - 0.3277 + 1e-9;
}

static void avl_basico(void) {
    AvlMap *m = avl_map_new(sizeof(int), sizeof(int), cmp_int);
    CHECK_EQ(avl_map_size(m), 0);
    CHECK_EQ(avl_map_height(m), 0);
    int k;
    CHECK(!avl_map_min_key(m, &k));
    CHECK(avl_map_insert(m, &(int){20}, &(int){2}));
    CHECK(avl_map_insert(m, &(int){10}, &(int){1}));
    CHECK(avl_map_insert(m, &(int){30}, &(int){3}));
    CHECK(!avl_map_insert(m, &(int){10}, &(int){100}));     /* reemplaza */
    CHECK_EQ(*(int *)avl_map_find(m, &(int){10}), 100);
    CHECK(avl_map_find(m, &(int){99}) == NULL);
    int ks[3];
    CHECK_EQ(avl_map_keys(m, ks), 3);
    CHECK(ks[0] == 10 && ks[1] == 20 && ks[2] == 30);
    CHECK(avl_map_min_key(m, &k)); CHECK_EQ(k, 10);
    CHECK(avl_map_max_key(m, &k)); CHECK_EQ(k, 30);
    CHECK(avl_map_erase(m, &(int){20}));
    CHECK(!avl_map_erase(m, &(int){20}));
    CHECK_EQ(avl_map_size(m), 2);
    avl_map_free(m);
    avl_map_free(NULL);
}

static void avl_secuencial_es_perfecto(void) {
    AvlMap *m = avl_map_new(sizeof(int), sizeof(int), cmp_int);
    for (int i = 1; i <= 7; i++) avl_map_insert(m, &i, &i);
    CHECK_EQ(avl_map_height(m), 3);      /* 1..7 en orden → árbol perfecto */
    avl_map_free(m);
}

static void avl_cota_de_altura(void) {
    AvlMap *m = avl_map_new(sizeof(int), sizeof(int), cmp_int);
    for (int i = 0; i < 100000; i++) {
        avl_map_insert(m, &i, &i);
        if ((i & (i + 1)) == 0) CHECK(cumple_cota_avl(avl_map_height(m), avl_map_size(m)));
    }
    CHECK(cumple_cota_avl(avl_map_height(m), avl_map_size(m)));
    for (int i = 0; i < 100000; i += 3) avl_map_erase(m, &i);
    CHECK(cumple_cota_avl(avl_map_height(m), avl_map_size(m)));
    avl_map_free(m);
}

static void avl_borrar_rebalancea(void) {
    /* Borrar casi todo un lado: sin rotaciones al borrar, quedaría la columna
     * derecha del árbol original (altura ~16 con ~500 claves) y violaría la cota. */
    AvlMap *m = avl_map_new(sizeof(int), sizeof(int), cmp_int);
    for (int i = 0; i < 65536; i++) avl_map_insert(m, &i, &i);
    for (int i = 0; i < 65000; i++) CHECK(avl_map_erase(m, &i));
    CHECK_EQ(avl_map_size(m), 536);
    CHECK(cumple_cota_avl(avl_map_height(m), avl_map_size(m)));
    int *ks = malloc(536 * sizeof *ks);
    CHECK_EQ(avl_map_keys(m, ks), 536);
    for (int i = 0; i < 536; i++) CHECK_EQ(ks[i], 65000 + i);
    free(ks);
    avl_map_free(m);
}

static void avl_aleatorio(void) {
    /* Oráculo: direccionamiento directo sobre claves en [0, 3000]. */
    int valor[3001];
    bool esta[3001] = {false};
    size_t n = 0;
    int *ks = malloc(3001 * sizeof *ks);
    Lcg rng = {12};
    AvlMap *m = avl_map_new(sizeof(int), sizeof(int), cmp_int);
    for (int op = 0; op < 50000; op++) {
        int k = (int)lcg_range(&rng, 0, 3000);
        switch (lcg_below(&rng, 3)) {
            case 0: {
                int v = (int)lcg_range(&rng, 0, 99);
                CHECK_EQ(avl_map_insert(m, &k, &v), !esta[k]);
                if (!esta[k]) n++;
                esta[k] = true;
                valor[k] = v;
                break;
            }
            case 1:
                CHECK_EQ(avl_map_erase(m, &k), esta[k]);
                if (esta[k]) n--;
                esta[k] = false;
                break;
            default: {
                int *p = avl_map_find(m, &k);
                CHECK_EQ(p != NULL, esta[k]);
                if (p) CHECK_EQ(*p, valor[k]);
            }
        }
        CHECK_EQ(avl_map_size(m), n);
        if (op % 5000 == 0) {
            CHECK_EQ(avl_map_keys(m, ks), n);
            size_t j = 0;
            for (int x = 0; x <= 3000; x++)
                if (esta[x]) CHECK_EQ(ks[j++], x);
            CHECK(cumple_cota_avl(avl_map_height(m), n));
        }
    }
    free(ks);
    avl_map_free(m);
}

int main(void) {
    return algotest_run("lab07", (AlgoTest[]){
        T(avl_basico), T(avl_secuencial_es_perfecto), T(avl_cota_de_altura), T(avl_aleatorio), T(avl_borrar_rebalancea), {0}});
}
