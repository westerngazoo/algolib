//! `LinkedStack<T>`: lista simplemente ligada usada como pila.

type Link<T> = Option<Box<Node<T>>>;

struct Node<T> {
    value: T,
    next: Link<T>,
}

pub struct LinkedStack<T> {
    head: Link<T>,
    len: usize,
}

impl<T> LinkedStack<T> {
    pub fn new() -> Self { todo!() }
    pub fn push(&mut self, x: T) { todo!() }
    pub fn pop(&mut self) -> Option<T> { todo!() }
    pub fn peek(&self) -> Option<&T> { todo!() }
    pub fn peek_mut(&mut self) -> Option<&mut T> { todo!() }
    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }

    /// In-place, O(n) tiempo, O(1) memoria extra.
    pub fn reverse(&mut self) { todo!() }

    /// Iterador por referencia, del tope al fondo.
    /// Pista: struct Iter<'a, T> { next: Option<&'a Node<T>> }
    pub fn iter(&self) -> Box<dyn Iterator<Item = &T> + '_> { todo!() }
}

impl<T> Default for LinkedStack<T> {
    fn default() -> Self { Self::new() }
}

impl<T> Drop for LinkedStack<T> {
    fn drop(&mut self) {
        // TODO: destrucción iterativa. El Drop derivado por defecto es recursivo.
    }
}
