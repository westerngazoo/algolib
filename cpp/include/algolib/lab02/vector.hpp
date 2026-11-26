// LAB 02 — Vector<T>: arreglo dinámico de propósito general con memoria cruda.
// Notas: notas/L02-vector-memoria.md
//
// Generaliza ArrayStack: acceso aleatorio, inserción/borrado en posición arbitraria,
// reserve/shrink_to_fit e iteradores de acceso aleatorio (punteros crudos son
// contiguous iterators válidos en C++20).
//
// Complejidad exigida:
//   push_back            O(1) amortizado         operator[]/at   O(1)
//   insert(i, x)         O(n - i) + amortizado   erase(i)        O(n - i)
//   reserve(n)           O(size) si realoja       shrink_to_fit   O(size)
//
// Garantías de excepción: push_back/insert con garantía fuerte cuando
// T es nothrow-move-constructible o copiable.
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

namespace algolib {

template <class T>
class Vector {
public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;

    Vector() = default;
    Vector(std::initializer_list<T> il) { (void)il; ALGOLIB_TODO(); }
    ~Vector() { /* TODO */ }

    Vector(const Vector& o) { (void)o; ALGOLIB_TODO(); }
    Vector(Vector&& o) noexcept { (void)o; /* TODO */ }
    Vector& operator=(Vector o) noexcept { swap(o); return *this; }
    void swap(Vector& o) noexcept {
        std::swap(data_, o.data_);
        std::swap(size_, o.size_);
        std::swap(cap_, o.cap_);
    }

    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return cap_; }
    bool empty() const noexcept { return size_ == 0; }

    T& operator[](std::size_t i) { (void)i; ALGOLIB_TODO(); }              // sin verificación
    const T& operator[](std::size_t i) const { (void)i; ALGOLIB_TODO(); }
    T& at(std::size_t i) { (void)i; ALGOLIB_TODO(); }                      // std::out_of_range
    const T& at(std::size_t i) const { (void)i; ALGOLIB_TODO(); }
    T& front() { ALGOLIB_TODO(); }
    T& back() { ALGOLIB_TODO(); }

    void push_back(const T& x) { emplace_back(x); }
    void push_back(T&& x) { emplace_back(std::move(x)); }
    template <class... Args>
    T& emplace_back(Args&&... args) { ((void)args, ...); ALGOLIB_TODO(); }
    void pop_back() { ALGOLIB_TODO(); }                                     // std::out_of_range si vacío

    // Inserta antes de la posición i (0 <= i <= size). std::out_of_range si i > size.
    void insert(std::size_t i, const T& x) { (void)i; (void)x; ALGOLIB_TODO(); }
    // Borra la posición i (0 <= i < size). std::out_of_range si i >= size.
    void erase(std::size_t i) { (void)i; ALGOLIB_TODO(); }

    void reserve(std::size_t n) { (void)n; ALGOLIB_TODO(); }   // capacity() >= n al terminar
    void shrink_to_fit() { ALGOLIB_TODO(); }                    // capacity() == size()
    void clear() noexcept { /* TODO */ }

    iterator begin() noexcept { return data_; }
    iterator end() noexcept { return data_ + size_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator end() const noexcept { return data_ + size_; }

private:
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t cap_ = 0;
};

}  // namespace algolib
