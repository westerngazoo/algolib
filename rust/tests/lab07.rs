mod common;
use algolib::lab07::AvlMap;
use common::Lcg;
use std::collections::BTreeMap;

fn main() {
    common::run(
        "lab07",
        cases![avl_basico, avl_secuencial_es_perfecto, avl_cota_de_altura_insercion_ordenada, avl_aleatorio_vs_btreemap, avl_borrar_rebalancea],
    );
}

fn cumple_cota_avl(h: i32, n: usize) -> bool {
    (h as f64) < 1.4405 * ((n as f64) + 2.0).log2() - 0.3277 + 1e-9
}

fn avl_basico() {
    let mut m = AvlMap::new();
    assert!(m.is_empty());
    assert_eq!(m.height(), 0);
    assert_eq!(m.insert("b", 2), None);
    assert_eq!(m.insert("a", 1), None);
    assert_eq!(m.insert("c", 3), None);
    assert_eq!(m.insert("a", 10), Some(1));
    assert_eq!(m.get(&"a"), Some(&10));
    *m.get_mut(&"c").unwrap() = 30;
    assert_eq!(m.get(&"c"), Some(&30));
    assert_eq!(m.get(&"z"), None);
    assert_eq!(m.keys(), vec![&"a", &"b", &"c"]);
    assert_eq!((m.min_key(), m.max_key()), (Some(&"a"), Some(&"c")));
    assert_eq!(m.remove(&"b"), Some(2));
    assert_eq!(m.remove(&"b"), None);
    assert_eq!(m.len(), 2);
}

fn avl_secuencial_es_perfecto() {
    let mut m = AvlMap::new();
    for i in 1..=7 { m.insert(i, i); }
    assert_eq!(m.height(), 3);
    let e: AvlMap<i32, i32> = AvlMap::new();
    assert_eq!(e.min_key(), None);
}

fn avl_cota_de_altura_insercion_ordenada() {
    let mut m = AvlMap::new();
    for i in 0..100_000usize {
        m.insert(i, i);
        if i & (i + 1) == 0 { assert!(cumple_cota_avl(m.height(), m.len())); }
    }
    assert!(cumple_cota_avl(m.height(), m.len()));
    for i in (0..100_000).step_by(3) { m.remove(&i); }
    assert!(cumple_cota_avl(m.height(), m.len()));
}

fn avl_aleatorio_vs_btreemap() {
    let mut rng = Lcg::new(12);
    let mut m = AvlMap::new();
    let mut o = BTreeMap::new();
    for op in 0..50_000 {
        let k = rng.range(0, 3000);
        match rng.below(3) {
            0 => { let v = rng.range(0, 99); assert_eq!(m.insert(k, v), o.insert(k, v)); }
            1 => assert_eq!(m.remove(&k), o.remove(&k)),
            _ => assert_eq!(m.get(&k), o.get(&k)),
        }
        assert_eq!(m.len(), o.len());
        if op % 5000 == 0 {
            assert!(m.keys().into_iter().eq(o.keys()));
            assert!(cumple_cota_avl(m.height(), m.len()));
        }
    }
}

/// Borrar casi todo un lado: sin rotaciones al borrar, quedaría la columna derecha
/// del árbol original (altura ~16 con ~500 claves) y violaría la cota.
fn avl_borrar_rebalancea() {
    let mut m = AvlMap::new();
    for i in 0..65_536usize { m.insert(i, i); }
    for i in 0..65_000usize { assert_eq!(m.remove(&i), Some(i)); }
    assert_eq!(m.len(), 536);
    assert!(cumple_cota_avl(m.height(), m.len()));
    let ks: Vec<usize> = m.keys().into_iter().copied().collect();
    assert_eq!(ks, (65_000..65_536).collect::<Vec<_>>());
}
