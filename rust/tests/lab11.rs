mod common;
use algolib::lab11::*;
use common::Lcg;

fn main() {
    common::run(
        "lab11",
        cases![dsu_basico, dsu_grande_y_profundo, mst_ejemplo_clrs_23_1, mst_no_conexo, mst_aleatorio_vs_fuerza_bruta, mst_grande_kruskal_igual_prim],
    );
}

fn e(u: usize, v: usize, w: i64) -> Edge { Edge { u, v, w } }

fn mst_fuerza_bruta(n: usize, es: &[Edge]) -> Option<i64> {
    if n <= 1 { return Some(0); }
    let m = es.len();
    let mut best: Option<i64> = None;
    for mask in 0u32..(1 << m) {
        if mask.count_ones() as usize != n - 1 { continue; }
        let mut p: Vec<usize> = (0..n).collect();
        fn f(p: &[usize], mut x: usize) -> usize { while p[x] != x { x = p[x]; } x }
        let (mut w, mut acyclic) = (0, true);
        for i in 0..m {
            if mask >> i & 1 == 0 { continue; }
            let (a, b) = (f(&p, es[i].u), f(&p, es[i].v));
            if a == b { acyclic = false; break; }
            p[a] = b;
            w += es[i].w;
        }
        if acyclic && best.map_or(true, |b| w < b) { best = Some(w); }
    }
    best
}

fn dsu_basico() {
    let mut d = DisjointSet::new(6);
    assert_eq!(d.count(), 6);
    assert!(d.unite(0, 1));
    assert!(d.unite(2, 3));
    assert!(!d.unite(1, 0));
    assert!(d.unite(1, 3));
    assert!(d.same(0, 2));
    assert!(!d.same(0, 4));
    assert_eq!(d.count(), 3);
    assert_eq!(d.size_of(3), 4);
    assert_eq!(d.size_of(5), 1);
}

fn dsu_grande_y_profundo() {
    let n = 1_000_000;
    let mut d = DisjointSet::new(n);
    for i in 1..n { d.unite(i - 1, i); }
    assert_eq!(d.count(), 1);
    for i in (0..n).step_by(997) { assert!(d.same(0, i)); }
    assert_eq!(d.size_of(n / 2), n);
}

fn mst_ejemplo_clrs_23_1() {
    let es = [e(0, 1, 4), e(0, 7, 8), e(1, 2, 8), e(1, 7, 11), e(2, 3, 7), e(2, 8, 2), e(2, 5, 4),
              e(3, 4, 9), e(3, 5, 14), e(4, 5, 10), e(5, 6, 2), e(6, 7, 1), e(6, 8, 6), e(7, 8, 7)];
    assert_eq!(kruskal(9, &es), Some(37));
    assert_eq!(prim(9, &es), Some(37));
}

fn mst_no_conexo() {
    let es = [e(0, 1, 1), e(2, 3, 1)];
    assert_eq!(kruskal(4, &es), None);
    assert_eq!(prim(4, &es), None);
    assert_eq!(kruskal(1, &[]), Some(0));
    assert_eq!(prim(1, &[]), Some(0));
}

fn mst_aleatorio_vs_fuerza_bruta() {
    let mut rng = Lcg::new(19);
    for _ in 0..60 {
        let n = 2 + rng.below(5) as usize;
        let m = 1 + rng.below(11) as usize;
        let es: Vec<Edge> = (0..m)
            .map(|_| e(rng.below(n as u64) as usize, rng.below(n as u64) as usize, rng.range(-5, 20)))
            .collect();
        let o = mst_fuerza_bruta(n, &es);
        assert_eq!(kruskal(n, &es), o);
        assert_eq!(prim(n, &es), o);
    }
}

/// 200 000 vértices, ~10⁶ aristas (con un camino que garantiza conexidad).
/// Kruskal y Prim deben coincidir; un Prim Θ(V²) no termina a tiempo.
fn mst_grande_kruskal_igual_prim() {
    let n = 200_000usize;
    let mut rng = Lcg::new(20);
    let mut es: Vec<Edge> = (0..n - 1).map(|i| e(i, i + 1, 1_000_000 + rng.range(0, 1000))).collect();
    for _ in 0..800_000 {
        let (u, v) = (rng.below(n as u64) as usize, rng.below(n as u64) as usize);
        es.push(e(u, v, rng.range(-1000, 1_000_000)));
    }
    let k = kruskal(n, &es);
    assert!(k.is_some());
    assert_eq!(k, prim(n, &es));
}
