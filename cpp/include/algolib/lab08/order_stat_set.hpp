// LAB 08 — OrderStatSet<K>: conjunto ordenado con estadísticos de orden.
// Notas: notas/L08-arboles-aumentados.md
//
// Árbol balanceado AUMENTADO: cada nodo guarda size = tamaño de su subárbol.
// Debes demostrar que el campo se mantiene en O(1) por rotación (teorema de
// aumentación, CLRS 17.2) y que select/rank son O(lg n).
//
// Puedes partir de tu AVL (lab07) o implementar un treap (aleatorizado).
//
//   select(k): k-ésimo menor, 0-indexado (std::nullopt si k >= size)
//   rank(x):   número de elementos estrictamente menores que x (x no tiene por qué estar)
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <optional>

namespace algolib {

template <class K>
class OrderStatSet {
public:
    bool insert(const K& key) { (void)key; ALGOLIB_TODO(); }   // false si ya estaba
    bool erase(const K& key) { (void)key; ALGOLIB_TODO(); }    // false si no estaba
    bool contains(const K& key) const { (void)key; ALGOLIB_TODO(); }
    std::size_t size() const { ALGOLIB_TODO(); }

    std::optional<K> select(std::size_t k) const { (void)k; ALGOLIB_TODO(); }
    std::size_t rank(const K& key) const { (void)key; ALGOLIB_TODO(); }

private:
    // Representación libre.
};

}  // namespace algolib
