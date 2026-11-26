#include <algolib/lab11/disjoint_set.hpp>
#include <algolib/lab11/mst.hpp>

#include <algotest.hpp>

#include <limits>
#include <vector>

using namespace algolib;

namespace {
// Oráculo exponencial: prueba todos los subconjuntos de n-1 aristas (n y E pequeños).
std::optional<std::int64_t> mst_fuerza_bruta(std::size_t n, const std::vector<Edge>& es) {
    if (n <= 1) return 0;
    std::optional<std::int64_t> best;
    std::size_t m = es.size();
    for (std::uint32_t mask = 0; mask < (1u << m); ++mask) {
        if ((std::size_t)__builtin_popcount(mask) != n - 1) continue;
        std::vector<std::size_t> p(n);
        for (std::size_t i = 0; i < n; ++i) p[i] = i;
        auto f = [&](std::size_t x) { while (p[x] != x) x = p[x]; return x; };
        std::int64_t w = 0;
        bool acyclic = true;
        for (std::size_t i = 0; i < m && acyclic; ++i) if (mask >> i & 1) {
            auto a = f(es[i].u), b = f(es[i].v);
            if (a == b) acyclic = false; else { p[a] = b; w += es[i].w; }
        }
        if (acyclic && (!best || w < *best)) best = w;
    }
    return best;
}
}  // namespace

TEST(dsu_basico) {
    DisjointSet d(6);
    CHECK_EQ(d.count(), 6u);
    CHECK(d.unite(0, 1));
    CHECK(d.unite(2, 3));
    CHECK(!d.unite(1, 0));
    CHECK(d.unite(1, 3));
    CHECK(d.same(0, 2));
    CHECK(!d.same(0, 4));
    CHECK_EQ(d.count(), 3u);
    CHECK_EQ(d.size_of(3), 4u);
    CHECK_EQ(d.size_of(5), 1u);
}

TEST(dsu_grande_y_profundo) {
    // Cadena de uniones: sin unión por rango + compresión, find degenera a Θ(n).
    const std::size_t n = 1'000'000;
    DisjointSet d(n);
    for (std::size_t i = 1; i < n; ++i) d.unite(i - 1, i);
    CHECK_EQ(d.count(), 1u);
    for (std::size_t i = 0; i < n; i += 997) CHECK(d.same(0, i));
    CHECK_EQ(d.size_of(n / 2), n);
}

TEST(mst_ejemplo_clrs_23_1) {
    // a..i = 0..8 (CLRS Fig. 23.1): peso del MST = 37
    std::vector<Edge> es{{0, 1, 4}, {0, 7, 8}, {1, 2, 8}, {1, 7, 11}, {2, 3, 7}, {2, 8, 2}, {2, 5, 4},
                         {3, 4, 9}, {3, 5, 14}, {4, 5, 10}, {5, 6, 2}, {6, 7, 1}, {6, 8, 6}, {7, 8, 7}};
    CHECK_EQ(*kruskal(9, es), 37);
    CHECK_EQ(*prim(9, es), 37);
}

TEST(mst_no_conexo) {
    std::vector<Edge> es{{0, 1, 1}, {2, 3, 1}};
    CHECK(!kruskal(4, es).has_value());
    CHECK(!prim(4, es).has_value());
    CHECK_EQ(*kruskal(1, {}), 0);
    CHECK_EQ(*prim(1, {}), 0);
}

TEST(mst_aleatorio_vs_fuerza_bruta) {
    algotest::Lcg rng(19);
    for (int t = 0; t < 60; ++t) {
        std::size_t n = 2 + rng.below(5);    // 2..6 vértices
        std::size_t m = 1 + rng.below(11);   // 1..11 aristas
        std::vector<Edge> es;
        for (std::size_t i = 0; i < m; ++i) es.push_back({rng.below(n), rng.below(n), rng.range(-5, 20)});
        auto o = mst_fuerza_bruta(n, es);
        CHECK(kruskal(n, es) == o);
        CHECK(prim(n, es) == o);
    }
}

TEST(mst_grande_kruskal_igual_prim) {
    // 200 000 vértices, ~10⁶ aristas (con un camino que garantiza conexidad).
    // Kruskal y Prim deben coincidir; un Prim Θ(V²) no termina a tiempo.
    const std::size_t n = 200000;
    algotest::Lcg rng(20);
    std::vector<Edge> es;
    for (std::size_t i = 0; i + 1 < n; ++i) es.push_back({i, i + 1, 1000000 + rng.range(0, 1000)});
    for (int i = 0; i < 800000; ++i) es.push_back({rng.below(n), rng.below(n), rng.range(-1000, 1000000)});
    auto k = kruskal(n, es);
    CHECK(k.has_value());
    CHECK(k == prim(n, es));
}
