// LAB 09 — Grafos dirigidos: representación y recorridos.
// Notas: notas/L09-grafos-recorridos.md
//
// Representación: listas de adyacencia, vértices 0..n-1. Θ(V + E) memoria.
// Grafo no dirigido = añadir ambas aristas.
//
// Requisitos:
//   bfs_distances       O(V + E)  distancia en # de aristas desde s (nullopt si inalcanzable)
//   dfs_preorder        O(V + E)  orden de descubrimiento desde s, visitando los vecinos
//                                 en el orden en que se añadieron (semántica del DFS recursivo)
//   topological_sort    O(V + E)  nullopt si hay ciclo
//   scc                 O(V + E)  id de componente fuertemente conexa por vértice
//                                 (ids arbitrarios; Kosaraju o Tarjan)
//
// IMPORTANTE: los tests usan caminos de 200 000 vértices. Una implementación
// recursiva desborda la pila → implementa DFS ITERATIVO con pila explícita.
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <optional>
#include <vector>

namespace algolib {

class Graph {
public:
    explicit Graph(std::size_t n) { (void)n; ALGOLIB_TODO(); }
    void add_edge(std::size_t u, std::size_t v) { (void)u; (void)v; ALGOLIB_TODO(); }
    std::size_t num_vertices() const { ALGOLIB_TODO(); }
    const std::vector<std::size_t>& neighbors(std::size_t u) const { (void)u; ALGOLIB_TODO(); }

private:
    std::vector<std::vector<std::size_t>> adj_;
};

inline std::vector<std::optional<std::size_t>> bfs_distances(const Graph& g, std::size_t s) {
    (void)g; (void)s; ALGOLIB_TODO();
}

inline std::vector<std::size_t> dfs_preorder(const Graph& g, std::size_t s) {
    (void)g; (void)s; ALGOLIB_TODO();
}

inline std::optional<std::vector<std::size_t>> topological_sort(const Graph& g) {
    (void)g; ALGOLIB_TODO();
}

inline std::vector<std::size_t> strongly_connected_components(const Graph& g) {
    (void)g; ALGOLIB_TODO();
}

}  // namespace algolib
