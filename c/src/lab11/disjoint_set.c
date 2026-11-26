/* LAB11 — tu implementación de <algolib/lab11/disjoint_set.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab11/disjoint_set.h>

#include <stdlib.h>
#include <string.h>

struct DisjointSet {
    size_t *parent;
    size_t *rank_or_size;
    size_t n;
    size_t sets;
};

DisjointSet *disjoint_set_new(size_t n) {
    (void)n;
    ALGOLIB_TODO();
}

void disjoint_set_free(DisjointSet *d) {
    (void)d;
    ALGOLIB_TODO();
}

size_t disjoint_set_find(DisjointSet *d, size_t x) {
    (void)d;
    (void)x;
    ALGOLIB_TODO();
}

bool disjoint_set_unite(DisjointSet *d, size_t a, size_t b) {
    (void)d;
    (void)a;
    (void)b;
    ALGOLIB_TODO();
}

bool disjoint_set_same(DisjointSet *d, size_t a, size_t b) {
    (void)d;
    (void)a;
    (void)b;
    ALGOLIB_TODO();
}

size_t disjoint_set_count(const DisjointSet *d) {
    (void)d;
    ALGOLIB_TODO();
}

size_t disjoint_set_size_of(DisjointSet *d, size_t x) {
    (void)d;
    (void)x;
    ALGOLIB_TODO();
}
