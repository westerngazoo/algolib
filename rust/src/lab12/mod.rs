//! LAB 12 — Programación dinámica. Notas: notas/L12-programacion-dinamica.md
//!
//! Mismos contratos y complejidades que en C++ (ver dp.hpp). Usa SRTBOT.
//!
//! Contraste con C++: `&str` es UTF-8; para indexar por posición trabaja
//! con `as_bytes()` (los tests usan ASCII) o `chars().collect::<Vec<_>>()`.

pub fn fibonacci(n: u32) -> u64 { todo!() }

pub fn lcs_length(a: &str, b: &str) -> usize { todo!() }

pub fn edit_distance(a: &str, b: &str) -> usize { todo!() }

pub fn knapsack_01(weights: &[usize], values: &[u64], capacity: usize) -> u64 { todo!() }

/// Longitud de la subsecuencia ESTRICTAMENTE creciente más larga, O(n lg n).
pub fn lis_length(a: &[i64]) -> usize { todo!() }

pub fn coin_change_min(coins: &[usize], amount: usize) -> Option<usize> { todo!() }
