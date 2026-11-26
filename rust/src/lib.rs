//! algolib — nuestra librería de estructuras de datos y algoritmos.
//!
//! Cada `labNN` contiene la API del laboratorio con cuerpos `todo!()`.
//! El alumno los reemplaza por su implementación hasta que
//! `cargo test --test labNN` quede en verde.

// Mientras los labs están sin implementar, los campos sugeridos y los parámetros
// aparecen como "no usados". Quita esta línea cuando termines el curso.
#![allow(dead_code, unused_variables, unused_mut, unused_imports)]

pub mod lab01; // Pilas sobre arreglo dinámico; análisis amortizado
pub mod lab02; // Vector con memoria no inicializada
pub mod lab03; // Listas ligadas y ownership
pub mod lab04; // Colas (buffer circular) y tablas hash
pub mod lab05; // Ordenamiento
pub mod lab06; // Heaps binarios
pub mod lab07; // Árboles AVL
pub mod lab08; // Árboles aumentados: estadísticos de orden
pub mod lab09; // Grafos: BFS, DFS, orden topológico, SCC
pub mod lab10; // Caminos mínimos
pub mod lab11; // Union-Find y árboles de expansión mínima
pub mod lab12; // Programación dinámica
pub mod lab13; // Tries, Fenwick y segment trees
