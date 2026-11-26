//! `MinStack<T>`: pila con `min()` en O(1) peor caso. (Problem Set 1, ej. 8)
//!
//! Sugerencia: compón tu `ArrayStack`. Enuncia y demuestra el invariante.

use super::ArrayStack;

pub struct MinStack<T: Ord + Clone> {
    // Representación libre. Ejemplo:
    items: ArrayStack<T>,
    mins: ArrayStack<T>,
}

impl<T: Ord + Clone> MinStack<T> {
    pub fn new() -> Self { todo!() }
    pub fn push(&mut self, x: T) { todo!() }
    pub fn pop(&mut self) -> Option<T> { todo!() }
    pub fn peek(&self) -> Option<&T> { todo!() }
    pub fn min(&self) -> Option<&T> { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }
}

impl<T: Ord + Clone> Default for MinStack<T> {
    fn default() -> Self { Self::new() }
}
