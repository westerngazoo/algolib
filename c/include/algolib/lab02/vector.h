/* LAB 02 — Vector: arreglo dinámico de propósito general.  Notas: notas/L02
 *
 * push_back O(1) amortizado; get O(1); insert(i)/erase(i) O(n - i);
 * reserve/shrink_to_fit O(n) si realojan. Los índices fuera de rango NO son UB:
 * get devuelve NULL e insert/erase devuelven false.
 * Cuidado con vector_insert(v, 0, vector_get(v, 0)): el elemento es parte de tu bloque. */
#ifndef ALGOLIB_LAB02_VECTOR_H
#define ALGOLIB_LAB02_VECTOR_H

#include <algolib/common.h>

typedef struct Vector Vector;

Vector *vector_new(size_t elem_size);
void vector_free(Vector *v);
size_t vector_size(const Vector *v);
size_t vector_capacity(const Vector *v);
void *vector_get(const Vector *v, size_t i);
void *vector_data(const Vector *v);
bool vector_push_back(Vector *v, const void *elem);
bool vector_pop_back(Vector *v, void *out);
bool vector_insert(Vector *v, size_t i, const void *elem);
bool vector_erase(Vector *v, size_t i, void *out);
bool vector_reserve(Vector *v, size_t n);
bool vector_shrink_to_fit(Vector *v);
void vector_clear(Vector *v);

/* vector_data: puntero al primer elemento (los size() elementos son contiguos),
 *              para usar qsort, memcpy, etc. NULL si la capacidad es 0.
 * vector_insert: inserta ANTES de la posición i (0 <= i <= size).
 * vector_reserve: capacity() >= n al terminar; nunca encoge.
 * vector_shrink_to_fit: capacity() == size(). */
#endif
