// LAB 03 — DList<T>: lista doblemente ligada (deque sobre nodos).
//
// El ownership ya no es un árbol: cada nodo es apuntado por su anterior Y su siguiente.
// En C++ usamos punteros crudos y manejamos la memoria a mano (new/delete), con un
// invariante explícito. (Alternativa elegante: nodo centinela circular.)
//
// Invariante:
//   size_ == 0  <=>  head_ == tail_ == nullptr
//   head_->prev == nullptr, tail_->next == nullptr
//   para todo nodo n con n->next != nullptr: n->next->prev == n
//
// Todas las operaciones O(1) excepto to_vector / destructor (O(n)).
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace algolib {

template <class T>
class DList {
    struct Node {
        T value;
        Node* prev = nullptr;
        Node* next = nullptr;
    };

public:
    DList() = default;
    ~DList() { /* TODO */ }
    DList(const DList&) = delete;
    DList& operator=(const DList&) = delete;

    void push_front(const T& x) { (void)x; ALGOLIB_TODO(); }
    void push_back(const T& x) { (void)x; ALGOLIB_TODO(); }
    void pop_front() { ALGOLIB_TODO(); }   // std::out_of_range si vacía
    void pop_back() { ALGOLIB_TODO(); }    // std::out_of_range si vacía
    T& front() { ALGOLIB_TODO(); }
    T& back() { ALGOLIB_TODO(); }
    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

    std::vector<T> to_vector() const { ALGOLIB_TODO(); }          // frente → final
    std::vector<T> to_vector_reversed() const { ALGOLIB_TODO(); } // final → frente (usa prev)

private:
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
};

}  // namespace algolib
