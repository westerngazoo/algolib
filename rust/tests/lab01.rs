mod common;
use algolib::lab01::{ArrayStack, MinStack};
use common::Lcg;
use std::rc::Rc;

fn main() {
    common::run(
        "lab01",
        cases![
            stack_lifo,
            stack_vacia_es_none,
            stack_crecimiento_geometrico,
            stack_peek_mut_e_iter,
            stack_clone_y_drop_sin_fugas,
            stack_aleatorio_vs_vec,
            minstack_basico,
            minstack_aleatorio,
        ],
    );
}

fn stack_lifo() {
    let mut s = ArrayStack::new();
    assert!(s.is_empty());
    for i in 0..100 { s.push(i); }
    assert_eq!(s.len(), 100);
    for i in (0..100).rev() {
        assert_eq!(s.peek(), Some(&i));
        assert_eq!(s.pop(), Some(i));
    }
    assert!(s.is_empty());
}

fn stack_vacia_es_none() {
    let mut s: ArrayStack<i32> = ArrayStack::new();
    assert_eq!(s.pop(), None);
    assert_eq!(s.peek(), None);
    assert!(s.peek_mut().is_none());
}

fn stack_crecimiento_geometrico() {
    let mut s = ArrayStack::new();
    let (mut prev, mut cambios) = (s.capacity(), 0);
    for i in 0..1024 {
        s.push(i);
        assert!(s.len() <= s.capacity());
        if s.capacity() != prev {
            assert!(if prev == 0 { s.capacity() == 1 } else { s.capacity() == 2 * prev });
            cambios += 1;
            prev = s.capacity();
        }
    }
    assert_eq!(cambios, 11);
}

fn stack_peek_mut_e_iter() {
    let mut s = ArrayStack::new();
    s.push(1);
    s.push(2);
    s.push(3);
    *s.peek_mut().unwrap() = 30;
    assert_eq!(s.iter().copied().collect::<Vec<_>>(), vec![30, 2, 1]);
}

fn stack_clone_y_drop_sin_fugas() {
    // Rc cuenta referencias: si la pila no suelta bien sus elementos, el conteo lo delata.
    let x = Rc::new(());
    {
        let mut s = ArrayStack::new();
        for _ in 0..50 { s.push(Rc::clone(&x)); }
        let t = s.clone();
        assert_eq!(Rc::strong_count(&x), 101);
        drop(t);
        s.pop();
        assert_eq!(Rc::strong_count(&x), 50);
        s.clear();
        assert!(s.is_empty());
        assert_eq!(Rc::strong_count(&x), 1);
        s.push(Rc::clone(&x));
    }
    assert_eq!(Rc::strong_count(&x), 1);
}

fn stack_aleatorio_vs_vec() {
    let mut rng = Lcg::new(1);
    let mut s = ArrayStack::new();
    let mut oracle = Vec::new();
    for _ in 0..20000 {
        if oracle.is_empty() || rng.below(3) != 0 {
            let x = rng.range(-1000, 1000);
            s.push(x);
            oracle.push(x);
        } else {
            assert_eq!(s.pop(), oracle.pop());
        }
        assert_eq!(s.len(), oracle.len());
    }
}

fn minstack_basico() {
    let mut m = MinStack::new();
    assert!(m.is_empty());
    for x in [5, 3, 7, 3, 1] { m.push(x); }
    assert_eq!(m.min(), Some(&1));
    m.pop();
    assert_eq!(m.min(), Some(&3));
    m.pop();
    assert_eq!(m.min(), Some(&3)); // el 3 duplicado
    m.pop();
    assert_eq!(m.min(), Some(&3));
    m.pop();
    assert_eq!(m.min(), Some(&5));
    assert_eq!(m.peek(), Some(&5));
    assert_eq!(m.pop(), Some(5));
    assert_eq!(m.min(), None);
}

fn minstack_aleatorio() {
    let mut rng = Lcg::new(7);
    let mut m = MinStack::new();
    let mut oracle: Vec<i64> = Vec::new();
    for _ in 0..20000 {
        if oracle.is_empty() || rng.below(2) == 0 {
            let x = rng.range(-50, 50);
            m.push(x);
            oracle.push(x);
        } else {
            assert_eq!(m.pop(), oracle.pop());
        }
        assert_eq!(m.len(), oracle.len());
        assert_eq!(m.min(), oracle.iter().min());
    }
}
