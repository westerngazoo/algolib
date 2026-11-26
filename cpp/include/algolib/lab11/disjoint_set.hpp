// LAB 11 — Union-Find y árboles de expansión mínima. Notas: notas/L11-union-find-mst.md
//
// DisjointSet con unión por rango (o tamaño) + compresión de caminos:
//   m operaciones sobre n elementos en O(m·α(n)) (Tarjan 1975).
// find() muta la estructura (compresión) → no es const. En Rust se nota en la
// firma: find(&mut self, ..). (Alternativa: mutable / Cell — discútelo.)
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <vector>

namespace algolib {

class DisjointSet {
public:
    explicit DisjointSet(std::size_t n) { (void)n; ALGOLIB_TODO(); }
    std::size_t find(std::size_t x) { (void)x; ALGOLIB_TODO(); }
    // Une los conjuntos de a y b. false si ya estaban juntos.
    bool unite(std::size_t a, std::size_t b) { (void)a; (void)b; ALGOLIB_TODO(); }
    bool same(std::size_t a, std::size_t b) { return find(a) == find(b); }
    std::size_t count() const { ALGOLIB_TODO(); }                         // # de conjuntos
    std::size_t size_of(std::size_t x) { (void)x; ALGOLIB_TODO(); }      // tamaño del conjunto de x

private:
    std::vector<std::size_t> parent_;
    std::vector<std::size_t> rank_or_size_;
    std::size_t sets_ = 0;
};

}  // namespace algolib
