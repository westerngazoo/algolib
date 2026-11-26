// LAB 05 — Ordenamiento. Notas: notas/L05-ordenamiento.md
//
// Todas las funciones ordenan IN-PLACE de forma ascendente según operator<.
// Reciben std::span<T>: una vista (puntero + longitud) sobre memoria contigua,
// el equivalente directo de &mut [T] en Rust. Llamada: merge_sort(std::span{v}).
//
//  función          tiempo (peor)        extra       estable  requisito
//  insertion_sort   Θ(n²)                O(1)        sí
//  merge_sort       Θ(n lg n)            Θ(n)        SÍ       (debe ser estable)
//  quick_sort       Θ(n lg n) esperado   O(lg n) esp no       pivote aleatorio +
//                                                             partición en 3 vías
//  heap_sort        Θ(n lg n)            O(1)        no       heapify en O(n)
//  counting_sort    Θ(n + k)             Θ(n + k)    sí       claves en [0, max_key]
//  radix_sort       Θ(d(n + b))          Θ(n + b)    sí       LSD, base b = 256, d = 4
//
// Los tests incluyen 200 000 elementos ya ordenados y todos iguales: un quicksort
// con pivote fijo o partición en 2 vías tarda Θ(n²) y no terminará a tiempo.
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>

namespace algolib {

template <class T>
void insertion_sort(std::span<T> a) { (void)a; ALGOLIB_TODO(); }

template <class T>
void merge_sort(std::span<T> a) { (void)a; ALGOLIB_TODO(); }

template <class T>
void quick_sort(std::span<T> a) { (void)a; ALGOLIB_TODO(); }

template <class T>
void heap_sort(std::span<T> a) { (void)a; ALGOLIB_TODO(); }

inline void counting_sort(std::span<std::uint32_t> a, std::uint32_t max_key) {
    (void)a; (void)max_key; ALGOLIB_TODO();
}

inline void radix_sort(std::span<std::uint32_t> a) { (void)a; ALGOLIB_TODO(); }

}  // namespace algolib
