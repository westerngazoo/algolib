mod common;
use algolib::lab02::Vector;
use common::Lcg;
use std::rc::Rc;

fn main() {
    common::run(
        "lab02",
        cases![
            vector_push_index,
            vector_slice_api,
            vector_insert_remove,
            vector_reserve_shrink,
            vector_zst,
            vector_sin_fugas,
            vector_clone_con_panic_sin_fugas,
            vector_aleatorio_vs_vec,
        ],
    );
}

fn vector_push_index() {
    let mut v = Vector::new();
    for i in 0..1000i64 { v.push(i * i); }
    assert_eq!(v.len(), 1000);
    for i in 0..1000i64 { assert_eq!(v[i as usize], i * i); }
    assert_eq!(v.first(), Some(&0));
    assert_eq!(v.last(), Some(&(999 * 999)));
    assert_eq!(v.get(1000), None);
    assert_eq!(v.pop(), Some(999 * 999));
}

fn vector_slice_api() {
    // Deref<Target=[T]> nos da sort, iter, contains, binary_search... gratis.
    let mut v: Vector<i32> = [5, 3, 8, 1].into_iter().collect();
    v.sort();
    assert_eq!(&v[..], &[1, 3, 5, 8]);
    assert_eq!(v.iter().sum::<i32>(), 17);
    assert_eq!(v.binary_search(&5), Ok(2));
}

fn vector_insert_remove() {
    let mut v: Vector<String> = ["b", "d"].iter().map(|s| s.to_string()).collect();
    v.insert(0, "a".into());
    v.insert(2, "c".into());
    v.insert(4, "e".into());
    assert_eq!(v.as_slice(), &["a", "b", "c", "d", "e"]);
    assert_eq!(v.remove(0), "a");
    assert_eq!(v.remove(1), "c");
    assert_eq!(v.remove(2), "e");
    assert_eq!(v.as_slice(), &["b", "d"]);
    let r = std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| v.insert(3, "x".into())));
    assert!(r.is_err(), "insert fuera de rango debe hacer panic");
}

fn vector_reserve_shrink() {
    let mut v = Vector::with_capacity(4);
    assert!(v.capacity() >= 4);
    v.reserve(100);
    assert!(v.capacity() >= 100);
    for i in 0..10 { v.push(i); }
    let cap = v.capacity();
    v.reserve(5);
    assert_eq!(v.capacity(), cap);
    v.shrink_to_fit();
    assert_eq!(v.capacity(), 10);
    assert_eq!(v[9], 9);
    v.clear();
    assert!(v.is_empty());
}

fn vector_zst() {
    let mut v = Vector::new();
    for _ in 0..10_000 { v.push(()); }
    assert_eq!(v.len(), 10_000);
    assert_eq!(v.capacity(), usize::MAX);
    v.insert(5, ());
    assert_eq!(v.remove(0), ());
    assert_eq!(v.len(), 10_000);
}

fn vector_sin_fugas() {
    let x = Rc::new(());
    {
        let mut a = Vector::new();
        for _ in 0..100 { a.push(Rc::clone(&x)); }
        a.insert(50, Rc::clone(&x));
        drop(a.remove(0));
        assert_eq!(Rc::strong_count(&x), 101);
        let mut b = a.clone();
        assert_eq!(Rc::strong_count(&x), 201);
        b.clear();
        assert_eq!(Rc::strong_count(&x), 101);
        a.shrink_to_fit();
        a.pop();
        assert_eq!(Rc::strong_count(&x), 100);
    }
    assert_eq!(Rc::strong_count(&x), 1);
}

fn vector_aleatorio_vs_vec() {
    let mut rng = Lcg::new(2);
    let mut v = Vector::new();
    let mut o: Vec<i64> = Vec::new();
    for _ in 0..5000 {
        let r = rng.below(10);
        if r < 4 || o.is_empty() {
            let x = rng.range(-99, 99);
            v.push(x);
            o.push(x);
        } else if r < 6 {
            let i = rng.below(o.len() as u64 + 1) as usize;
            let x = rng.range(-99, 99);
            v.insert(i, x);
            o.insert(i, x);
        } else if r < 8 {
            let i = rng.below(o.len() as u64) as usize;
            assert_eq!(v.remove(i), o.remove(i));
        } else {
            assert_eq!(v.pop(), o.pop());
        }
        assert_eq!(v.len(), o.len());
    }
    assert_eq!(v.as_slice(), o.as_slice());
}

/// Clone que hace panic al llegar a la n-ésima copia.
struct Explota(Rc<()>, std::rc::Rc<std::cell::Cell<i32>>);
impl Clone for Explota {
    fn clone(&self) -> Self {
        let n = self.1.get();
        if n == 0 { panic!("boom"); }
        self.1.set(n - 1);
        Explota(Rc::clone(&self.0), Rc::clone(&self.1))
    }
}

fn vector_clone_con_panic_sin_fugas() {
    let x = Rc::new(());
    let fuse = Rc::new(std::cell::Cell::new(i32::MAX));
    let mut a = Vector::new();
    for _ in 0..20 { a.push(Explota(Rc::clone(&x), Rc::clone(&fuse))); }
    assert_eq!(Rc::strong_count(&x), 21);
    fuse.set(7); // la 8.ª copia hace panic
    let r = std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| a.clone()));
    assert!(r.is_err());
    // Las 7 copias parciales se destruyeron al desenrollar: no hay fugas.
    assert_eq!(Rc::strong_count(&x), 21);
    drop(a);
    assert_eq!(Rc::strong_count(&x), 1);
}
