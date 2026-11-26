/* LAB13 — tu implementación de <algolib/lab13/range_trees.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab13/range_trees.h>

#include <stdlib.h>
#include <string.h>

struct FenwickTree {
    int64_t *bit;   /* índices 1..n */
    size_t n;
};

struct SegmentTree {
    int64_t *t;
    size_t n;
};

FenwickTree *fenwick_new(size_t n) {
    (void)n;
    ALGOLIB_TODO();
}

void fenwick_free(FenwickTree *f) {
    (void)f;
    ALGOLIB_TODO();
}

void fenwick_add(FenwickTree *f, size_t i, int64_t delta) {
    (void)f;
    (void)i;
    (void)delta;
    ALGOLIB_TODO();
}

int64_t fenwick_prefix_sum(const FenwickTree *f, size_t i) {
    (void)f;
    (void)i;
    ALGOLIB_TODO();
}

int64_t fenwick_range_sum(const FenwickTree *f, size_t l, size_t r) {
    return fenwick_prefix_sum(f, r) - fenwick_prefix_sum(f, l);
}

SegmentTree *segtree_new(const int64_t *a, size_t n) {
    (void)a;
    (void)n;
    ALGOLIB_TODO();
}

void segtree_free(SegmentTree *s) {
    (void)s;
    ALGOLIB_TODO();
}

void segtree_set(SegmentTree *s, size_t i, int64_t x) {
    (void)s;
    (void)i;
    (void)x;
    ALGOLIB_TODO();
}

bool segtree_range_min(const SegmentTree *s, size_t l, size_t r, int64_t *out) {
    (void)s;
    (void)l;
    (void)r;
    (void)out;
    ALGOLIB_TODO();
}
