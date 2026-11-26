/* LAB 06 — BinaryHeap: cola de prioridad (min-heap según cmp).  Notas: notas/L06
 * Arreglo implícito: padre(i) = (i-1)/2, hijos 2i+1 y 2i+2.
 * push/pop O(lg n); top O(1); binary_heap_from_array O(n) (heapify bottom-up). */
#ifndef ALGOLIB_LAB06_BINARY_HEAP_H
#define ALGOLIB_LAB06_BINARY_HEAP_H

#include <algolib/common.h>

typedef struct BinaryHeap BinaryHeap;

BinaryHeap *binary_heap_new(size_t elem_size, algolib_cmp cmp);
BinaryHeap *binary_heap_from_array(size_t elem_size, algolib_cmp cmp, const void *items, size_t n);
void binary_heap_free(BinaryHeap *h);
bool binary_heap_push(BinaryHeap *h, const void *elem);
bool binary_heap_pop(BinaryHeap *h, void *out);
void *binary_heap_top(const BinaryHeap *h);
size_t binary_heap_size(const BinaryHeap *h);

#endif
