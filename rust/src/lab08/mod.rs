//! LAB 08 — `OrderStatSet<K>`: árbol aumentado con `select`/`rank`.
//! Notas: notas/L08-arboles-aumentados.md
//!
//! Igual que en C++: cada nodo mantiene el tamaño de su subárbol.
//! Puedes reutilizar el diseño del lab07 o usar un treap.

pub struct OrderStatSet<K: Ord> {
    // Representación libre.
    _todo: std::marker::PhantomData<K>,
}

impl<K: Ord> OrderStatSet<K> {
    pub fn new() -> Self { todo!() }
    /// `false` si ya estaba.
    pub fn insert(&mut self, key: K) -> bool { todo!() }
    /// `false` si no estaba.
    pub fn remove(&mut self, key: &K) -> bool { todo!() }
    pub fn contains(&self, key: &K) -> bool { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { self.len() == 0 }
    /// k-ésimo menor, 0-indexado.
    pub fn select(&self, k: usize) -> Option<&K> { todo!() }
    /// Número de elementos estrictamente menores que `key`.
    pub fn rank(&self, key: &K) -> usize { todo!() }
}

impl<K: Ord> Default for OrderStatSet<K> {
    fn default() -> Self { Self::new() }
}
