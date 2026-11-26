//! `HashMap<K, V, S>`: tabla hash.
//!
//! Estrategia libre (encadenamiento o direccionamiento abierto); factor de carga
//! acotado; O(1) esperado; correcta con un hasher pésimo (ver tests).
//!
//! Contraste con C++: en Rust el hasher se separa en dos traits:
//!   `Hash` (cómo un tipo alimenta bytes) y `BuildHasher`/`Hasher` (el algoritmo).
//! En C++ un único functor `std::hash<K>` hace ambas cosas.

use std::collections::hash_map::RandomState;
use std::hash::{BuildHasher, Hash};

pub struct HashMap<K, V, S = RandomState> {
    // Representación libre. Ejemplo (encadenamiento):
    buckets: Vec<Vec<(K, V)>>,
    len: usize,
    hasher: S,
}

impl<K: Hash + Eq, V> HashMap<K, V, RandomState> {
    pub fn new() -> Self { Self::with_hasher(RandomState::new()) }
}

impl<K: Hash + Eq, V, S: BuildHasher> HashMap<K, V, S> {
    pub fn with_hasher(hasher: S) -> Self { todo!() }

    /// Inserta o reemplaza; devuelve el valor anterior (como `std`).
    pub fn insert(&mut self, key: K, value: V) -> Option<V> { todo!() }
    pub fn get(&self, key: &K) -> Option<&V> { todo!() }
    pub fn get_mut(&mut self, key: &K) -> Option<&mut V> { todo!() }
    pub fn contains_key(&self, key: &K) -> bool { self.get(key).is_some() }
    pub fn remove(&mut self, key: &K) -> Option<V> { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }
    pub fn load_factor(&self) -> f64 { todo!() }
}

impl<K: Hash + Eq, V> Default for HashMap<K, V, RandomState> {
    fn default() -> Self { Self::new() }
}
