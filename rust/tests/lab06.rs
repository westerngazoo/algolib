mod common;
use algolib::lab06::BinaryHeap;
use common::Lcg;
use std::cmp::Reverse;

fn main() {
    common::run(
        "lab06",
        cases![heap_min_basico, heap_reverse_es_max, heap_from_vec, heap_aleatorio_vs_std, heap_from_vec_es_lineal],
    );
}

fn heap_min_basico() {
    let mut h = BinaryHeap::new();
    assert!(h.is_empty());
    for x in [5, 1, 8, 3, 9, 2] { h.push(x); }
    assert_eq!(h.len(), 6);
    assert_eq!(h.peek(), Some(&1));
    assert_eq!(h.pop(), Some(1));
    assert_eq!(h.peek(), Some(&2));
    assert_eq!(h.into_sorted_vec(), vec![2, 3, 5, 8, 9]);
    let mut e: BinaryHeap<i32> = BinaryHeap::new();
    assert_eq!(e.pop(), None);
}

fn heap_reverse_es_max() {
    let mut h = BinaryHeap::new();
    for s in ["b", "d", "a", "c"] { h.push(Reverse(s)); }
    assert_eq!(h.peek(), Some(&Reverse("d")));
    let v: Vec<&str> = h.into_sorted_vec().into_iter().map(|Reverse(s)| s).collect();
    assert_eq!(v, vec!["d", "c", "b", "a"]);
}

fn heap_from_vec() {
    let mut rng = Lcg::new(10);
    let mut v: Vec<i64> = (0..50_000).map(|_| rng.range(-100_000, 100_000)).collect();
    let h = BinaryHeap::from_vec(v.clone());
    assert_eq!(h.len(), v.len());
    v.sort();
    assert_eq!(h.into_sorted_vec(), v);
}

fn heap_aleatorio_vs_std() {
    let mut rng = Lcg::new(11);
    let mut h = BinaryHeap::new();
    let mut o = std::collections::BinaryHeap::new();
    for _ in 0..50_000 {
        if o.is_empty() || rng.below(3) != 0 {
            let x = rng.range(0, 500);
            h.push(x);
            o.push(Reverse(x));
        } else {
            assert_eq!(h.pop(), o.pop().map(|Reverse(x)| x));
        }
        assert_eq!(h.len(), o.len());
    }
}

thread_local! { static COMPARACIONES: std::cell::Cell<u64> = const { std::cell::Cell::new(0) }; }

/// Entero cuyo `Ord` cuenta las comparaciones.
#[derive(PartialEq, Eq, Debug)]
struct Contado(i64);
impl PartialOrd for Contado {
    fn partial_cmp(&self, o: &Self) -> Option<std::cmp::Ordering> { Some(self.cmp(o)) }
}
impl Ord for Contado {
    fn cmp(&self, o: &Self) -> std::cmp::Ordering {
        COMPARACIONES.with(|c| c.set(c.get() + 1));
        self.0.cmp(&o.0)
    }
}

/// `from_vec` debe costar O(n): a lo más 2n comparaciones (L06 §4). Con n `push`
/// sobre la entrada descendente costaría ~n lg n ≈ 16n.
fn heap_from_vec_es_lineal() {
    let n: i64 = 1 << 16;
    let v: Vec<Contado> = (0..n).map(|i| Contado(n - i)).collect();
    COMPARACIONES.with(|c| c.set(0));
    let h = BinaryHeap::from_vec(v);
    let c = COMPARACIONES.with(|c| c.get());
    assert!(c <= 2 * n as u64, "heapify hizo {c} comparaciones; se esperaban ≤ {}", 2 * n);
    assert_eq!(h.peek(), Some(&Contado(1)));
}
