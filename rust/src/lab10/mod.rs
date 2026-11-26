//! LAB 10 — Caminos mínimos con pesos. Notas: notas/L10-caminos-minimos.md
//!
//! Mismos contratos que en C++. `Option<i64>`: `None` = inalcanzable.
//! Para Dijkstra usa tu `lab06::BinaryHeap` con tuplas `(dist, vértice)`
//! (las tuplas implementan `Ord` lexicográficamente).

pub struct WeightedGraph {
    adj: Vec<Vec<(usize, i64)>>,
}

impl WeightedGraph {
    pub fn new(n: usize) -> Self { todo!() }
    pub fn add_edge(&mut self, u: usize, v: usize, w: i64) { todo!() }
    pub fn num_vertices(&self) -> usize { todo!() }
    /// Pares (destino, peso).
    pub fn neighbors(&self, u: usize) -> &[(usize, i64)] { todo!() }
}

pub type Dist = Option<i64>;

/// Precondición: pesos >= 0.
pub fn dijkstra(g: &WeightedGraph, s: usize) -> Vec<Dist> { todo!() }

/// `None` si hay un ciclo negativo alcanzable desde `s`.
pub fn bellman_ford(g: &WeightedGraph, s: usize) -> Option<Vec<Dist>> { todo!() }

/// Precondición: `g` es acíclico.
pub fn dag_shortest_paths(g: &WeightedGraph, s: usize) -> Vec<Dist> { todo!() }

/// Precondición: sin ciclos negativos.
pub fn floyd_warshall(g: &WeightedGraph) -> Vec<Vec<Dist>> { todo!() }
