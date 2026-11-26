mod common;
use algolib::lab04::{HashMap, RingQueue};
use common::Lcg;
use std::collections::{BTreeMap, VecDeque};
use std::hash::{BuildHasher, Hasher};
use std::rc::Rc;

fn main() {
    common::run(
        "lab04",
        cases![
            ring_fifo,
            ring_envuelve_y_crece,
            ring_aleatorio_vs_vecdeque,
            hash_basico,
            hash_factor_de_carga_acotado,
            hash_colisiones_totales,
            hash_aleatorio_vs_btreemap,
            ring_sin_fugas,
            hash_muchos_borrados,
        ],
    );
}

fn ring_fifo() {
    let mut q = RingQueue::new();
    for i in 0..100 { q.push_back(i); }
    for i in 0..100 { assert_eq!(q.pop_front(), Some(i)); }
    assert!(q.is_empty());
    assert_eq!(q.pop_front(), None);
}

fn ring_envuelve_y_crece() {
    let mut q = RingQueue::new();
    let mut n = 0;
    loop {
        q.push_back(n.to_string());
        n += 1;
        if q.len() >= 3 && q.len() == q.capacity() { break; }
    }
    let cap = q.capacity(); // lleno
    q.pop_front();
    q.pop_front();
    for _ in 0..2 { q.push_back(n.to_string()); n += 1; }
    assert_eq!(q.capacity(), cap, "con cupo libre no debe crecer: debe dar la vuelta");
    q.push_back(n.to_string());
    assert!(q.capacity() > cap);
    for i in 0..q.len() { assert_eq!(q.get(i), Some(&(i + 2).to_string())); }
    assert_eq!(q.back(), Some(&n.to_string()));
    assert_eq!(q.get(q.len()), None);
}

fn ring_aleatorio_vs_vecdeque() {
    let mut rng = Lcg::new(4);
    let mut q = RingQueue::new();
    let mut o = VecDeque::new();
    for _ in 0..50000 {
        if o.is_empty() || rng.below(5) < 3 {
            let x = rng.range(0, 1 << 20);
            q.push_back(x);
            o.push_back(x);
        } else {
            assert_eq!(q.pop_front(), o.pop_front());
        }
        assert_eq!(q.len(), o.len());
        assert_eq!(q.front(), o.front());
        assert_eq!(q.back(), o.back());
    }
}

fn hash_basico() {
    let mut m = HashMap::new();
    assert_eq!(m.insert("uno".to_string(), 1), None);
    assert_eq!(m.insert("dos".to_string(), 2), None);
    assert_eq!(m.insert("uno".to_string(), 11), Some(1));
    assert_eq!(m.len(), 2);
    assert_eq!(m.get(&"uno".to_string()), Some(&11));
    *m.get_mut(&"dos".to_string()).unwrap() += 20;
    assert_eq!(m.get(&"dos".to_string()), Some(&22));
    assert_eq!(m.get(&"tres".to_string()), None);
    assert_eq!(m.remove(&"uno".to_string()), Some(11));
    assert_eq!(m.remove(&"uno".to_string()), None);
    assert_eq!(m.len(), 1);
}

fn hash_factor_de_carga_acotado() {
    let mut m = HashMap::new();
    for i in 0..100_000 { m.insert(i, i); }
    assert!(m.load_factor() <= 1.0 && m.load_factor() > 0.0);
    for i in 0..100_000 { assert_eq!(m.get(&i), Some(&i)); }
}

/// Hasher pésimo: todo colisiona.
#[derive(Default, Clone)]
struct ConstantState;
struct ConstantHasher;
impl Hasher for ConstantHasher {
    fn finish(&self) -> u64 { 42 }
    fn write(&mut self, _: &[u8]) {}
}
impl BuildHasher for ConstantState {
    type Hasher = ConstantHasher;
    fn build_hasher(&self) -> ConstantHasher { ConstantHasher }
}

fn hash_colisiones_totales() {
    let mut m = HashMap::with_hasher(ConstantState);
    for i in 0..500 { m.insert(i, -i); }
    for i in (0..500).step_by(2) { assert_eq!(m.remove(&i), Some(-i)); }
    for i in 0..500 {
        if i % 2 == 0 { assert!(!m.contains_key(&i)) } else { assert_eq!(m.get(&i), Some(&-i)) }
    }
    assert_eq!(m.len(), 250);
}

fn hash_aleatorio_vs_btreemap() {
    let mut rng = Lcg::new(5);
    let mut m = HashMap::new();
    let mut o = BTreeMap::new();
    for _ in 0..50000 {
        let k = rng.range(0, 2000);
        match rng.below(3) {
            0 => {
                let v = rng.range(0, 1000);
                assert_eq!(m.insert(k, v), o.insert(k, v));
            }
            1 => assert_eq!(m.remove(&k), o.remove(&k)),
            _ => assert_eq!(m.get(&k), o.get(&k)),
        }
        assert_eq!(m.len(), o.len());
    }
}

fn ring_sin_fugas() {
    let x = Rc::new(());
    {
        let mut q = RingQueue::new();
        for _ in 0..37 { q.push_back(Rc::clone(&x)); }
        for _ in 0..20 { drop(q.pop_front()); }
        for _ in 0..50 { q.push_back(Rc::clone(&x)); } // da la vuelta y crece
        assert_eq!(Rc::strong_count(&x), 68);
    }
    assert_eq!(Rc::strong_count(&x), 1);
}

/// 200 000 claves distintas pasan por la tabla, pero nunca hay más de 64 vivas.
/// Con lápidas que nunca se limpian, las búsquedas fallidas no terminarían.
fn hash_muchos_borrados() {
    let mut m = HashMap::new();
    for i in 0..200_000i64 {
        assert_eq!(m.insert(i, i), None);
        if i >= 64 { assert_eq!(m.remove(&(i - 64)), Some(i - 64)); }
        assert!(!m.contains_key(&(-1 - i)));
    }
    assert_eq!(m.len(), 64);
    for i in 200_000 - 64..200_000 { assert_eq!(m.get(&i), Some(&i)); }
}
