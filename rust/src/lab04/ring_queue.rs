//! `RingQueue<T>`: cola FIFO sobre buffer circular que crece.
//!
//! Elemento i-ésimo (0 = frente) en `buf[(head + i) % buf.len()]`.
//! push O(1) amortizado; pop/front/back/get O(1).

pub struct RingQueue<T> {
    buf: Box<[Option<T>]>,
    head: usize,
    len: usize,
}

impl<T> RingQueue<T> {
    pub fn new() -> Self { todo!() }
    pub fn push_back(&mut self, x: T) { todo!() }
    pub fn pop_front(&mut self) -> Option<T> { todo!() }
    pub fn front(&self) -> Option<&T> { todo!() }
    pub fn back(&self) -> Option<&T> { todo!() }
    /// i-ésimo desde el frente.
    pub fn get(&self, i: usize) -> Option<&T> { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }
    pub fn capacity(&self) -> usize { todo!() }
}

impl<T> Default for RingQueue<T> {
    fn default() -> Self { Self::new() }
}
