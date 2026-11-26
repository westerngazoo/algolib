// LAB 07 — AvlMap<K, V>: diccionario ordenado sobre árbol AVL.
// Notas: notas/L07-bst-avl.md
//
// Invariantes:
//   (BST)  claves(subárbol izq) < clave(nodo) < claves(subárbol der)
//   (AVL)  |h(izq) - h(der)| <= 1 en todo nodo, con h(vacío) = 0, h(hoja) = 1
// Teorema (que demostrarás): h < 1.4405·lg(n + 2) − 0.3277.
//
// Complejidad: insert/find/erase O(lg n) peor caso; keys O(n).
// Sugerencia de representación: std::unique_ptr<Node> left, right; int height;
// funciones libres recursivas que reciben y devuelven unique_ptr<Node>
// (insert(std::unique_ptr<Node> t, ...) -> std::unique_ptr<Node>) simplifican las rotaciones.
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

namespace algolib {

template <class K, class V>
class AvlMap {
public:
    // Inserta o reemplaza. true si la clave era nueva.
    bool insert(const K& key, const V& value) { (void)key; (void)value; ALGOLIB_TODO(); }
    V* find(const K& key) { (void)key; ALGOLIB_TODO(); }
    const V* find(const K& key) const { (void)key; ALGOLIB_TODO(); }
    bool contains(const K& key) const { return find(key) != nullptr; }
    // true si la clave existía.
    bool erase(const K& key) { (void)key; ALGOLIB_TODO(); }

    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    int height() const { ALGOLIB_TODO(); }                 // 0 si vacío

    std::vector<K> keys() const { ALGOLIB_TODO(); }        // recorrido in-order
    std::optional<K> min_key() const { ALGOLIB_TODO(); }
    std::optional<K> max_key() const { ALGOLIB_TODO(); }

private:
    // struct Node { K key; V value; int height; std::unique_ptr<Node> left, right; };
    // std::unique_ptr<Node> root_;
    std::size_t size_ = 0;
};

}  // namespace algolib
