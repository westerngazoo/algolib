/* LAB 11 — Union-Find con unión por rango (o tamaño) + compresión de caminos.
 * m operaciones en O(m·α(n)).  Notas: notas/L11 */
#ifndef ALGOLIB_LAB11_DISJOINT_SET_H
#define ALGOLIB_LAB11_DISJOINT_SET_H

#include <algolib/common.h>

typedef struct DisjointSet DisjointSet;

DisjointSet *disjoint_set_new(size_t n);
void disjoint_set_free(DisjointSet *d);
size_t disjoint_set_find(DisjointSet *d, size_t x);
bool disjoint_set_unite(DisjointSet *d, size_t a, size_t b);
bool disjoint_set_same(DisjointSet *d, size_t a, size_t b);
size_t disjoint_set_count(const DisjointSet *d);
size_t disjoint_set_size_of(DisjointSet *d, size_t x);

/* disjoint_set_unite: false si ya estaban en el mismo conjunto.
 * disjoint_set_count: número de conjuntos. */
#endif
