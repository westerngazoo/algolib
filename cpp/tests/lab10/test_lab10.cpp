#include <algolib/lab10/shortest_paths.hpp>

#include <algotest.hpp>

#include <tuple>
#include <vector>

using namespace algolib;

namespace {
using Edges = std::vector<std::tuple<std::size_t, std::size_t, std::int64_t>>;

WeightedGraph construir(std::size_t n, const Edges& es) {
    WeightedGraph g(n);
    for (auto [u, v, w] : es) g.add_edge(u, v, w);
    return g;
}

// Oráculo: relajación ingenua V-1 rondas sobre la lista de aristas (sin ciclos negativos).
std::vector<Dist> oraculo(std::size_t n, const Edges& es, std::size_t s) {
    std::vector<Dist> d(n);
    d[s] = 0;
    for (std::size_t r = 0; r + 1 < n; ++r)
        for (auto [u, v, w] : es)
            if (d[u] && (!d[v] || *d[u] + w < *d[v])) d[v] = *d[u] + w;
    return d;
}

Edges aleatorio(algotest::Lcg& rng, std::size_t n, int m, std::int64_t wmin, bool dag) {
    Edges es;
    for (int i = 0; i < m; ++i) {
        auto u = rng.below(n), v = rng.below(n);
        if (dag) { if (u == v) continue; if (u > v) std::swap(u, v); }
        es.emplace_back(u, v, rng.range(wmin, 100));
    }
    return es;
}
}  // namespace

TEST(dijkstra_clrs_24_6) {
    // s=0 t=1 x=2 y=3 z=4  (CLRS Fig. 24.6)
    Edges es{{0, 1, 10}, {0, 3, 5}, {1, 2, 1}, {1, 3, 2}, {2, 4, 4},
             {3, 1, 3},  {3, 2, 9}, {3, 4, 2}, {4, 0, 7}, {4, 2, 6}};
    auto d = dijkstra(construir(5, es), 0);
    CHECK(d == (std::vector<Dist>{0, 8, 9, 5, 7}));
}

TEST(dijkstra_inalcanzable) {
    auto d = dijkstra(construir(3, {{0, 1, 4}}), 0);
    CHECK(d == (std::vector<Dist>{0, 4, std::nullopt}));
}

TEST(dijkstra_aleatorio_vs_oraculo) {
    algotest::Lcg rng(16);
    for (int t = 0; t < 20; ++t) {
        auto es = aleatorio(rng, 60, 300, 0, false);
        CHECK(dijkstra(construir(60, es), 0) == oraculo(60, es, 0));
    }
}

TEST(bellman_ford_pesos_negativos) {
    auto d = bellman_ford(construir(3, {{0, 1, 4}, {0, 2, 5}, {2, 1, -3}}), 0);
    CHECK(d.has_value());
    CHECK(*d == (std::vector<Dist>{0, 2, 5}));
}

TEST(bellman_ford_ciclo_negativo) {
    // ciclo 1 → 2 → 1 de peso -2, alcanzable desde 0
    CHECK(!bellman_ford(construir(3, {{0, 1, 1}, {1, 2, -1}, {2, 1, -1}}), 0).has_value());
    // mismo ciclo NO alcanzable desde 0: debe devolver distancias
    auto d = bellman_ford(construir(3, {{1, 2, -1}, {2, 1, -1}}), 0);
    CHECK(d.has_value());
    CHECK(*d == (std::vector<Dist>{0, std::nullopt, std::nullopt}));
}

TEST(dag_aleatorio_con_negativos) {
    algotest::Lcg rng(17);
    for (int t = 0; t < 20; ++t) {
        auto es = aleatorio(rng, 80, 400, -50, true);
        auto g = construir(80, es);
        auto o = oraculo(80, es, 0);
        CHECK(dag_shortest_paths(g, 0) == o);
        CHECK(*bellman_ford(g, 0) == o);
    }
}

TEST(floyd_warshall_vs_oraculo) {
    // Dos familias sin ciclos negativos: (a) cíclico con pesos >= 0, (b) DAG con negativos.
    algotest::Lcg rng(18);
    for (bool dag : {false, true}) {
        auto es = aleatorio(rng, 40, 200, dag ? -50 : 0, dag);
        auto all = floyd_warshall(construir(40, es));
        CHECK_EQ(all.size(), 40u);
        for (std::size_t s = 0; s < 40; ++s) CHECK(all[s] == oraculo(40, es, s));
    }
}
