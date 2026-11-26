//! `ArrayStack<T>`: pila sobre arreglo dinámico.
//!
//! TAREA: reemplaza cada `todo!()`.
//!
//! Representación sugerida (sin `unsafe`): `buf: Box<[Option<T>]>`, `len`.
//! Invariante: `len <= buf.len()` y `buf[i].is_some() <=> i < len`.
//!
//! Complejidad: `push` O(1) amortizado (factor 2); `pop`/`peek` O(1).
//!
//! Contraste con C++:
//!   - pila vacía: `Option<T>` en vez de excepción (función parcial → total).
//!   - no hay regla de los cinco: el movimiento es implícito y `Drop` es automático;
//!     `Clone` es opt-in y solo existe si `T: Clone`.
//!   - el aliasing `s.push(*s.peek())` no compila (borrow checker).

pub struct ArrayStack<T> {
    buf: Box<[Option<T>]>,
    len: usize,
}

impl<T> ArrayStack<T> {
    pub fn new() -> Self { todo!() }

    /// O(1) amortizado.
    pub fn push(&mut self, x: T) { todo!() }

    /// `None` si está vacía.
    pub fn pop(&mut self) -> Option<T> { todo!() }

    pub fn peek(&self) -> Option<&T> { todo!() }
    pub fn peek_mut(&mut self) -> Option<&mut T> { todo!() }

    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }
    pub fn capacity(&self) -> usize { todo!() }
    pub fn clear(&mut self) { todo!() }

    /// Recorre del tope hacia el fondo.
    /// (Puedes cambiar el tipo de retorno a `impl Iterator<Item = &T> + '_`
    /// cuando lo implementes; la caja solo existe para que el stub compile.)
    pub fn iter(&self) -> Box<dyn Iterator<Item = &T> + '_> { todo!() }
}

impl<T> Default for ArrayStack<T> {
    fn default() -> Self { Self::new() }
}

impl<T: Clone> Clone for ArrayStack<T> {
    fn clone(&self) -> Self { todo!() }
}
