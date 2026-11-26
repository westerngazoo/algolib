mod common;
use algolib::lab10::*;
use common::Lcg;

fn main() {
    common::run(
        "lab10",
        cases![
            dijkstra_clrs_24_6,
            dijkstra_inalcanzable,
            dijkstra_aleatorio_vs_oraculo,
            bellman_ford_pesos_negativos,
            bellman_ford_ciclo_negativo,
            dag_aleatorio_con_negativos,
            floyd_warshall_vs_oraculo,
        ],
    );
}

type Edges = Vec<(usize, usize, i64)>;

fn construir(n: usize, es: &[(usize, usize, i64)]) -> WeightedGraph {
    let mut g = WeightedGraph::new(n);
    for &(u, v, w) in es { g.add_edge(u, v, w); }
    g
}

fn oraculo(n: usize, es: &[(usize, usize, i64)], s: usize) -> Vec<Dist> {
    let mut d: Vec<Dist> = vec![None; n];
    d[s] = Some(0);
    for _ in 1..n {
        for &(u, v, w) in es {
            if let Some(du) = d[u] {
                if d[v].map_or(true, |dv| du + w < dv) { d[v] = Some(du + w); }
            }
        }
    }
    d
}

fn aleatorio(rng: &mut Lcg, n: usize, m: usize, wmin: i64, dag: bool) -> Edges {
    let mut es = vec![];
    for _ in 0..m {
        let (mut u, mut v) = (rng.below(n as u64) as usize, rng.below(n as u64) as usize);
        if dag {
            if u == v { continue; }
            if u > v { std::mem::swap(&mut u, &mut v); }
        }
        es.push((u, v, rng.range(wmin, 100)));
    }
    es
}

fn dijkstra_clrs_24_6() {
    let es = [(0, 1, 10), (0, 3, 5), (1, 2, 1), (1, 3, 2), (2, 4, 4),
              (3, 1, 3), (3, 2, 9), (3, 4, 2), (4, 0, 7), (4, 2, 6)];
    assert_eq!(dijkstra(&construir(5, &es), 0), vec![Some(0), Some(8), Some(9), Some(5), Some(7)]);
}

fn dijkstra_inalcanzable() {
    assert_eq!(dijkstra(&construir(3, &[(0, 1, 4)]), 0), vec![Some(0), Some(4), None]);
}

fn dijkstra_aleatorio_vs_oraculo() {
    let mut rng = Lcg::new(16);
    for _ in 0..20 {
        let es = aleatorio(&mut rng, 60, 300, 0, false);
        assert_eq!(dijkstra(&construir(60, &es), 0), oraculo(60, &es, 0));
    }
}

fn bellman_ford_pesos_negativos() {
    let d = bellman_ford(&construir(3, &[(0, 1, 4), (0, 2, 5), (2, 1, -3)]), 0);
    assert_eq!(d, Some(vec![Some(0), Some(2), Some(5)]));
}

fn bellman_ford_ciclo_negativo() {
    assert_eq!(bellman_ford(&construir(3, &[(0, 1, 1), (1, 2, -1), (2, 1, -1)]), 0), None);
    let d = bellman_ford(&construir(3, &[(1, 2, -1), (2, 1, -1)]), 0);
    assert_eq!(d, Some(vec![Some(0), None, None]));
}

fn dag_aleatorio_con_negativos() {
    let mut rng = Lcg::new(17);
    for _ in 0..20 {
        let es = aleatorio(&mut rng, 80, 400, -50, true);
        let g = construir(80, &es);
        let o = oraculo(80, &es, 0);
        assert_eq!(dag_shortest_paths(&g, 0), o);
        assert_eq!(bellman_ford(&g, 0), Some(o));
    }
}

fn floyd_warshall_vs_oraculo() {
    let mut rng = Lcg::new(18);
    for dag in [false, true] {
        let es = aleatorio(&mut rng, 40, 200, if dag { -50 } else { 0 }, dag);
        let all = floyd_warshall(&construir(40, &es));
        assert_eq!(all.len(), 40);
        for s in 0..40 { assert_eq!(all[s], oraculo(40, &es, s)); }
    }
}
