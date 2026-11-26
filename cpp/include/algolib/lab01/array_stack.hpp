// LAB 01 — ArrayStack<T>: pila sobre arreglo dinámico.
// Notas: notas/L01-modelo-asintotica-pilas.md
//
// TAREA: implementa cada función marcada con ALGOLIB_TODO().
//
// Invariante de representación (debes preservarlo en toda operación pública):
//   size_ <= cap_
//   [data_, data_ + size_)        objetos construidos
//   [data_ + size_, data_ + cap_) memoria cruda (sin objetos)
//   data_ == nullptr  <=>  cap_ == 0
//
// Requisitos de complejidad:
//   push/emplace  O(1) amortizado (crecimiento geométrico, factor 2)
//   pop/top       O(1) peor caso
//
// Pistas:
//   - Separa reservar memoria de construir objetos:
//       std::allocator<T>{}.allocate(n) / deallocate(p, n)
//       std::construct_at(p, args...) / std::destroy_at(p) / std::destroy_n(p, n)
//   - Al crecer, construye el NUEVO elemento antes de mover los viejos
//     (piensa en s.push(s.top())).
//   - Usa std::move_if_noexcept al trasladar elementos (garantía fuerte).
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

namespace algolib {

template <class T>
class ArrayStack {
public:
    ArrayStack() = default;
    ~ArrayStack() { /* TODO: destruir elementos y liberar memoria */ }

    // Regla de los cinco.
    ArrayStack(const ArrayStack& other) { (void)other; ALGOLIB_TODO(); }
    ArrayStack(ArrayStack&& other) noexcept { (void)other; /* TODO */ }
    ArrayStack& operator=(ArrayStack other) noexcept {
        swap(other);  // copy-and-swap: ya funciona si swap y los constructores funcionan
        return *this;
    }
    void swap(ArrayStack& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(cap_, other.cap_);
    }

    void push(const T& x) { emplace(x); }
    void push(T&& x) { emplace(std::move(x)); }

    // Construye un elemento en el tope. O(1) amortizado.
    template <class... Args>
    T& emplace(Args&&... args) { ((void)args, ...); ALGOLIB_TODO(); }

    // Quita el tope. Lanza std::out_of_range si está vacía.
    void pop() { ALGOLIB_TODO(); }

    // Tope. Lanza std::out_of_range si está vacía.
    T& top() { ALGOLIB_TODO(); }
    const T& top() const { ALGOLIB_TODO(); }

    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return cap_; }

    void clear() { ALGOLIB_TODO(); }

private:
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t cap_ = 0;
};

}  // namespace algolib
