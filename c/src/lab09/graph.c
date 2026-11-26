/* LAB09 — tu implementación de <algolib/lab09/graph.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab09/graph.h>

#include <stdlib.h>
#include <string.h>

struct Graph {
    size_t n;
    size_t **adj;    /* adj[u] = arreglo dinámico de vecinos */
    size_t *deg;     /* número de vecinos de u */
    size_t *cap;     /* capacidad de adj[u] */
};

Graph *graph_new(size_t n) {
    (void)n;
    ALGOLIB_TODO();
}

void graph_free(Graph *g) {
    (void)g;
    ALGOLIB_TODO();
}

bool graph_add_edge(Graph *g, size_t u, size_t v) {
    (void)g;
    (void)u;
    (void)v;
    ALGOLIB_TODO();
}

size_t graph_num_vertices(const Graph *g) {
    (void)g;
    ALGOLIB_TODO();
}

const size_t *graph_neighbors(const Graph *g, size_t u, size_t *count) {
    (void)g;
    (void)u;
    (void)count;
    ALGOLIB_TODO();
}

void graph_bfs(const Graph *g, size_t s, size_t *dist) {
    (void)g;
    (void)s;
    (void)dist;
    ALGOLIB_TODO();
}

size_t graph_dfs_preorder(const Graph *g, size_t s, size_t *out) {
    (void)g;
    (void)s;
    (void)out;
    ALGOLIB_TODO();
}

bool graph_topological_sort(const Graph *g, size_t *out) {
    (void)g;
    (void)out;
    ALGOLIB_TODO();
}

void graph_scc(const Graph *g, size_t *comp) {
    (void)g;
    (void)comp;
    ALGOLIB_TODO();
}
