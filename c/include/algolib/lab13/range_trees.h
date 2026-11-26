/* LAB 13 — Árboles para consultas de rango (intervalos semiabiertos [l, r)).
 *   FenwickTree:  add(i, δ) y prefix_sum(i) = a[0] + ... + a[i-1] en O(lg n).
 *   SegmentTree:  mínimo en rango con asignación puntual; construcción O(n), set/range_min O(lg n).
 *                 range_min de un rango vacío devuelve false. */
#ifndef ALGOLIB_LAB13_RANGE_TREES_H
#define ALGOLIB_LAB13_RANGE_TREES_H

#include <algolib/common.h>

typedef struct FenwickTree FenwickTree;
typedef struct SegmentTree SegmentTree;

FenwickTree *fenwick_new(size_t n);
void fenwick_free(FenwickTree *f);
void fenwick_add(FenwickTree *f, size_t i, int64_t delta);
int64_t fenwick_prefix_sum(const FenwickTree *f, size_t i);
int64_t fenwick_range_sum(const FenwickTree *f, size_t l, size_t r);

SegmentTree *segtree_new(const int64_t *a, size_t n);
void segtree_free(SegmentTree *s);
void segtree_set(SegmentTree *s, size_t i, int64_t x);
bool segtree_range_min(const SegmentTree *s, size_t l, size_t r, int64_t *out);

#endif
