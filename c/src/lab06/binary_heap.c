/* LAB06 — tu implementación de <algolib/lab06/binary_heap.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab06/binary_heap.h>

#include <stdlib.h>
#include <string.h>

struct BinaryHeap {
    unsigned char *data;
    size_t elem_size;
    size_t size;
    size_t cap;
    algolib_cmp cmp;
};

BinaryHeap *binary_heap_new(size_t elem_size, algolib_cmp cmp) {
    (void)elem_size;
    (void)cmp;
    ALGOLIB_TODO();
}

BinaryHeap *binary_heap_from_array(size_t elem_size, algolib_cmp cmp, const void *items, size_t n) {
    (void)elem_size;
    (void)cmp;
    (void)items;
    (void)n;
    ALGOLIB_TODO();
}

void binary_heap_free(BinaryHeap *h) {
    (void)h;
    ALGOLIB_TODO();
}

bool binary_heap_push(BinaryHeap *h, const void *elem) {
    (void)h;
    (void)elem;
    ALGOLIB_TODO();
}

bool binary_heap_pop(BinaryHeap *h, void *out) {
    (void)h;
    (void)out;
    ALGOLIB_TODO();
}

void *binary_heap_top(const BinaryHeap *h) {
    (void)h;
    ALGOLIB_TODO();
}

size_t binary_heap_size(const BinaryHeap *h) {
    (void)h;
    ALGOLIB_TODO();
}
