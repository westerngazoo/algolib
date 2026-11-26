// LAB 11 — Árbol de expansión mínima (grafo NO dirigido, vértices 0..n-1).
//
// Ambas devuelven el PESO TOTAL del MST, o nullopt si el grafo no es conexo.
//   kruskal  O(E lg E)        ordenar aristas + DisjointSet
//   prim     O(E lg V)        con tu BinaryHeap (lab06), inserción perezosa
// Demuestra la correctitud con la propiedad del corte.
#pragma once

#include <algolib/common.hpp>
#include <algolib/lab11/disjoint_set.hpp>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace algolib {

struct Edge {
    std::size_t u, v;
    std::int64_t w;
};

inline std::optional<std::int64_t> kruskal(std::size_t n, const std::vector<Edge>& edges) {
    (void)n; (void)edges; ALGOLIB_TODO();
}

inline std::optional<std::int64_t> prim(std::size_t n, const std::vector<Edge>& edges) {
    (void)n; (void)edges; ALGOLIB_TODO();
}

}  // namespace algolib
