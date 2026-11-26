/* LAB 05 — Ordenamiento.  Notas: notas/L05
 * Misma firma que qsort: (base, n, tamaño de cada elemento, comparador). In-place, ascendente.
 *
 *  función          tiempo (peor)        extra      estable  requisito
 *  insertion_sort   Θ(n²)                O(1)       sí
 *  merge_sort       Θ(n lg n)            Θ(n)       SÍ       debe ser estable
 *  quick_sort       Θ(n lg n) esperado   O(lg n)    no       pivote aleatorio + partición en 3 vías
 *  heap_sort        Θ(n lg n)            O(1)       no       heapify en O(n)
 *  counting_sort    Θ(n + k)             Θ(n + k)   sí       claves en [0, max_key]
 *  radix_sort       Θ(d(n + b))          Θ(n + b)   sí       LSD, base 256, 4 pasadas */
#ifndef ALGOLIB_LAB05_SORTING_H
#define ALGOLIB_LAB05_SORTING_H

#include <algolib/common.h>

void insertion_sort(void *base, size_t n, size_t size, algolib_cmp cmp);
void merge_sort(void *base, size_t n, size_t size, algolib_cmp cmp);
void quick_sort(void *base, size_t n, size_t size, algolib_cmp cmp);
void heap_sort(void *base, size_t n, size_t size, algolib_cmp cmp);
void counting_sort(uint32_t *a, size_t n, uint32_t max_key);
void radix_sort(uint32_t *a, size_t n);

#endif
