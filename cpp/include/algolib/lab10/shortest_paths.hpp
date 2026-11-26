// LAB 10 — Caminos mínimos con pesos. Notas: notas/L10-caminos-minimos.md
//
// Distancias como std::optional<int64_t>: nullopt = inalcanzable (∞).
//
//   dijkstra            pesos >= 0                O((V + E) lg V) con tu BinaryHeap (lab06)
//                                                 (sin decrease-key: inserción perezosa)
//   bellman_ford        pesos arbitrarios          O(V·E); nullopt si hay un ciclo negativo
//                                                 ALCANZABLE desde s
//   dag_shortest_paths  grafo acíclico, pesos arb.  O(V + E) (orden topológico + relajación)
//   floyd_warshall      todos los pares            Θ(V³); precondición: sin ciclos negativos
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace algolib {

struct WeightedEdge {
    std::size_t to;
    std::int64_t w;
};

class WeightedGraph {
public:
    explicit WeightedGraph(std::size_t n) { (void)n; ALGOLIB_TODO(); }
    void add_edge(std::size_t u, std::size_t v, std::int64_t w) { (void)u; (void)v; (void)w; ALGOLIB_TODO(); }
    std::size_t num_vertices() const { ALGOLIB_TODO(); }
    const std::vector<WeightedEdge>& neighbors(std::size_t u) const { (void)u; ALGOLIB_TODO(); }

private:
    std::vector<std::vector<WeightedEdge>> adj_;
};

using Dist = std::optional<std::int64_t>;

inline std::vector<Dist> dijkstra(const WeightedGraph& g, std::size_t s) { (void)g; (void)s; ALGOLIB_TODO(); }

inline std::optional<std::vector<Dist>> bellman_ford(const WeightedGraph& g, std::size_t s) {
    (void)g; (void)s; ALGOLIB_TODO();
}

inline std::vector<Dist> dag_shortest_paths(const WeightedGraph& g, std::size_t s) {
    (void)g; (void)s; ALGOLIB_TODO();
}

inline std::vector<std::vector<Dist>> floyd_warshall(const WeightedGraph& g) { (void)g; ALGOLIB_TODO(); }

}  // namespace algolib
