/* LAB 10 — Caminos mínimos con pesos.  Notas: notas/L10
 * Distancias int64_t; DIST_INF = inalcanzable.
 *   dijkstra            pesos >= 0                O((V + E) lg V) con tu BinaryHeap
 *   bellman_ford        pesos arbitrarios          O(V·E); false si hay un ciclo negativo alcanzable
 *   dag_shortest_paths  grafo acíclico             O(V + E)
 *   floyd_warshall      todos los pares            Θ(V³); dist es una matriz n×n por filas */
#ifndef ALGOLIB_LAB10_WGRAPH_H
#define ALGOLIB_LAB10_WGRAPH_H

#include <algolib/common.h>

#define DIST_INF INT64_MAX

typedef struct {
    size_t to;
    int64_t w;
} WeightedEdge;

typedef struct WGraph WGraph;

WGraph *wgraph_new(size_t n);
void wgraph_free(WGraph *g);
bool wgraph_add_edge(WGraph *g, size_t u, size_t v, int64_t w);
size_t wgraph_num_vertices(const WGraph *g);
const WeightedEdge *wgraph_neighbors(const WGraph *g, size_t u, size_t *count);
void dijkstra(const WGraph *g, size_t s, int64_t *dist);
bool bellman_ford(const WGraph *g, size_t s, int64_t *dist);
void dag_shortest_paths(const WGraph *g, size_t s, int64_t *dist);
void floyd_warshall(const WGraph *g, int64_t *dist);

#endif
