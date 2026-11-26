//! LAB 11 — Union-Find y MST. Notas: notas/L11-union-find-mst.md

pub struct DisjointSet {
    parent: Vec<usize>,
    rank_or_size: Vec<usize>,
    sets: usize,
}

impl DisjointSet {
    pub fn new(n: usize) -> Self { todo!() }
    /// `&mut self`: la compresión de caminos muta la estructura.
    pub fn find(&mut self, x: usize) -> usize { todo!() }
    /// `false` si ya estaban juntos.
    pub fn unite(&mut self, a: usize, b: usize) -> bool { todo!() }
    pub fn same(&mut self, a: usize, b: usize) -> bool { self.find(a) == self.find(b) }
    pub fn count(&self) -> usize { todo!() }
    pub fn size_of(&mut self, x: usize) -> usize { todo!() }
}

#[derive(Clone, Copy, Debug)]
pub struct Edge {
    pub u: usize,
    pub v: usize,
    pub w: i64,
}

/// Peso total del MST; `None` si no es conexo.
pub fn kruskal(n: usize, edges: &[Edge]) -> Option<i64> { todo!() }

/// Peso total del MST; `None` si no es conexo.
pub fn prim(n: usize, edges: &[Edge]) -> Option<i64> { todo!() }
