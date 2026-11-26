#include <algolib/lab04/hash_map.h>
#include <algolib/lab04/ring_queue.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

static void ring_fifo(void) {
    RingQueue *q = ring_queue_new(sizeof(int));
    for (int i = 0; i < 100; i++) ring_queue_push(q, &i);
    for (int i = 0; i < 100; i++) {
        CHECK_EQ(*(int *)ring_queue_front(q), i);
        int x;
        CHECK(ring_queue_pop(q, &x));
        CHECK_EQ(x, i);
    }
    CHECK_EQ(ring_queue_size(q), 0);
    CHECK(!ring_queue_pop(q, NULL));
    CHECK(ring_queue_front(q) == NULL);
    ring_queue_free(q);
    ring_queue_free(NULL);
}

static void ring_envuelve_y_crece(void) {
    /* Llena la cola, saca 2 y mete 2: con cupo libre NO debe crecer (da la vuelta).
     * Luego una más sí la hace crecer, y el orden debe conservarse. */
    RingQueue *q = ring_queue_new(sizeof(int));
    int n = 0;
    do { ring_queue_push(q, &n); n++; } while (ring_queue_size(q) < 3 || ring_queue_size(q) != ring_queue_capacity(q));
    size_t cap = ring_queue_capacity(q);
    ring_queue_pop(q, NULL);
    ring_queue_pop(q, NULL);
    ring_queue_push(q, &n); n++;
    ring_queue_push(q, &n); n++;
    CHECK_EQ(ring_queue_capacity(q), cap);
    ring_queue_push(q, &n); n++;
    CHECK(ring_queue_capacity(q) > cap);
    for (size_t i = 0; i < ring_queue_size(q); i++) CHECK_EQ(*(int *)ring_queue_get(q, i), (int)i + 2);
    CHECK(ring_queue_get(q, ring_queue_size(q)) == NULL);
    CHECK_EQ(*(int *)ring_queue_back(q), n - 1);
    ring_queue_free(q);
}

static void ring_aleatorio(void) {
    Lcg rng = {4};
    RingQueue *q = ring_queue_new(sizeof(int));
    int *o = malloc(50000 * sizeof *o);
    size_t ini = 0, fin = 0;
    for (int op = 0; op < 50000; op++) {
        if (fin == ini || lcg_below(&rng, 5) < 3) {
            int x = (int)lcg_range(&rng, 0, 1 << 20);
            ring_queue_push(q, &x);
            o[fin++] = x;
        } else {
            int x;
            CHECK(ring_queue_pop(q, &x));
            CHECK_EQ(x, o[ini++]);
        }
        CHECK_EQ(ring_queue_size(q), fin - ini);
        if (fin > ini) {
            CHECK_EQ(*(int *)ring_queue_front(q), o[ini]);
            CHECK_EQ(*(int *)ring_queue_back(q), o[fin - 1]);
        }
    }
    free(o);
    ring_queue_free(q);
}

/* claves de texto de tamaño fijo, para probar claves que no son int */
typedef struct { char s[16]; } Clave;
static size_t hash_clave(const void *k) {              /* FNV-1a */
    const unsigned char *p = (const unsigned char *)((const Clave *)k)->s;
    uint64_t h = 1469598103934665603ULL;
    for (; *p; p++) h = (h ^ *p) * 1099511628211ULL;
    return (size_t)h;
}
static bool eq_clave(const void *a, const void *b) {
    return strcmp(((const Clave *)a)->s, ((const Clave *)b)->s) == 0;
}
static Clave clave(const char *s) {
    Clave k;
    memset(&k, 0, sizeof k);
    strncpy(k.s, s, sizeof k.s - 1);
    return k;
}

static void hash_basico(void) {
    HashMap *m = hash_map_new(sizeof(Clave), sizeof(int), hash_clave, eq_clave);
    Clave uno = clave("uno"), dos = clave("dos"), tres = clave("tres");
    CHECK(hash_map_insert(m, &uno, &(int){1}));
    CHECK(hash_map_insert(m, &dos, &(int){2}));
    CHECK(!hash_map_insert(m, &uno, &(int){11}));        /* reemplaza */
    CHECK_EQ(hash_map_size(m), 2);
    CHECK_EQ(*(int *)hash_map_find(m, &uno), 11);
    *(int *)hash_map_find(m, &dos) += 20;                 /* find da acceso de escritura */
    CHECK_EQ(*(int *)hash_map_find(m, &dos), 22);
    CHECK(hash_map_find(m, &tres) == NULL);
    CHECK(hash_map_erase(m, &uno));
    CHECK(!hash_map_erase(m, &uno));
    CHECK(hash_map_find(m, &uno) == NULL);
    CHECK_EQ(hash_map_size(m), 1);
    hash_map_free(m);
    hash_map_free(NULL);
}

static void hash_factor_de_carga(void) {
    HashMap *m = hash_map_new(sizeof(int), sizeof(int), algolib_hash_int, algolib_eq_int);
    for (int i = 0; i < 100000; i++) hash_map_insert(m, &i, &i);
    CHECK(hash_map_load_factor(m) <= 1.0);
    CHECK(hash_map_load_factor(m) > 0.0);
    for (int i = 0; i < 100000; i++) CHECK_EQ(*(int *)hash_map_find(m, &i), i);
    hash_map_free(m);
}

static size_t hash_constante(const void *k) { (void)k; return 42; }

static void hash_colisiones_totales(void) {
    HashMap *m = hash_map_new(sizeof(int), sizeof(int), hash_constante, algolib_eq_int);
    for (int i = 0; i < 500; i++) hash_map_insert(m, &i, &(int){-i});
    for (int i = 0; i < 500; i += 2) CHECK(hash_map_erase(m, &i));
    for (int i = 0; i < 500; i++) {
        if (i % 2 == 0) CHECK(hash_map_find(m, &i) == NULL);
        else CHECK_EQ(*(int *)hash_map_find(m, &i), -i);
    }
    CHECK_EQ(hash_map_size(m), 250);
    hash_map_free(m);
}

static void hash_aleatorio(void) {
    /* Oráculo: direccionamiento directo (las claves están en [0, 2000]). */
    int valor[2001];
    bool esta[2001] = {false};
    size_t n = 0;
    Lcg rng = {5};
    HashMap *m = hash_map_new(sizeof(int), sizeof(int), algolib_hash_int, algolib_eq_int);
    for (int op = 0; op < 50000; op++) {
        int k = (int)lcg_range(&rng, 0, 2000);
        switch (lcg_below(&rng, 3)) {
            case 0: {
                int v = (int)lcg_range(&rng, 0, 1000);
                CHECK_EQ(hash_map_insert(m, &k, &v), !esta[k]);
                if (!esta[k]) n++;
                esta[k] = true;
                valor[k] = v;
                break;
            }
            case 1:
                CHECK_EQ(hash_map_erase(m, &k), esta[k]);
                if (esta[k]) n--;
                esta[k] = false;
                break;
            default: {
                int *p = hash_map_find(m, &k);
                CHECK_EQ(p != NULL, esta[k]);
                if (p) CHECK_EQ(*p, valor[k]);
            }
        }
        CHECK_EQ(hash_map_size(m), n);
    }
    hash_map_free(m);
}

static void hash_muchos_borrados(void) {
    /* 200 000 claves distintas pasan por la tabla, pero nunca hay más de 64 vivas.
     * Con lápidas que nunca se limpian, las búsquedas fallidas no terminarían. */
    HashMap *m = hash_map_new(sizeof(int), sizeof(int), algolib_hash_int, algolib_eq_int);
    for (int i = 0; i < 200000; i++) {
        CHECK(hash_map_insert(m, &i, &i));
        if (i >= 64) CHECK(hash_map_erase(m, &(int){i - 64}));
        CHECK(hash_map_find(m, &(int){-1 - i}) == NULL);
    }
    CHECK_EQ(hash_map_size(m), 64);
    for (int i = 200000 - 64; i < 200000; i++) CHECK_EQ(*(int *)hash_map_find(m, &i), i);
    hash_map_free(m);
}

int main(void) {
    return algotest_run("lab04", (AlgoTest[]){
        T(ring_fifo), T(ring_envuelve_y_crece), T(ring_aleatorio), T(hash_basico),
        T(hash_factor_de_carga), T(hash_colisiones_totales), T(hash_aleatorio), T(hash_muchos_borrados), {0}});
}
