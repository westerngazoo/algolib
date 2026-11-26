//! LAB 06 — `BinaryHeap<T>`: min-heap binario implícito. Notas: notas/L06-heaps.md
//!
//! Convención: `peek()` es el MÍNIMO (al revés que `std::collections::BinaryHeap`,
//! que es max-heap). Para cambiar el orden en Rust se envuelve el tipo
//! (`std::cmp::Reverse<T>`) en vez de pasar un comparador como en C++.
//!
//! Complejidad: push/pop O(lg n); peek O(1); from_vec O(n).

pub struct BinaryHeap<T: Ord> {
    a: Vec<T>,
}

impl<T: Ord> BinaryHeap<T> {
    pub fn new() -> Self { todo!() }
    /// Heapify bottom-up en O(n).
    pub fn from_vec(v: Vec<T>) -> Self { todo!() }
    pub fn push(&mut self, x: T) { todo!() }
    pub fn pop(&mut self) -> Option<T> { todo!() }
    pub fn peek(&self) -> Option<&T> { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }
    /// Consume el heap y devuelve sus elementos en orden ascendente.
    pub fn into_sorted_vec(self) -> Vec<T> { todo!() }
}

impl<T: Ord> Default for BinaryHeap<T> {
    fn default() -> Self { Self::new() }
}
