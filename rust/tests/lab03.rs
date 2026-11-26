mod common;
use algolib::lab03::{DList, LinkedStack};
use common::Lcg;
use std::collections::VecDeque;
use std::rc::Rc;

fn main() {
    common::run(
        "lab03",
        cases![
            lstack_lifo,
            lstack_reverse_e_iter,
            lstack_drop_iterativo_un_millon,
            lstack_sin_fugas,
            dlist_deque_basico,
            dlist_sin_fugas,
            dlist_aleatorio_vs_vecdeque,
        ],
    );
}

fn lstack_lifo() {
    let mut s = LinkedStack::new();
    assert!(s.is_empty());
    for i in 0..5 { s.push(i); }
    assert_eq!(s.len(), 5);
    assert_eq!(s.iter().copied().collect::<Vec<_>>(), vec![4, 3, 2, 1, 0]);
    assert_eq!(s.pop(), Some(4));
    *s.peek_mut().unwrap() *= 10;
    assert_eq!(s.peek(), Some(&30));
    let mut e: LinkedStack<i32> = LinkedStack::new();
    assert_eq!(e.pop(), None);
}

fn lstack_reverse_e_iter() {
    let mut s: LinkedStack<String> = LinkedStack::new();
    s.reverse();
    assert!(s.is_empty());
    for x in ["c", "b", "a"] { s.push(x.to_string()); }
    s.reverse();
    assert_eq!(s.iter().cloned().collect::<Vec<_>>(), vec!["c", "b", "a"]);
    s.push("d".into());
    assert_eq!(s.len(), 4);
}

fn lstack_drop_iterativo_un_millon() {
    let mut s = LinkedStack::new();
    for i in 0..1_000_000 { s.push(i); }
    assert_eq!(s.len(), 1_000_000);
}

fn lstack_sin_fugas() {
    let x = Rc::new(());
    {
        let mut s = LinkedStack::new();
        for _ in 0..10 { s.push(Rc::clone(&x)); }
        s.reverse();
        drop(s.pop());
        assert_eq!(Rc::strong_count(&x), 10);
    }
    assert_eq!(Rc::strong_count(&x), 1);
}

fn dlist_deque_basico() {
    let mut d = DList::new();
    d.push_back(2);
    d.push_front(1);
    d.push_back(3);
    assert_eq!(d.iter().copied().collect::<Vec<_>>(), vec![1, 2, 3]);
    assert_eq!(d.iter_rev().copied().collect::<Vec<_>>(), vec![3, 2, 1]);
    assert_eq!((d.front(), d.back()), (Some(&1), Some(&3)));
    assert_eq!(d.pop_front(), Some(1));
    assert_eq!(d.pop_back(), Some(3));
    assert_eq!((d.front(), d.back()), (Some(&2), Some(&2)));
    assert_eq!(d.pop_back(), Some(2));
    assert!(d.is_empty());
    assert_eq!(d.pop_front(), None);
    d.push_front(9);
    assert_eq!(d.iter_rev().copied().collect::<Vec<_>>(), vec![9]);
}

fn dlist_sin_fugas() {
    let x = Rc::new(());
    {
        let mut d = DList::new();
        for i in 0..100 {
            if i % 2 == 0 { d.push_front(Rc::clone(&x)) } else { d.push_back(Rc::clone(&x)) }
        }
        drop(d.pop_front());
        drop(d.pop_back());
        assert_eq!(Rc::strong_count(&x), 99);
    }
    assert_eq!(Rc::strong_count(&x), 1);
}

fn dlist_aleatorio_vs_vecdeque() {
    let mut rng = Lcg::new(3);
    let mut d = DList::new();
    let mut o = VecDeque::new();
    for _ in 0..20000 {
        let x = rng.range(0, 999);
        match rng.below(4) {
            0 => { d.push_front(x); o.push_front(x); }
            1 => { d.push_back(x); o.push_back(x); }
            2 => assert_eq!(d.pop_front(), o.pop_front()),
            _ => assert_eq!(d.pop_back(), o.pop_back()),
        }
        assert_eq!(d.len(), o.len());
    }
    assert!(d.iter().eq(o.iter()));
    assert!(d.iter_rev().eq(o.iter().rev()));
}
