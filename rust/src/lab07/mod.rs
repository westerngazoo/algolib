//! LAB 07 — `AvlMap<K, V>`: diccionario ordenado sobre árbol AVL. Notas: notas/L07-bst-avl.md
//!
//! Mismos invariantes y cota de altura que en C++.
//!
//! Contraste con C++: el patrón natural en Rust es
//!     fn insert(t: Link<K, V>, k: K, v: V) -> (Link<K, V>, Option<V>)
//! es decir, *tomar* el subárbol por valor (ownership) y *devolver* el nuevo.
//! Las rotaciones son movimientos de `Box`, sin copias ni punteros colgantes.
//! `Option::take()` es tu herramienta principal.

use std::cmp::Ordering;

type Link<K, V> = Option<Box<Node<K, V>>>;

struct Node<K, V> {
    key: K,
    value: V,
    height: i32,
    left: Link<K, V>,
    right: Link<K, V>,
}

pub struct AvlMap<K: Ord, V> {
    root: Link<K, V>,
    len: usize,
}

impl<K: Ord, V> AvlMap<K, V> {
    pub fn new() -> Self { todo!() }
    /// Inserta o reemplaza; devuelve el valor anterior.
    pub fn insert(&mut self, key: K, value: V) -> Option<V> { todo!() }
    pub fn get(&self, key: &K) -> Option<&V> { todo!() }
    pub fn get_mut(&mut self, key: &K) -> Option<&mut V> { todo!() }
    pub fn contains_key(&self, key: &K) -> bool { self.get(key).is_some() }
    pub fn remove(&mut self, key: &K) -> Option<V> { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }
    /// 0 si vacío, 1 si solo raíz.
    pub fn height(&self) -> i32 { todo!() }
    /// Recorrido in-order.
    pub fn keys(&self) -> Vec<&K> { todo!() }
    pub fn min_key(&self) -> Option<&K> { todo!() }
    pub fn max_key(&self) -> Option<&K> { todo!() }
}

impl<K: Ord, V> Default for AvlMap<K, V> {
    fn default() -> Self { Self::new() }
}
