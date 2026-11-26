/* LAB 09 — Grafos dirigidos: listas de adyacencia y recorridos.  Notas: notas/L09
 * Vértices 0..n-1. Todos los recorridos O(V + E).
 * Los tests usan caminos de 200 000 vértices: el DFS debe ser ITERATIVO.
 * Los arreglos de salida los reserva quien llama (n elementos). */
#ifndef ALGOLIB_LAB09_GRAPH_H
#define ALGOLIB_LAB09_GRAPH_H

#include <algolib/common.h>

#define GRAPH_INF SIZE_MAX

typedef struct Graph Graph;

Graph *graph_new(size_t n);
void graph_free(Graph *g);
bool graph_add_edge(Graph *g, size_t u, size_t v);
size_t graph_num_vertices(const Graph *g);
const size_t *graph_neighbors(const Graph *g, size_t u, size_t *count);
void graph_bfs(const Graph *g, size_t s, size_t *dist);
size_t graph_dfs_preorder(const Graph *g, size_t s, size_t *out);
bool graph_topological_sort(const Graph *g, size_t *out);
void graph_scc(const Graph *g, size_t *comp);

/* graph_neighbors:        vecinos de u en orden de inserción; *count = cuántos.
 * graph_bfs:              dist[v] = # de aristas desde s, o GRAPH_INF si inalcanzable.
 * graph_dfs_preorder:     orden de descubrimiento del DFS recursivo (vecinos en orden
 *                         de inserción) desde s; devuelve cuántos vértices escribió.
 * graph_topological_sort: false si hay un ciclo.
 * graph_scc:              comp[v] = id de su componente fuertemente conexa (ids arbitrarios). */
#endif
