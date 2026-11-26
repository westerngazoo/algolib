mod common;
use algolib::lab12::*;
use common::Lcg;

fn main() {
    common::run(
        "lab12",
        cases![
            fibonacci_valores,
            lcs_clasicos,
            edit_distance_clasicos,
            lcs_y_edit_aleatorio_vs_recursion,
            knapsack_vs_subconjuntos,
            lis_vs_cuadratico,
            coin_change,
        ],
    );
}

fn fibonacci_valores() {
    assert_eq!(fibonacci(0), 0);
    assert_eq!(fibonacci(1), 1);
    assert_eq!(fibonacci(10), 55);
    assert_eq!(fibonacci(90), 2880067194370816120);
    assert_eq!(fibonacci(93), 12200160415121876738);
}

fn lcs_clasicos() {
    assert_eq!(lcs_length("ABCBDAB", "BDCABA"), 4);
    assert_eq!(lcs_length("", "abc"), 0);
    assert_eq!(lcs_length("abc", "abc"), 3);
    assert_eq!(lcs_length("abc", "def"), 0);
}

fn edit_distance_clasicos() {
    assert_eq!(edit_distance("kitten", "sitting"), 3);
    assert_eq!(edit_distance("", "abc"), 3);
    assert_eq!(edit_distance("intention", "execution"), 5);
    assert_eq!(edit_distance("same", "same"), 0);
}

fn lcs_rec(a: &[u8], b: &[u8]) -> usize {
    match (a.split_last(), b.split_last()) {
        (Some((x, a2)), Some((y, b2))) => {
            if x == y { 1 + lcs_rec(a2, b2) } else { lcs_rec(a2, b).max(lcs_rec(a, b2)) }
        }
        _ => 0,
    }
}

fn ed_rec(a: &[u8], b: &[u8]) -> usize {
    match (a.split_last(), b.split_last()) {
        (None, _) => b.len(),
        (_, None) => a.len(),
        (Some((x, a2)), Some((y, b2))) => {
            (ed_rec(a2, b) + 1).min(ed_rec(a, b2) + 1).min(ed_rec(a2, b2) + (x != y) as usize)
        }
    }
}

fn lcs_y_edit_aleatorio_vs_recursion() {
    let mut rng = Lcg::new(20);
    for _ in 0..40 {
        let a: String = (0..rng.below(8)).map(|_| (b'a' + rng.below(3) as u8) as char).collect();
        let b: String = (0..rng.below(8)).map(|_| (b'a' + rng.below(3) as u8) as char).collect();
        assert_eq!(lcs_length(&a, &b), lcs_rec(a.as_bytes(), b.as_bytes()), "{a} {b}");
        assert_eq!(edit_distance(&a, &b), ed_rec(a.as_bytes(), b.as_bytes()), "{a} {b}");
    }
}

fn knapsack_vs_subconjuntos() {
    assert_eq!(knapsack_01(&[1, 3, 4, 5], &[1, 4, 5, 7], 7), 9);
    assert_eq!(knapsack_01(&[], &[], 10), 0);
    let mut rng = Lcg::new(21);
    for _ in 0..40 {
        let n = rng.below(13) as usize;
        let cap = rng.below(50) as usize;
        let w: Vec<usize> = (0..n).map(|_| 1 + rng.below(20) as usize).collect();
        let v: Vec<u64> = (0..n).map(|_| rng.below(100)).collect();
        let mut best = 0;
        for mask in 0u32..(1 << n) {
            let (mut ws, mut vs) = (0, 0);
            for i in 0..n {
                if mask >> i & 1 == 1 { ws += w[i]; vs += v[i]; }
            }
            if ws <= cap { best = best.max(vs); }
        }
        assert_eq!(knapsack_01(&w, &v, cap), best);
    }
}

fn lis_vs_cuadratico() {
    assert_eq!(lis_length(&[10, 9, 2, 5, 3, 7, 101, 18]), 4);
    assert_eq!(lis_length(&[]), 0);
    assert_eq!(lis_length(&[7, 7, 7]), 1);
    let mut rng = Lcg::new(22);
    for _ in 0..30 {
        let a: Vec<i64> = (0..rng.below(300)).map(|_| rng.range(-50, 50)).collect();
        let mut l = vec![1usize; a.len()];
        let mut best = 0;
        for i in 0..a.len() {
            for j in 0..i {
                if a[j] < a[i] { l[i] = l[i].max(l[j] + 1); }
            }
            best = best.max(l[i]);
        }
        assert_eq!(lis_length(&a), best);
    }
    let grande: Vec<i64> = (0..1_000_000).collect();
    assert_eq!(lis_length(&grande), grande.len());
}

fn coin_change() {
    assert_eq!(coin_change_min(&[1, 5, 6, 9], 11), Some(2));
    assert_eq!(coin_change_min(&[1, 2, 5], 0), Some(0));
    assert_eq!(coin_change_min(&[2], 3), None);
    assert_eq!(coin_change_min(&[186, 419, 83, 408], 6249), Some(20));
}
