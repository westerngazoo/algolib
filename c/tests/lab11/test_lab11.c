#include <algolib/lab11/disjoint_set.h>
#include <algolib/lab11/mst.h>

#include "algotest.h"

#include <stdlib.h>

/* Oráculo exponencial: prueba todos los subconjuntos de n-1 aristas (n y m pequeños). */
static bool mst_fuerza_bruta(size_t n, const Edge *es, size_t m, int64_t *best) {
    if (n <= 1) { *best = 0; return true; }
    bool hay = false;
    for (uint32_t mask = 0; mask < (1u << m); mask++) {
        if ((size_t)__builtin_popcount(mask) != n - 1) continue;
        size_t p[8];
        for (size_t i = 0; i < n; i++) p[i] = i;
        int64_t w = 0;
        bool aciclico = true;
        for (size_t i = 0; i < m && aciclico; i++) {
            if (!(mask >> i & 1)) continue;
            size_t a = es[i].u, b = es[i].v;
            while (p[a] != a) a = p[a];
            while (p[b] != b) b = p[b];
            if (a == b) aciclico = false;
            else { p[a] = b; w += es[i].w; }
        }
        if (aciclico && (!hay || w < *best)) { *best = w; hay = true; }
    }
    return hay;
}

static void dsu_basico(void) {
    DisjointSet *d = disjoint_set_new(6);
    CHECK_EQ(disjoint_set_count(d), 6);
    CHECK(disjoint_set_unite(d, 0, 1));
    CHECK(disjoint_set_unite(d, 2, 3));
    CHECK(!disjoint_set_unite(d, 1, 0));
    CHECK(disjoint_set_unite(d, 1, 3));
    CHECK(disjoint_set_same(d, 0, 2));
    CHECK(!disjoint_set_same(d, 0, 4));
    CHECK_EQ(disjoint_set_count(d), 3);
    CHECK_EQ(disjoint_set_size_of(d, 3), 4);
    CHECK_EQ(disjoint_set_size_of(d, 5), 1);
    disjoint_set_free(d);
    disjoint_set_free(NULL);
}

static void dsu_grande_y_profundo(void) {
    size_t n = 1000000;
    DisjointSet *d = disjoint_set_new(n);
    for (size_t i = 1; i < n; i++) disjoint_set_unite(d, i - 1, i);
    CHECK_EQ(disjoint_set_count(d), 1);
    for (size_t i = 0; i < n; i += 997) CHECK(disjoint_set_same(d, 0, i));
    CHECK_EQ(disjoint_set_size_of(d, n / 2), n);
    disjoint_set_free(d);
}

static void mst_clrs_23_1(void) {
    Edge es[] = {{0, 1, 4}, {0, 7, 8}, {1, 2, 8}, {1, 7, 11}, {2, 3, 7}, {2, 8, 2}, {2, 5, 4},
                 {3, 4, 9}, {3, 5, 14}, {4, 5, 10}, {5, 6, 2}, {6, 7, 1}, {6, 8, 6}, {7, 8, 7}};
    int64_t w = 0;
    CHECK(kruskal(9, es, 14, &w)); CHECK_EQ(w, 37);
    w = 0;
    CHECK(prim(9, es, 14, &w)); CHECK_EQ(w, 37);
}

static void mst_no_conexo(void) {
    Edge es[] = {{0, 1, 1}, {2, 3, 1}};
    int64_t w;
    CHECK(!kruskal(4, es, 2, &w));
    CHECK(!prim(4, es, 2, &w));
    CHECK(kruskal(1, NULL, 0, &w)); CHECK_EQ(w, 0);
    CHECK(prim(1, NULL, 0, &w)); CHECK_EQ(w, 0);
}

static void mst_aleatorio_vs_fuerza_bruta(void) {
    Lcg rng = {19};
    for (int t = 0; t < 60; t++) {
        size_t n = 2 + lcg_below(&rng, 5), m = 1 + lcg_below(&rng, 11);
        Edge es[11];
        for (size_t i = 0; i < m; i++)
            es[i] = (Edge){lcg_below(&rng, n), lcg_below(&rng, n), lcg_range(&rng, -5, 20)};
        int64_t o = 0, k = 0, p = 0;
        bool hay = mst_fuerza_bruta(n, es, m, &o);
        CHECK_EQ(kruskal(n, es, m, &k), hay);
        CHECK_EQ(prim(n, es, m, &p), hay);
        if (hay) { CHECK_EQ(k, o); CHECK_EQ(p, o); }
    }
}

static void mst_grande_kruskal_igual_prim(void) {
    /* 200 000 vértices, ~10⁶ aristas (con un camino que garantiza conexidad).
     * Kruskal y Prim deben coincidir; un Prim Θ(V²) no termina a tiempo. */
    enum { N = 200000, EXTRA = 800000 };
    Lcg rng = {20};
    Edge *es = malloc((N - 1 + EXTRA) * sizeof *es);
    size_t m = 0;
    for (size_t i = 0; i + 1 < N; i++) es[m++] = (Edge){i, i + 1, 1000000 + lcg_range(&rng, 0, 1000)};
    for (int i = 0; i < EXTRA; i++) {
        size_t u = lcg_below(&rng, N), v = lcg_below(&rng, N);
        es[m++] = (Edge){u, v, lcg_range(&rng, -1000, 1000000)};
    }
    int64_t tk = 0, tp = 0;
    CHECK(kruskal(N, es, m, &tk));
    CHECK(prim(N, es, m, &tp));
    CHECK_EQ(tk, tp);
    free(es);
}

int main(void) {
    return algotest_run("lab11", (AlgoTest[]){
        T(dsu_basico), T(dsu_grande_y_profundo), T(mst_clrs_23_1), T(mst_no_conexo),
        T(mst_aleatorio_vs_fuerza_bruta), T(mst_grande_kruskal_igual_prim), {0}});
}
