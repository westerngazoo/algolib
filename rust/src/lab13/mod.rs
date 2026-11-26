//! LAB 13 — Tries, Fenwick y segment trees. Notas: notas/L13-tries-arboles-de-rangos.md
//!
//! Mismos contratos que en C++ (ver trie.hpp y range_trees.hpp).

pub struct Trie {
    // Representación libre. Ejemplo:
    // root: Node, len: usize   con   struct Node { next: [Option<Box<Node>>; 26], ... }
}

impl Trie {
    pub fn new() -> Self { todo!() }
    pub fn insert(&mut self, w: &str) -> bool { todo!() }
    pub fn contains(&self, w: &str) -> bool { todo!() }
    pub fn starts_with(&self, p: &str) -> bool { todo!() }
    pub fn count_with_prefix(&self, p: &str) -> usize { todo!() }
    pub fn remove(&mut self, w: &str) -> bool { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { self.len() == 0 }
    /// Número de nodos incluida la raíz.
    pub fn node_count(&self) -> usize { todo!() }
}

impl Default for Trie {
    fn default() -> Self { Self::new() }
}

pub struct FenwickTree {
    bit: Vec<i64>,
}

impl FenwickTree {
    pub fn new(n: usize) -> Self { todo!() }
    pub fn add(&mut self, i: usize, delta: i64) { todo!() }
    /// a[0] + ... + a[i-1]
    pub fn prefix_sum(&self, i: usize) -> i64 { todo!() }
    pub fn range_sum(&self, l: usize, r: usize) -> i64 { self.prefix_sum(r) - self.prefix_sum(l) }
}

pub struct SegmentTree {
    n: usize,
    t: Vec<i64>,
}

impl SegmentTree {
    pub fn new(a: &[i64]) -> Self { todo!() }
    pub fn set(&mut self, i: usize, x: i64) { todo!() }
    /// Mínimo en [l, r); `None` si el rango es vacío.
    pub fn range_min(&self, l: usize, r: usize) -> Option<i64> { todo!() }
}
