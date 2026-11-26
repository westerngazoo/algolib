//! LAB 03 — Listas ligadas y ownership. Notas: notas/L03-listas-ownership.md
//!
//! El lab más instructivo para contrastar paradigmas:
//!   - `LinkedStack<T>` con `Option<Box<Node<T>>>`: 100% seguro. Ownership lineal.
//!     Aprenderás `Option::take`, `as_ref`, `as_deref`, `map` y por qué
//!     `Drop` debe ser iterativo (igual que en C++).
//!   - `DList<T>`: el grafo de punteros tiene ciclos (prev/next), lo que el modelo
//!     de ownership de Rust NO puede expresar con `Box`. Opciones:
//!       (a) `Rc<RefCell<Node>>` + `Weak` para prev: seguro pero costoso y verboso;
//!       (b) `NonNull<Node>` + `unsafe`, como `std::collections::LinkedList`.
//!     Este lab usa (b). Documenta cada `unsafe` con `// SAFETY:`.
//!
//! Lectura recomendada: "Learning Rust With Entirely Too Many Linked Lists".

mod dlist;
mod linked_stack;

pub use dlist::DList;
pub use linked_stack::LinkedStack;
