// LAB 06 — BinaryHeap<T, Compare>: cola de prioridad con heap binario implícito.
// Notas: notas/L06-heaps.md
//
// Convención: top() es el elemento MÍNIMO según Compare (min-heap con std::less).
//   (std::priority_queue es max-heap; aquí elegimos min-heap porque es lo que
//    necesitan Dijkstra y Prim en los labs 10 y 11.)
//
// Representación implícita en un arreglo, índices desde 0:
//   padre(i) = (i - 1) / 2,  hijos(i) = 2i + 1, 2i + 2
// Invariante de heap: para todo i > 0, !comp(a[i], a[padre(i)]).
//
// Complejidad: push/pop O(lg n); top O(1); construcción desde vector O(n) (¡demuéstralo!).
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace algolib {

template <class T, class Compare = std::less<T>>
class BinaryHeap {
public:
    BinaryHeap() = default;

    // Heapify bottom-up en O(n).
    explicit BinaryHeap(std::vector<T> items) : a_(std::move(items)) { ALGOLIB_TODO(); }

    void push(const T& x) { (void)x; ALGOLIB_TODO(); }
    void pop() { ALGOLIB_TODO(); }            // std::out_of_range si vacío
    const T& top() const { ALGOLIB_TODO(); }  // std::out_of_range si vacío
    bool empty() const noexcept { return a_.empty(); }
    std::size_t size() const noexcept { return a_.size(); }

    // Vacía el heap devolviendo los elementos en orden (top primero). O(n lg n).
    std::vector<T> drain_sorted() { ALGOLIB_TODO(); }

private:
    std::vector<T> a_;
    Compare comp_{};
    // Sugerencia: void sift_up(std::size_t i); void sift_down(std::size_t i);
};

}  // namespace algolib
