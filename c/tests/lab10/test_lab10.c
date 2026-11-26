#include <algolib/lab10/wgraph.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

typedef struct { size_t u, v; int64_t w; } Arista;

static WGraph *construir(size_t n, const Arista *es, size_t m) {
    WGraph *g = wgraph_new(n);
    for (size_t i = 0; i < m; i++) wgraph_add_edge(g, es[i].u, es[i].v, es[i].w);
    return g;
}

/* Oráculo: relajación ingenua V-1 rondas (precondición: sin ciclos negativos). */
static void oraculo(size_t n, const Arista *es, size_t m, size_t s, int64_t *d) {
    for (size_t i = 0; i < n; i++) d[i] = DIST_INF;
    d[s] = 0;
    for (size_t r = 0; r + 1 < n; r++)
        for (size_t i = 0; i < m; i++)
            if (d[es[i].u] != DIST_INF && d[es[i].u] + es[i].w < d[es[i].v])
                d[es[i].v] = d[es[i].u] + es[i].w;
}

static size_t aleatorio(Lcg *rng, size_t n, size_t m, int64_t wmin, bool dag, Arista *es) {
    size_t k = 0;
    for (size_t i = 0; i < m; i++) {
        size_t u = lcg_below(rng, n), v = lcg_below(rng, n);
        if (dag) {
            if (u == v) continue;
            if (u > v) { size_t t = u; u = v; v = t; }
        }
        es[k++] = (Arista){u, v, lcg_range(rng, wmin, 100)};
    }
    return k;
}

static void dijkstra_clrs_24_6(void) {
    /* s=0 t=1 x=2 y=3 z=4  (CLRS Fig. 24.6) */
    Arista es[] = {{0, 1, 10}, {0, 3, 5}, {1, 2, 1}, {1, 3, 2}, {2, 4, 4},
                   {3, 1, 3},  {3, 2, 9}, {3, 4, 2}, {4, 0, 7}, {4, 2, 6}};
    WGraph *g = construir(5, es, 10);
    int64_t d[5], e[] = {0, 8, 9, 5, 7};
    dijkstra(g, 0, d);
    CHECK(memcmp(d, e, sizeof e) == 0);
    wgraph_free(g);
    wgraph_free(NULL);
}

static void dijkstra_inalcanzable(void) {
    Arista es[] = {{0, 1, 4}};
    WGraph *g = construir(3, es, 1);
    int64_t d[3];
    dijkstra(g, 0, d);
    CHECK_EQ(d[0], 0); CHECK_EQ(d[1], 4); CHECK(d[2] == DIST_INF);
    wgraph_free(g);
}

static void dijkstra_aleatorio(void) {
    Lcg rng = {16};
    Arista es[300];
    int64_t d[60], o[60];
    for (int t = 0; t < 20; t++) {
        size_t m = aleatorio(&rng, 60, 300, 0, false, es);
        WGraph *g = construir(60, es, m);
        dijkstra(g, 0, d);
        oraculo(60, es, m, 0, o);
        CHECK(memcmp(d, o, sizeof d) == 0);
        wgraph_free(g);
    }
}

static void bellman_ford_negativos_y_ciclos(void) {
    Arista a[] = {{0, 1, 4}, {0, 2, 5}, {2, 1, -3}};
    WGraph *g = construir(3, a, 3);
    int64_t d[3];
    CHECK(bellman_ford(g, 0, d));
    CHECK_EQ(d[0], 0); CHECK_EQ(d[1], 2); CHECK_EQ(d[2], 5);
    wgraph_free(g);

    Arista b[] = {{0, 1, 1}, {1, 2, -1}, {2, 1, -1}};       /* ciclo negativo alcanzable */
    g = construir(3, b, 3);
    CHECK(!bellman_ford(g, 0, d));
    wgraph_free(g);

    Arista c[] = {{1, 2, -1}, {2, 1, -1}};                  /* ciclo negativo NO alcanzable */
    g = construir(3, c, 2);
    CHECK(bellman_ford(g, 0, d));
    CHECK_EQ(d[0], 0); CHECK(d[1] == DIST_INF); CHECK(d[2] == DIST_INF);
    wgraph_free(g);
}

static void dag_aleatorio_con_negativos(void) {
    Lcg rng = {17};
    Arista es[400];
    int64_t d[80], o[80];
    for (int t = 0; t < 20; t++) {
        size_t m = aleatorio(&rng, 80, 400, -50, true, es);
        WGraph *g = construir(80, es, m);
        oraculo(80, es, m, 0, o);
        dag_shortest_paths(g, 0, d);
        CHECK(memcmp(d, o, sizeof d) == 0);
        CHECK(bellman_ford(g, 0, d));
        CHECK(memcmp(d, o, sizeof d) == 0);
        wgraph_free(g);
    }
}

static void floyd_warshall_vs_oraculo(void) {
    Lcg rng = {18};
    Arista es[200];
    int64_t *all = malloc(40 * 40 * sizeof *all), o[40];
    for (int dag = 0; dag <= 1; dag++) {     /* (a) cíclico con pesos >= 0, (b) DAG con negativos */
        size_t m = aleatorio(&rng, 40, 200, dag ? -50 : 0, dag, es);
        WGraph *g = construir(40, es, m);
        floyd_warshall(g, all);
        for (size_t s = 0; s < 40; s++) {
            oraculo(40, es, m, s, o);
            CHECK(memcmp(all + s * 40, o, sizeof o) == 0);
        }
        wgraph_free(g);
    }
    free(all);
}

int main(void) {
    return algotest_run("lab10", (AlgoTest[]){
        T(dijkstra_clrs_24_6), T(dijkstra_inalcanzable), T(dijkstra_aleatorio),
        T(bellman_ford_negativos_y_ciclos), T(dag_aleatorio_con_negativos), T(floyd_warshall_vs_oraculo), {0}});
}
