//! `DList<T>`: lista doblemente ligada con punteros crudos.
//!
//! Invariante (igual que en C++):
//!   len == 0 <=> head == tail == None
//!   (*head).prev == None, (*tail).next == None
//!   ∀ n con next = Some(m): (*m).prev == Some(n)
//! Cada nodo se crea con `Box::into_raw` y se destruye exactamente una vez con
//! `Box::from_raw`.

use std::marker::PhantomData;
use std::ptr::NonNull;

type Link<T> = Option<NonNull<Node<T>>>;

struct Node<T> {
    value: T,
    prev: Link<T>,
    next: Link<T>,
}

pub struct DList<T> {
    head: Link<T>,
    tail: Link<T>,
    len: usize,
    _marker: PhantomData<Box<Node<T>>>,
}

impl<T> DList<T> {
    pub fn new() -> Self { todo!() }
    pub fn push_front(&mut self, x: T) { todo!() }
    pub fn push_back(&mut self, x: T) { todo!() }
    pub fn pop_front(&mut self) -> Option<T> { todo!() }
    pub fn pop_back(&mut self) -> Option<T> { todo!() }
    pub fn front(&self) -> Option<&T> { todo!() }
    pub fn back(&self) -> Option<&T> { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }

    /// Frente → final.
    pub fn iter(&self) -> Box<dyn Iterator<Item = &T> + '_> { todo!() }
    /// Final → frente (recorre `prev`).
    pub fn iter_rev(&self) -> Box<dyn Iterator<Item = &T> + '_> { todo!() }
}

impl<T> Default for DList<T> {
    fn default() -> Self { Self::new() }
}

impl<T> Drop for DList<T> {
    fn drop(&mut self) {
        // TODO: liberar todos los nodos.
    }
}
