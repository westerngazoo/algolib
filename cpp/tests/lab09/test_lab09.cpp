#include <algolib/lab09/graph.hpp>

#include <algotest.hpp>

#include <algorithm>
#include <numeric>
#include <queue>
#include <vector>

using namespace algolib;

namespace {
// Oráculo independiente: alcanzabilidad por BFS desde cada vértice.
std::vector<std::vector<bool>> cierre_transitivo(std::size_t n, const std::vector<std::pair<std::size_t, std::size_t>>& es) {
    std::vector<std::vector<std::size_t>> adj(n);
    for (auto [u, v] : es) adj[u].push_back(v);
    std::vector<std::vector<bool>> r(n, std::vector<bool>(n, false));
    for (std::size_t s = 0; s < n; ++s) {
        std::queue<std::size_t> q;
        q.push(s);
        r[s][s] = true;
        while (!q.empty()) {
            auto u = q.front(); q.pop();
            for (auto v : adj[u]) if (!r[s][v]) { r[s][v] = true; q.push(v); }
        }
    }
    return r;
}

bool es_orden_topologico(const Graph& g, const std::vector<std::size_t>& ord) {
    std::size_t n = g.num_vertices();
    if (ord.size() != n) return false;
    std::vector<std::size_t> pos(n, n);
    for (std::size_t i = 0; i < n; ++i) {
        if (ord[i] >= n || pos[ord[i]] != n) return false;  // no es permutación
        pos[ord[i]] = i;
    }
    for (std::size_t u = 0; u < n; ++u)
        for (auto v : g.neighbors(u)) if (pos[u] >= pos[v]) return false;
    return true;
}
}  // namespace

TEST(grafo_representacion) {
    Graph g(3);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    CHECK_EQ(g.num_vertices(), 3u);
    CHECK(g.neighbors(0) == (std::vector<std::size_t>{1, 2}));
    CHECK(g.neighbors(2).empty());
}

TEST(bfs_distancias) {
    //  0 → 1 → 2 → 3      4 (aislado)
    //  └───────────↗ (0 → 3)
    Graph g(5);
    g.add_edge(0, 1); g.add_edge(1, 2); g.add_edge(2, 3); g.add_edge(0, 3);
    auto d = bfs_distances(g, 0);
    CHECK_EQ(*d[0], 0u);
    CHECK_EQ(*d[1], 1u);
    CHECK_EQ(*d[2], 2u);
    CHECK_EQ(*d[3], 1u);
    CHECK(!d[4].has_value());
}

TEST(dfs_preorden_semantica_recursiva) {
    Graph g(6);
    g.add_edge(0, 1); g.add_edge(0, 2); g.add_edge(1, 3); g.add_edge(2, 3);
    g.add_edge(3, 4); g.add_edge(2, 5); g.add_edge(4, 2);
    // Recursivo: 0 → 1 → 3 → 4 → 2 → (3 visto) → 5
    CHECK(dfs_preorder(g, 0) == (std::vector<std::size_t>{0, 1, 3, 4, 2, 5}));
    CHECK(dfs_preorder(g, 5) == (std::vector<std::size_t>{5}));
}

TEST(topo_dag_aleatorio_y_ciclo) {
    algotest::Lcg rng(14);
    const std::size_t n = 300;
    std::vector<std::size_t> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    for (std::size_t i = n - 1; i > 0; --i) std::swap(perm[i], perm[rng.below(i + 1)]);
    Graph g(n);
    for (int e = 0; e < 2000; ++e) {
        auto a = rng.below(n), b = rng.below(n);
        if (a == b) continue;
        if (a > b) std::swap(a, b);
        g.add_edge(perm[a], perm[b]);  // siempre "hacia adelante" en perm → DAG
    }
    auto ord = topological_sort(g);
    CHECK(ord.has_value());
    CHECK(es_orden_topologico(g, *ord));

    Graph c(3);
    c.add_edge(0, 1); c.add_edge(1, 2); c.add_edge(2, 0);
    CHECK(!topological_sort(c).has_value());
    Graph lazo(1);
    lazo.add_edge(0, 0);
    CHECK(!topological_sort(lazo).has_value());
}

TEST(scc_vs_cierre_transitivo) {
    algotest::Lcg rng(15);
    for (int trial = 0; trial < 5; ++trial) {
        const std::size_t n = 120;
        std::vector<std::pair<std::size_t, std::size_t>> es;
        Graph g(n);
        for (int e = 0; e < 200; ++e) {
            auto u = rng.below(n), v = rng.below(n);
            es.push_back({u, v});
            g.add_edge(u, v);
        }
        auto comp = strongly_connected_components(g);
        CHECK_EQ(comp.size(), n);
        auto r = cierre_transitivo(n, es);
        for (std::size_t u = 0; u < n; ++u)
            for (std::size_t v = 0; v < n; ++v)
                CHECK_EQ(comp[u] == comp[v], r[u][v] && r[v][u]);
    }
}

TEST(recorridos_iterativos_200k) {
    const std::size_t n = 200000;
    Graph path(n);
    for (std::size_t i = 0; i + 1 < n; ++i) path.add_edge(i, i + 1);
    CHECK_EQ(dfs_preorder(path, 0).size(), n);
    CHECK_EQ(*bfs_distances(path, 0)[n - 1], n - 1);
    auto ord = topological_sort(path);
    CHECK(ord.has_value() && es_orden_topologico(path, *ord));
    path.add_edge(n - 1, 0);  // ahora es un gran ciclo: una sola SCC
    auto comp = strongly_connected_components(path);
    CHECK(std::all_of(comp.begin(), comp.end(), [&](std::size_t c) { return c == comp[0]; }));
}

TEST(dfs_estrella_lineal) {
    // 0 → i e i → 0 para todo i: el vértice 0 tiene 200 000 vecinos y se "retoma"
    // 200 000 veces. Un DFS que vuelve a recorrer adj[0] desde el principio en cada
    // retorno cuesta Θ(n²) y no termina; con (vértice, próximo índice) es Θ(n).
    const std::size_t n = 200000;
    Graph g(n);
    for (std::size_t i = 1; i < n; ++i) { g.add_edge(0, i); g.add_edge(i, 0); }
    auto ord = dfs_preorder(g, 0);
    CHECK_EQ(ord.size(), n);
    for (std::size_t i = 0; i < n; ++i) CHECK_EQ(ord[i], i);
    auto comp = strongly_connected_components(g);
    CHECK(std::all_of(comp.begin(), comp.end(), [&](std::size_t c) { return c == comp[0]; }));
}
