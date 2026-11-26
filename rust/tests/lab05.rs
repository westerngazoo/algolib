mod common;
use algolib::lab05::*;
use common::Lcg;
use std::cmp::Ordering;

fn main() {
    common::run(
        "lab05",
        cases![
            insertion_sort_correcto,
            insertion_sort_estable,
            merge_sort_correcto,
            merge_sort_estable,
            quick_sort_correcto,
            heap_sort_correcto,
            quick_sort_adversarial,
            sort_strings,
            counting_y_radix,
        ],
    );
}

/// Ord solo por `key` (ignora `id`) para comprobar estabilidad.
#[derive(Clone, Copy, Debug)]
struct Item { key: i64, id: usize }
impl PartialEq for Item { fn eq(&self, o: &Self) -> bool { self.key == o.key } }
impl Eq for Item {}
impl PartialOrd for Item { fn partial_cmp(&self, o: &Self) -> Option<Ordering> { Some(self.cmp(o)) } }
impl Ord for Item { fn cmp(&self, o: &Self) -> Ordering { self.key.cmp(&o.key) } }

fn casos(n_rand: usize) -> Vec<Vec<i64>> {
    let mut rng = Lcg::new(6);
    let mut cs: Vec<Vec<i64>> = vec![vec![], vec![1], vec![2, 1], vec![1, 2], vec![3, 3, 3], vec![5, -1, 3, -1, 0]];
    for n in [7, 16, 100, n_rand] {
        cs.push((0..n).map(|_| rng.range(-1000, 1000)).collect());
        cs.push((0..n).map(|_| rng.range(0, 3)).collect());
        cs.push((0..n as i64).collect());
        cs.push((0..n as i64).rev().collect());
    }
    cs
}

fn probar(sort: fn(&mut [i64]), n_rand: usize) {
    for mut c in casos(n_rand) {
        let mut e = c.clone();
        e.sort();
        sort(&mut c);
        assert_eq!(c, e);
    }
}

fn probar_estable(sort: fn(&mut [Item])) {
    let mut rng = Lcg::new(8);
    let mut v: Vec<Item> = (0..2000).map(|id| Item { key: rng.range(0, 20), id }).collect();
    let mut e = v.clone();
    e.sort(); // estable en std
    sort(&mut v);
    let a: Vec<_> = v.iter().map(|x| (x.key, x.id)).collect();
    let b: Vec<_> = e.iter().map(|x| (x.key, x.id)).collect();
    assert_eq!(a, b);
}

fn insertion_sort_correcto() { probar(insertion_sort, 1000) }
fn insertion_sort_estable() { probar_estable(insertion_sort) }
fn merge_sort_correcto() { probar(merge_sort, 100_000) }
fn merge_sort_estable() { probar_estable(merge_sort) }
fn quick_sort_correcto() { probar(quick_sort, 100_000) }
fn heap_sort_correcto() { probar(heap_sort, 100_000) }

fn quick_sort_adversarial() {
    let mut asc: Vec<i64> = (0..200_000).collect();
    let e = asc.clone();
    quick_sort(&mut asc);
    assert_eq!(asc, e);
    let mut iguales = vec![7; 200_000];
    quick_sort(&mut iguales);
    assert!(iguales.iter().all(|&x| x == 7));
}

fn sort_strings() {
    let v: Vec<String> = ["pera", "manzana", "uva", "kiwi", "fresa", "banana"].iter().map(|s| s.to_string()).collect();
    let mut e = v.clone();
    e.sort();
    let (mut a, mut b, mut c) = (v.clone(), v.clone(), v.clone());
    merge_sort(&mut a);
    quick_sort(&mut b);
    heap_sort(&mut c);
    assert_eq!(a, e);
    assert_eq!(b, e);
    assert_eq!(c, e);
}

fn counting_y_radix() {
    let mut rng = Lcg::new(9);
    let mut a: Vec<u32> = (0..100_000).map(|_| rng.below(1000) as u32).collect();
    let mut b: Vec<u32> = (0..100_000).map(|_| (rng.next() as u32) ^ ((rng.next() as u32) << 31)).collect();
    let (mut ea, mut eb) = (a.clone(), b.clone());
    ea.sort();
    eb.sort();
    counting_sort(&mut a, 999);
    radix_sort(&mut b);
    assert_eq!(a, ea);
    assert_eq!(b, eb);
    let mut vacio: Vec<u32> = vec![];
    counting_sort(&mut vacio, 0);
    radix_sort(&mut vacio);
    assert!(vacio.is_empty());
}
