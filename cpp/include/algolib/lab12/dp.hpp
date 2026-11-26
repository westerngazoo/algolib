// LAB 12 — Programación dinámica. Notas: notas/L12-programacion-dinamica.md
//
// Para cada problema documenta el marco SRTBOT (MIT 6.006):
//   Subproblemas, Relación, Topological order, Base, Original, Tiempo.
//
//  función            complejidad exigida
//  fibonacci          O(n) tiempo, O(1) memoria                 (n <= 93 cabe en uint64)
//  lcs_length         O(|a|·|b|) tiempo, O(min(|a|,|b|)) memoria
//  edit_distance      O(|a|·|b|) (Levenshtein: insertar, borrar, sustituir = costo 1)
//  knapsack_01        O(n·W) pseudo-polinomial — explica por qué NO es polinomial
//  lis_length         O(n lg n) (patience sorting + búsqueda binaria), estrictamente creciente
//  coin_change_min    O(n·amount); nullopt si no se puede formar
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace algolib {

inline std::uint64_t fibonacci(unsigned n) { (void)n; ALGOLIB_TODO(); }

inline std::size_t lcs_length(const std::string& a, const std::string& b) { (void)a; (void)b; ALGOLIB_TODO(); }

inline std::size_t edit_distance(const std::string& a, const std::string& b) { (void)a; (void)b; ALGOLIB_TODO(); }

inline std::uint64_t knapsack_01(const std::vector<std::size_t>& weights,
                                 const std::vector<std::uint64_t>& values, std::size_t capacity) {
    (void)weights; (void)values; (void)capacity; ALGOLIB_TODO();
}

inline std::size_t lis_length(const std::vector<std::int64_t>& a) { (void)a; ALGOLIB_TODO(); }

inline std::optional<std::size_t> coin_change_min(const std::vector<std::size_t>& coins, std::size_t amount) {
    (void)coins; (void)amount; ALGOLIB_TODO();
}

}  // namespace algolib
