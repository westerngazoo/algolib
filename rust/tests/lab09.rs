mod common;
use algolib::lab09::*;
use common::Lcg;
use std::collections::VecDeque;

fn main() {
    common::run(
        "lab09",
        cases![
            grafo_representacion,
            bfs_distancias,
            dfs_preorden_semantica_recursiva,
            topo_dag_aleatorio_y_ciclo,
            scc_vs_cierre_transitivo,
            recorridos_iterativos_200k,
            dfs_estrella_lineal,
        ],
    );
}

fn cierre_transitivo(n: usize, es: &[(usize, usize)]) -> Vec<Vec<bool>> {
    let mut adj = vec![vec![]; n];
    for &(u, v) in es { adj[u].push(v); }
    let mut r = vec![vec![false; n]; n];
    for s in 0..n {
        let mut q = VecDeque::from([s]);
        r[s][s] = true;
        while let Some(u) = q.pop_front() {
            for &v in &adj[u] {
                if !r[s][v] { r[s][v] = true; q.push_back(v); }
            }
        }
    }
    r
}

fn es_orden_topologico(g: &Graph, ord: &[usize]) -> bool {
    let n = g.num_vertices();
    if ord.len() != n { return false; }
    let mut pos = vec![n; n];
    for (i, &v) in ord.iter().enumerate() {
        if v >= n || pos[v] != n { return false; }
        pos[v] = i;
    }
    (0..n).all(|u| g.neighbors(u).iter().all(|&v| pos[u] < pos[v]))
}

fn grafo_representacion() {
    let mut g = Graph::new(3);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    assert_eq!(g.num_vertices(), 3);
    assert_eq!(g.neighbors(0), &[1, 2]);
    assert!(g.neighbors(2).is_empty());
}

fn bfs_distancias() {
    let mut g = Graph::new(5);
    for (u, v) in [(0, 1), (1, 2), (2, 3), (0, 3)] { g.add_edge(u, v); }
    assert_eq!(bfs_distances(&g, 0), vec![Some(0), Some(1), Some(2), Some(1), None]);
}

fn dfs_preorden_semantica_recursiva() {
    let mut g = Graph::new(6);
    for (u, v) in [(0, 1), (0, 2), (1, 3), (2, 3), (3, 4), (2, 5), (4, 2)] { g.add_edge(u, v); }
    assert_eq!(dfs_preorder(&g, 0), vec![0, 1, 3, 4, 2, 5]);
    assert_eq!(dfs_preorder(&g, 5), vec![5]);
}

fn topo_dag_aleatorio_y_ciclo() {
    let mut rng = Lcg::new(14);
    let n = 300;
    let mut perm: Vec<usize> = (0..n).collect();
    for i in (1..n).rev() { perm.swap(i, rng.below(i as u64 + 1) as usize); }
    let mut g = Graph::new(n);
    for _ in 0..2000 {
        let (mut a, mut b) = (rng.below(n as u64) as usize, rng.below(n as u64) as usize);
        if a == b { continue; }
        if a > b { std::mem::swap(&mut a, &mut b); }
        g.add_edge(perm[a], perm[b]);
    }
    let ord = topological_sort(&g).expect("es un DAG");
    assert!(es_orden_topologico(&g, &ord));

    let mut c = Graph::new(3);
    for (u, v) in [(0, 1), (1, 2), (2, 0)] { c.add_edge(u, v); }
    assert_eq!(topological_sort(&c), None);
    let mut lazo = Graph::new(1);
    lazo.add_edge(0, 0);
    assert_eq!(topological_sort(&lazo), None);
}

fn scc_vs_cierre_transitivo() {
    let mut rng = Lcg::new(15);
    for _ in 0..5 {
        let n = 120;
        let mut g = Graph::new(n);
        let mut es = vec![];
        for _ in 0..200 {
            let (u, v) = (rng.below(n as u64) as usize, rng.below(n as u64) as usize);
            es.push((u, v));
            g.add_edge(u, v);
        }
        let comp = strongly_connected_components(&g);
        assert_eq!(comp.len(), n);
        let r = cierre_transitivo(n, &es);
        for u in 0..n {
            for v in 0..n {
                assert_eq!(comp[u] == comp[v], r[u][v] && r[v][u], "u={u} v={v}");
            }
        }
    }
}

fn recorridos_iterativos_200k() {
    let n = 200_000;
    let mut path = Graph::new(n);
    for i in 0..n - 1 { path.add_edge(i, i + 1); }
    assert_eq!(dfs_preorder(&path, 0).len(), n);
    assert_eq!(bfs_distances(&path, 0)[n - 1], Some(n - 1));
    let ord = topological_sort(&path).expect("camino es DAG");
    assert!(es_orden_topologico(&path, &ord));
    path.add_edge(n - 1, 0);
    let comp = strongly_connected_components(&path);
    assert!(comp.iter().all(|&c| c == comp[0]));
}

/// 0 → i e i → 0 para todo i: el vértice 0 tiene 200 000 vecinos y se "retoma"
/// 200 000 veces. Un DFS que vuelve a recorrer adj[0] desde el principio en cada
/// retorno cuesta Θ(n²) y no termina; con (vértice, próximo índice) es Θ(n).
fn dfs_estrella_lineal() {
    let n = 200_000;
    let mut g = Graph::new(n);
    for i in 1..n { g.add_edge(0, i); g.add_edge(i, 0); }
    let ord = dfs_preorder(&g, 0);
    assert_eq!(ord, (0..n).collect::<Vec<_>>());
    let comp = strongly_connected_components(&g);
    assert!(comp.iter().all(|&c| c == comp[0]));
}
