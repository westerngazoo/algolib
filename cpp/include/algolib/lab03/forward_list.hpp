// LAB 03 — ForwardList<T>: lista simplemente ligada con std::unique_ptr.
// Notas: notas/L03-listas-ownership.md
//
// Ownership: cada nodo es DUEÑO del siguiente (unique_ptr<Node> next).
// Esto da liberación automática... y un problema: el destructor por defecto es
// RECURSIVO (destruir head destruye head->next, que destruye ...). Con 10^6 nodos
// desborda la pila. Debes escribir un destructor ITERATIVO (ver test).
//
// Complejidad: push_front/pop_front/front O(1); reverse O(n) in-place, O(1) extra.
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace algolib {

template <class T>
class ForwardList {
    struct Node {
        T value;
        std::unique_ptr<Node> next;
    };

public:
    ForwardList() = default;
    ~ForwardList() { /* TODO: destrucción iterativa */ }
    ForwardList(const ForwardList&) = delete;            // (ejercicio opcional)
    ForwardList& operator=(const ForwardList&) = delete;
    // ¡No uses `= default` para el movimiento! El defaulted copiaría size_ (dejando
    // a `o` vacía pero con size() != 0) y el operator= defaulted liberaría la lista
    // vieja con la destrucción RECURSIVA de unique_ptr.
    ForwardList(ForwardList&& o) noexcept { (void)o; /* TODO: robar head_ y size_; o queda vacía */ }
    ForwardList& operator=(ForwardList&& o) noexcept {
        ForwardList tmp(std::move(o));  // tmp se lleva lo de o...
        swap(tmp);                      // ...lo intercambia con lo nuestro...
        return *this;                   // ...y al morir tmp libera la lista vieja (con TU destructor)
    }
    void swap(ForwardList& o) noexcept {
        std::swap(head_, o.head_);
        std::swap(size_, o.size_);
    }

    void push_front(const T& x) { (void)x; ALGOLIB_TODO(); }
    void pop_front() { ALGOLIB_TODO(); }            // std::out_of_range si vacía
    T& front() { ALGOLIB_TODO(); }                  // std::out_of_range si vacía
    bool empty() const noexcept { return head_ == nullptr; }
    std::size_t size() const noexcept { return size_; }

    // Invierte la lista en O(n) tiempo y O(1) memoria extra (sin crear nodos).
    void reverse() noexcept { /* TODO */ }

    // Elementos de frente a final (para pruebas).
    std::vector<T> to_vector() const { ALGOLIB_TODO(); }

private:
    std::unique_ptr<Node> head_;
    std::size_t size_ = 0;
};

}  // namespace algolib
