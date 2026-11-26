#include <algolib/lab09/graph.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

/* Oráculo: alcanzabilidad por BFS desde cada vértice (matriz n×n, escrita en el test). */
static bool *cierre_transitivo(size_t n, const size_t (*es)[2], size_t m) {
    bool *r = calloc(n * n, sizeof *r);
    size_t *cola = malloc(n * sizeof *cola);
    for (size_t s = 0; s < n; s++) {
        size_t ini = 0, fin = 0;
        cola[fin++] = s;
        r[s * n + s] = true;
        while (ini < fin) {
            size_t u = cola[ini++];
            for (size_t e = 0; e < m; e++)
                if (es[e][0] == u && !r[s * n + es[e][1]]) {
                    r[s * n + es[e][1]] = true;
                    cola[fin++] = es[e][1];
                }
        }
    }
    free(cola);
    return r;
}

static bool es_orden_topologico(const Graph *g, const size_t *ord) {
    size_t n = graph_num_vertices(g);
    size_t *pos = malloc(n * sizeof *pos);
    bool ok = true;
    for (size_t i = 0; i < n; i++) pos[i] = n;
    for (size_t i = 0; i < n && ok; i++) {
        if (ord[i] >= n || pos[ord[i]] != n) ok = false;   /* no es permutación */
        else pos[ord[i]] = i;
    }
    for (size_t u = 0; u < n && ok; u++) {
        size_t c;
        const size_t *vs = graph_neighbors(g, u, &c);
        for (size_t j = 0; j < c; j++)
            if (pos[u] >= pos[vs[j]]) ok = false;
    }
    free(pos);
    return ok;
}

static void grafo_representacion(void) {
    Graph *g = graph_new(3);
    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 0, 2);
    CHECK_EQ(graph_num_vertices(g), 3);
    size_t c;
    const size_t *vs = graph_neighbors(g, 0, &c);
    CHECK_EQ(c, 2);
    CHECK(vs[0] == 1 && vs[1] == 2);
    graph_neighbors(g, 2, &c);
    CHECK_EQ(c, 0);
    graph_free(g);
    graph_free(NULL);
}

static void bfs_distancias(void) {
    Graph *g = graph_new(5);
    size_t es[][2] = {{0, 1}, {1, 2}, {2, 3}, {0, 3}};
    for (int i = 0; i < 4; i++) graph_add_edge(g, es[i][0], es[i][1]);
    size_t d[5];
    graph_bfs(g, 0, d);
    CHECK_EQ(d[0], 0); CHECK_EQ(d[1], 1); CHECK_EQ(d[2], 2); CHECK_EQ(d[3], 1);
    CHECK(d[4] == GRAPH_INF);
    graph_free(g);
}

static void dfs_preorden_semantica_recursiva(void) {
    Graph *g = graph_new(6);
    size_t es[][2] = {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}, {2, 5}, {4, 2}};
    for (int i = 0; i < 7; i++) graph_add_edge(g, es[i][0], es[i][1]);
    size_t out[6], e[] = {0, 1, 3, 4, 2, 5};
    CHECK_EQ(graph_dfs_preorder(g, 0, out), 6);
    CHECK(memcmp(out, e, sizeof e) == 0);   /* 0 → 1 → 3 → 4 → 2 → (3 visto) → 5 */
    CHECK_EQ(graph_dfs_preorder(g, 5, out), 1);
    CHECK_EQ(out[0], 5);
    graph_free(g);
}

static void topo_dag_aleatorio_y_ciclo(void) {
    Lcg rng = {14};
    size_t n = 300, perm[300], ord[300];
    for (size_t i = 0; i < n; i++) perm[i] = i;
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = lcg_below(&rng, i + 1), t = perm[i];
        perm[i] = perm[j];
        perm[j] = t;
    }
    Graph *g = graph_new(n);
    for (int e = 0; e < 2000; e++) {
        size_t a = lcg_below(&rng, n), b = lcg_below(&rng, n);
        if (a == b) continue;
        if (a > b) { size_t t = a; a = b; b = t; }
        graph_add_edge(g, perm[a], perm[b]);     /* siempre "hacia adelante" en perm → DAG */
    }
    CHECK(graph_topological_sort(g, ord));
    CHECK(es_orden_topologico(g, ord));
    graph_free(g);

    Graph *c = graph_new(3);
    graph_add_edge(c, 0, 1); graph_add_edge(c, 1, 2); graph_add_edge(c, 2, 0);
    CHECK(!graph_topological_sort(c, ord));
    graph_free(c);
    Graph *lazo = graph_new(1);
    graph_add_edge(lazo, 0, 0);
    CHECK(!graph_topological_sort(lazo, ord));
    graph_free(lazo);
}

static void scc_vs_cierre_transitivo(void) {
    Lcg rng = {15};
    for (int trial = 0; trial < 5; trial++) {
        size_t n = 120, m = 200, es[200][2], comp[120];
        Graph *g = graph_new(n);
        for (size_t e = 0; e < m; e++) {
            es[e][0] = lcg_below(&rng, n);
            es[e][1] = lcg_below(&rng, n);
            graph_add_edge(g, es[e][0], es[e][1]);
        }
        graph_scc(g, comp);
        bool *r = cierre_transitivo(n, (const size_t (*)[2])es, m);
        for (size_t u = 0; u < n; u++)
            for (size_t v = 0; v < n; v++)
                CHECK_EQ(comp[u] == comp[v], r[u * n + v] && r[v * n + u]);
        free(r);
        graph_free(g);
    }
}

static void recorridos_iterativos_200k(void) {
    size_t n = 200000;
    size_t *buf = malloc(n * sizeof *buf);
    Graph *g = graph_new(n);
    for (size_t i = 0; i + 1 < n; i++) graph_add_edge(g, i, i + 1);
    CHECK_EQ(graph_dfs_preorder(g, 0, buf), n);
    graph_bfs(g, 0, buf);
    CHECK_EQ(buf[n - 1], n - 1);
    CHECK(graph_topological_sort(g, buf));
    CHECK(es_orden_topologico(g, buf));
    graph_add_edge(g, n - 1, 0);                 /* ahora es un gran ciclo: una sola SCC */
    graph_scc(g, buf);
    for (size_t i = 0; i < n; i++) CHECK(buf[i] == buf[0]);
    graph_free(g);
    free(buf);
}

static void dfs_estrella_lineal(void) {
    /* 0 → i e i → 0 para todo i: el vértice 0 tiene 200 000 vecinos y se "retoma"
     * 200 000 veces. Un DFS que vuelve a recorrer adj[0] desde el principio en cada
     * retorno cuesta Θ(n²) y no termina; con (vértice, próximo índice) es Θ(n). */
    enum { N = 200000 };
    Graph *g = graph_new(N);
    for (size_t i = 1; i < N; i++) {
        graph_add_edge(g, 0, i);
        graph_add_edge(g, i, 0);
    }
    size_t *ord = malloc(N * sizeof *ord), *comp = malloc(N * sizeof *comp);
    CHECK_EQ(graph_dfs_preorder(g, 0, ord), N);
    for (size_t i = 0; i < N; i++) CHECK_EQ(ord[i], i);
    graph_scc(g, comp);
    for (size_t i = 0; i < N; i++) CHECK_EQ(comp[i], comp[0]);
    free(comp);
    free(ord);
    graph_free(g);
}

int main(void) {
    return algotest_run("lab09", (AlgoTest[]){
        T(grafo_representacion), T(bfs_distancias), T(dfs_preorden_semantica_recursiva),
        T(topo_dag_aleatorio_y_ciclo), T(scc_vs_cierre_transitivo), T(recorridos_iterativos_200k), T(dfs_estrella_lineal), {0}});
}
