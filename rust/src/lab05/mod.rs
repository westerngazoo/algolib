//! LAB 05 — Ordenamiento. Notas: notas/L05-ordenamiento.md
//!
//! Misma tabla de requisitos que en C++ (ver cpp/include/algolib/lab05/sorting.hpp):
//! merge_sort ESTABLE; quick_sort con pivote aleatorio y partición en 3 vías;
//! heap_sort in-place con heapify O(n); counting O(n+k); radix LSD base 256.
//!
//! Contraste con C++:
//!   - `&mut [T]` ≈ `std::span<T>`, pero con exclusividad garantizada.
//!   - Para recursión divide-y-vencerás usa `split_at_mut`: el borrow checker no
//!     permite dos `&mut` a la misma slice, pero sí a dos mitades disjuntas.
//!   - `merge_sort` pide `T: Clone` para el buffer auxiliar (o `unsafe` con
//!     `ptr::copy_nonoverlapping`, como hace `std`). En C++ se movería con std::move.
//!   - No hay generador aleatorio en `std`: escribe uno pequeño (xorshift/LCG).

pub fn insertion_sort<T: Ord>(a: &mut [T]) { todo!() }

pub fn merge_sort<T: Ord + Clone>(a: &mut [T]) { todo!() }

pub fn quick_sort<T: Ord>(a: &mut [T]) { todo!() }

pub fn heap_sort<T: Ord>(a: &mut [T]) { todo!() }

/// Claves en `[0, max_key]`.
pub fn counting_sort(a: &mut [u32], max_key: u32) { todo!() }

pub fn radix_sort(a: &mut [u32]) { todo!() }
