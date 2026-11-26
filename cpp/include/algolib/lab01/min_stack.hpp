// LAB 01 — MinStack<T>: pila con get_min() en O(1) peor caso.  (Problem Set 1, ej. 8)
//
// Requisitos: push, pop, top, get_min en O(1) (push amortizado); memoria extra O(n).
// Debes enunciar y demostrar el invariante que hace correcto a get_min().
//
// Sugerencia: compón tu ArrayStack (reutilización: el TAD como bloque de construcción).
#pragma once

#include <algolib/common.hpp>
#include <algolib/lab01/array_stack.hpp>

#include <cstddef>

namespace algolib {

template <class T>
class MinStack {
public:
    void push(const T& x) { (void)x; ALGOLIB_TODO(); }
    void pop() { ALGOLIB_TODO(); }                     // std::out_of_range si vacía
    const T& top() const { ALGOLIB_TODO(); }           // std::out_of_range si vacía
    const T& get_min() const { ALGOLIB_TODO(); }       // std::out_of_range si vacía
    bool empty() const { ALGOLIB_TODO(); }
    std::size_t size() const { ALGOLIB_TODO(); }

private:
    // Representación libre. Ejemplo: ArrayStack<T> items_; ArrayStack<T> mins_;
};

}  // namespace algolib
