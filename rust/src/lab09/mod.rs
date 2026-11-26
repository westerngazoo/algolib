//! LAB 09 — Grafos dirigidos: representación y recorridos. Notas: notas/L09-grafos-recorridos.md
//!
//! Mismos requisitos que en C++ (ver cpp/include/algolib/lab09/graph.hpp),
//! incluido DFS ITERATIVO (tests con caminos de 200 000 vértices).
//!
//! Contraste con C++: en Rust la recursión profunda también desborda (pila de
//! 8 MiB en el hilo principal, 2 MiB en hilos de test). Ninguno de los dos
//! lenguajes garantiza optimización de llamada de cola.

pub struct Graph {
    adj: Vec<Vec<usize>>,
}

impl Graph {
    pub fn new(n: usize) -> Self { todo!() }
    pub fn add_edge(&mut self, u: usize, v: usize) { todo!() }
    pub fn num_vertices(&self) -> usize { todo!() }
    pub fn neighbors(&self, u: usize) -> &[usize] { todo!() }
}

pub fn bfs_distances(g: &Graph, s: usize) -> Vec<Option<usize>> { todo!() }

/// Preorden del DFS recursivo, vecinos en orden de inserción.
pub fn dfs_preorder(g: &Graph, s: usize) -> Vec<usize> { todo!() }

/// `None` si hay ciclo.
pub fn topological_sort(g: &Graph) -> Option<Vec<usize>> { todo!() }

/// Id de componente por vértice (ids arbitrarios).
pub fn strongly_connected_components(g: &Graph) -> Vec<usize> { todo!() }
