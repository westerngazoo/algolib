mod common;
use algolib::lab08::OrderStatSet;
use common::Lcg;
use std::collections::BTreeSet;

fn main() {
    common::run("lab08", cases![ostat_basico, ostat_aleatorio_vs_btreeset, ostat_select_rank_logaritmicos]);
}

fn ostat_basico() {
    let mut s = OrderStatSet::new();
    for x in [50, 20, 80, 10, 30, 70, 90] { assert!(s.insert(x)); }
    assert!(!s.insert(30));
    assert_eq!(s.len(), 7);
    assert_eq!(s.select(0), Some(&10));
    assert_eq!(s.select(3), Some(&50));
    assert_eq!(s.select(6), Some(&90));
    assert_eq!(s.select(7), None);
    assert_eq!(s.rank(&10), 0);
    assert_eq!(s.rank(&55), 4);
    assert_eq!(s.rank(&100), 7);
    assert!(s.remove(&50));
    assert!(!s.remove(&50));
    assert_eq!(s.select(3), Some(&70));
    assert!(!s.contains(&50));
}

fn ostat_aleatorio_vs_btreeset() {
    let mut rng = Lcg::new(13);
    let mut s = OrderStatSet::new();
    let mut o = BTreeSet::new();
    for op in 0..30_000 {
        let x = rng.range(0, 5000);
        if rng.below(3) != 0 { assert_eq!(s.insert(x), o.insert(x)); } else { assert_eq!(s.remove(&x), o.remove(&x)); }
        assert_eq!(s.len(), o.len());
        if op % 100 == 0 {
            let v: Vec<i64> = o.iter().copied().collect();
            for _ in 0..20 {
                let k = rng.below(v.len() as u64 + 1) as usize;
                assert_eq!(s.select(k), v.get(k));
                let y = rng.range(-1, 5001);
                assert_eq!(s.rank(&y), v.partition_point(|&e| e < y));
            }
        }
    }
}

/// 200 000 claves insertadas EN ORDEN (exige balanceo) y 200 000 consultas de
/// select y de rank. Con select/rank lineales serían ~4·10¹⁰ pasos: no termina.
fn ostat_select_rank_logaritmicos() {
    let n: u64 = 200_000;
    let mut s = OrderStatSet::new();
    for i in 0..n { s.insert(2 * i); }
    let mut rng = Lcg::new(14);
    for _ in 0..n {
        let k = rng.below(n);
        assert_eq!(s.select(k as usize), Some(&(2 * k)));
        assert_eq!(s.rank(&(2 * k + 1)), k as usize + 1);
    }
}
