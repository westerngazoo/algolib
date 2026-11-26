// LAB 13 — Trie (árbol de prefijos) sobre el alfabeto 'a'..'z'.
// Notas: notas/L13-tries-arboles-de-rangos.md
//
// Todas las operaciones O(L), L = longitud de la palabra/prefijo (independiente de n).
// count_with_prefix también O(L): mantén un contador por nodo.
// erase debe podar los nodos que queden sin uso (verifica con node_count()).
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <string>

namespace algolib {

class Trie {
public:
    Trie() = default;
    bool insert(const std::string& w) { (void)w; ALGOLIB_TODO(); }             // false si ya estaba
    bool contains(const std::string& w) const { (void)w; ALGOLIB_TODO(); }
    bool starts_with(const std::string& p) const { (void)p; ALGOLIB_TODO(); }
    std::size_t count_with_prefix(const std::string& p) const { (void)p; ALGOLIB_TODO(); }
    bool erase(const std::string& w) { (void)w; ALGOLIB_TODO(); }              // false si no estaba
    std::size_t size() const { ALGOLIB_TODO(); }                               // # de palabras
    std::size_t node_count() const { ALGOLIB_TODO(); }                         // # de nodos incl. raíz

private:
    // Representación libre. Ejemplo: struct Node { std::array<std::unique_ptr<Node>, 26> next; ... };
};

}  // namespace algolib
