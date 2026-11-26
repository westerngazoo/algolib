// LAB 04 — RingQueue<T>: cola FIFO sobre buffer circular que crece.
// Notas: notas/L04-colas-hashing.md
//
// Representación: bloque de capacidad m, índice head_ del frente y size_.
//   El elemento i-ésimo (0 = frente) vive en data_[(head_ + i) mod m].
// Al crecer, los elementos deben quedar "desenrollados" en el bloque nuevo.
//
// Complejidad: push O(1) amortizado; pop/front/back/operator[] O(1).
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

namespace algolib {

template <class T>
class RingQueue {
public:
    RingQueue() = default;
    ~RingQueue() { /* TODO */ }
    RingQueue(const RingQueue&) = delete;
    RingQueue& operator=(const RingQueue&) = delete;

    void push(const T& x) { (void)x; ALGOLIB_TODO(); }   // al final
    void pop() { ALGOLIB_TODO(); }                         // quita el frente; out_of_range si vacía
    T& front() { ALGOLIB_TODO(); }
    T& back() { ALGOLIB_TODO(); }
    T& operator[](std::size_t i) { (void)i; ALGOLIB_TODO(); }  // i-ésimo desde el frente
    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return cap_; }

private:
    T* data_ = nullptr;
    std::size_t head_ = 0;
    std::size_t size_ = 0;
    std::size_t cap_ = 0;
};

}  // namespace algolib
