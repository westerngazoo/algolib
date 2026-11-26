mod common;
use algolib::lab13::*;
use common::Lcg;
use std::collections::BTreeSet;

fn main() {
    common::run(
        "lab13",
        cases![trie_basico, trie_remove_poda, trie_aleatorio_vs_btreeset, fenwick_vs_ingenuo, segtree_vs_ingenuo, trie_conteo_es_o_l, rangos_grandes],
    );
}

fn trie_basico() {
    let mut t = Trie::new();
    assert_eq!(t.node_count(), 1);
    for w in ["casa", "cama", "camion", "cas", "perro"] { assert!(t.insert(w)); }
    assert!(!t.insert("casa"));
    assert_eq!(t.len(), 5);
    assert!(t.contains("cas"));
    assert!(!t.contains("ca"));
    assert!(t.starts_with("ca"));
    assert!(!t.starts_with("gato"));
    assert!(t.starts_with(""));
    assert_eq!(t.count_with_prefix("ca"), 4);
    assert_eq!(t.count_with_prefix("cam"), 2);
    assert_eq!(t.count_with_prefix(""), 5);
    assert_eq!(t.count_with_prefix("x"), 0);
}

fn trie_remove_poda() {
    let mut t = Trie::new();
    t.insert("abc");
    assert_eq!(t.node_count(), 4);
    t.insert("abcde");
    assert_eq!(t.node_count(), 6);
    assert!(t.remove("abcde"));
    assert_eq!(t.node_count(), 4);
    assert!(!t.remove("ab"));
    assert!(t.contains("abc"));
    assert!(t.remove("abc"));
    assert_eq!(t.node_count(), 1);
    assert_eq!(t.len(), 0);
}

fn trie_aleatorio_vs_btreeset() {
    let mut rng = Lcg::new(23);
    let mut t = Trie::new();
    let mut o = BTreeSet::new();
    for op in 0..20_000 {
        let w: String = (0..1 + rng.below(6)).map(|_| (b'a' + rng.below(4) as u8) as char).collect();
        if rng.below(3) != 0 { assert_eq!(t.insert(&w), o.insert(w.clone())); } else { assert_eq!(t.remove(&w), o.remove(&w)); }
        if op % 200 == 0 {
            let p = &w[..rng.below(w.len() as u64 + 1) as usize];
            assert_eq!(t.count_with_prefix(p), o.iter().filter(|s| s.starts_with(p)).count());
            assert_eq!(t.len(), o.len());
        }
    }
}

fn fenwick_vs_ingenuo() {
    let mut rng = Lcg::new(24);
    let n = 1000;
    let mut f = FenwickTree::new(n);
    let mut a = vec![0i64; n];
    assert_eq!(f.prefix_sum(0), 0);
    for _ in 0..20_000 {
        if rng.below(2) == 1 {
            let (i, d) = (rng.below(n as u64) as usize, rng.range(-1000, 1000));
            f.add(i, d);
            a[i] += d;
        } else {
            let (mut l, mut r) = (rng.below(n as u64 + 1) as usize, rng.below(n as u64 + 1) as usize);
            if l > r { std::mem::swap(&mut l, &mut r); }
            assert_eq!(f.range_sum(l, r), a[l..r].iter().sum::<i64>());
        }
    }
}

fn segtree_vs_ingenuo() {
    let mut rng = Lcg::new(25);
    let n = 777;
    let mut a: Vec<i64> = (0..n).map(|_| rng.range(-1_000_000, 1_000_000)).collect();
    let mut st = SegmentTree::new(&a);
    for _ in 0..20_000 {
        if rng.below(2) == 1 {
            let (i, x) = (rng.below(n as u64) as usize, rng.range(-1_000_000, 1_000_000));
            st.set(i, x);
            a[i] = x;
        } else {
            let (mut l, mut r) = (rng.below(n as u64 + 1) as usize, rng.below(n as u64 + 1) as usize);
            if l > r { std::mem::swap(&mut l, &mut r); }
            assert_eq!(st.range_min(l, r), a[l..r].iter().copied().min());
        }
    }
    assert_eq!(SegmentTree::new(&[]).range_min(0, 0), None);
}

/// "a" seguida de i en base 26 (letras a..z): una palabra distinta para cada i.
fn palabra(mut i: usize) -> String {
    let mut w = String::from("a");
    loop {
        w.push((b'a' + (i % 26) as u8) as char);
        i /= 26;
        if i == 0 { break w; }
    }
}

/// 100 000 palabras con el prefijo "a" y 100 000 consultas de ese prefijo.
/// Contar recorriendo el subárbol (Θ(nodos) por consulta) no termina a tiempo.
fn trie_conteo_es_o_l() {
    let mut t = Trie::new();
    let n = 100_000;
    for i in 0..n { assert!(t.insert(&palabra(i))); }
    for _ in 0..n { assert_eq!(t.count_with_prefix("a"), n); }
    assert!(t.contains(&palabra(42)));
    assert_eq!(t.count_with_prefix("b"), 0);
}

/// n = 2²⁰ y 200 000 consultas de cada tipo: una suma o un mínimo lineales por
/// consulta harían ~10¹¹ pasos.
fn rangos_grandes() {
    let n = 1usize << 20;
    let mut rng = Lcg::new(26);
    let mut f = FenwickTree::new(n);
    for i in 0..n { f.add(i, 1); }
    let a: Vec<i64> = (0..n).map(|i| (n - i) as i64).collect();
    let st = SegmentTree::new(&a);
    for _ in 0..200_000 {
        let (mut l, mut r) = (rng.below(n as u64) as usize, rng.below(n as u64 + 1) as usize);
        if l > r { std::mem::swap(&mut l, &mut r); }
        assert_eq!(f.range_sum(l, r), (r - l) as i64);
        if l < r { assert_eq!(st.range_min(l, r), Some((n - (r - 1)) as i64)); }
    }
}
