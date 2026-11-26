/* LAB 11 — Árbol de expansión mínima de un grafo NO dirigido (vértices 0..n-1).
 * Ambas escriben en *total el peso del MST y devuelven false si el grafo no es conexo.
 *   kruskal  O(E lg E)   ordenar aristas + DisjointSet
 *   prim     O(E lg V)   con tu BinaryHeap (lab06) */
#ifndef ALGOLIB_LAB11_MST_H
#define ALGOLIB_LAB11_MST_H

#include <algolib/common.h>

typedef struct {
    size_t u, v;
    int64_t w;
} Edge;

bool kruskal(size_t n, const Edge *edges, size_t m, int64_t *total);
bool prim(size_t n, const Edge *edges, size_t m, int64_t *total);

#endif
