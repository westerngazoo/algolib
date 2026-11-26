//! LAB 02 — `Vector<T>` con memoria no inicializada (`unsafe`). Notas: notas/L02-vector-memoria.md
//!
//! Aquí reimplementamos lo esencial de `std::Vec` para entender:
//!   - `std::alloc::{alloc, realloc, dealloc, Layout}`
//!   - `ptr::{read, write, copy}` y por qué son `unsafe`
//!   - `NonNull<T>` + `PhantomData<T>` (varianza y drop check)
//!   - tipos de tamaño cero (ZST): `size_of::<T>() == 0` → no se reserva nada
//!   - `Deref<Target = [T]>`: con `as_slice` obtienes gratis todo el API de slices
//!
//! Regla de oro: cada bloque `unsafe` lleva un comentario `// SAFETY:` que justifica
//! por qué se cumplen las precondiciones. Esto es el equivalente a demostrar
//! el invariante de representación.
//!
//! Contraste con C++: en C++ *todo* es "unsafe"; en Rust el `unsafe` queda
//! encapsulado y la API pública es segura (no hay forma de causar UB desde fuera).

use std::marker::PhantomData;
use std::ops::{Deref, DerefMut};
use std::ptr::NonNull;

pub struct Vector<T> {
    ptr: NonNull<T>, // NonNull::dangling() cuando cap == 0 o T es ZST
    cap: usize,
    len: usize,
    _marker: PhantomData<T>, // "este tipo es dueño de valores T"
}

// Al usar punteros crudos, Rust ya no infiere Send/Sync. Son correctos aquí
// por las mismas razones que en Vec<T>.
unsafe impl<T: Send> Send for Vector<T> {}
unsafe impl<T: Sync> Sync for Vector<T> {}

impl<T> Vector<T> {
    pub fn new() -> Self { todo!() }
    pub fn with_capacity(cap: usize) -> Self { todo!() }

    pub fn len(&self) -> usize { todo!() }
    pub fn is_empty(&self) -> bool { todo!() }
    /// Para ZST debe ser `usize::MAX`.
    pub fn capacity(&self) -> usize { todo!() }

    pub fn push(&mut self, x: T) { todo!() }
    pub fn pop(&mut self) -> Option<T> { todo!() }

    /// Inserta antes de `i`. Panic si `i > len`.
    pub fn insert(&mut self, i: usize, x: T) { todo!() }
    /// Quita y devuelve el elemento `i`. Panic si `i >= len`.
    pub fn remove(&mut self, i: usize) -> T { todo!() }

    /// Garantiza `capacity() >= len() + additional`.
    pub fn reserve(&mut self, additional: usize) { todo!() }
    pub fn shrink_to_fit(&mut self) { todo!() }
    pub fn clear(&mut self) { todo!() }

    pub fn as_slice(&self) -> &[T] { todo!() }
    pub fn as_mut_slice(&mut self) -> &mut [T] { todo!() }
}

impl<T> Drop for Vector<T> {
    fn drop(&mut self) {
        // TODO: destruir los elementos vivos y liberar el bloque.
    }
}

impl<T> Default for Vector<T> {
    fn default() -> Self { Self::new() }
}

impl<T: Clone> Clone for Vector<T> {
    fn clone(&self) -> Self { todo!() }
}

// --- Ya implementados: dependen de as_slice / as_mut_slice -------------------

impl<T> Deref for Vector<T> {
    type Target = [T];
    fn deref(&self) -> &[T] { self.as_slice() }
}

impl<T> DerefMut for Vector<T> {
    fn deref_mut(&mut self) -> &mut [T] { self.as_mut_slice() }
}

impl<T> FromIterator<T> for Vector<T> {
    fn from_iter<I: IntoIterator<Item = T>>(it: I) -> Self {
        let mut v = Vector::new();
        for x in it { v.push(x); }
        v
    }
}
