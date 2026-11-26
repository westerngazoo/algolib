/* LAB10 — tu implementación de <algolib/lab10/wgraph.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab10/wgraph.h>

#include <stdlib.h>
#include <string.h>

struct WGraph {
    size_t n;
    WeightedEdge **adj;
    size_t *deg;
    size_t *cap;
};

WGraph *wgraph_new(size_t n) {
    (void)n;
    ALGOLIB_TODO();
}

void wgraph_free(WGraph *g) {
    (void)g;
    ALGOLIB_TODO();
}

bool wgraph_add_edge(WGraph *g, size_t u, size_t v, int64_t w) {
    (void)g;
    (void)u;
    (void)v;
    (void)w;
    ALGOLIB_TODO();
}

size_t wgraph_num_vertices(const WGraph *g) {
    (void)g;
    ALGOLIB_TODO();
}

const WeightedEdge *wgraph_neighbors(const WGraph *g, size_t u, size_t *count) {
    (void)g;
    (void)u;
    (void)count;
    ALGOLIB_TODO();
}

void dijkstra(const WGraph *g, size_t s, int64_t *dist) {
    (void)g;
    (void)s;
    (void)dist;
    ALGOLIB_TODO();
}

bool bellman_ford(const WGraph *g, size_t s, int64_t *dist) {
    (void)g;
    (void)s;
    (void)dist;
    ALGOLIB_TODO();
}

void dag_shortest_paths(const WGraph *g, size_t s, int64_t *dist) {
    (void)g;
    (void)s;
    (void)dist;
    ALGOLIB_TODO();
}

void floyd_warshall(const WGraph *g, int64_t *dist) {
    (void)g;
    (void)dist;
    ALGOLIB_TODO();
}
