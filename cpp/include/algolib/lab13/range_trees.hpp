// LAB 13 — Estructuras para consultas de rango.
//
// FenwickTree (Binary Indexed Tree): sumas prefijas con actualizaciones puntuales.
//   add(i, δ) y prefix_sum(i) en O(lg n); memoria n + 1. Índices públicos 0..n-1.
//   prefix_sum(i) = a[0] + ... + a[i-1]   (semiabierto: prefix_sum(0) = 0)
//   range_sum(l, r) = a[l] + ... + a[r-1]
//
// SegmentTree: mínimo en rango con asignación puntual.
//   construcción O(n); set(i, x) y range_min(l, r) en O(lg n); intervalos [l, r).
//   range_min de un rango vacío: std::nullopt.
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace algolib {

class FenwickTree {
public:
    explicit FenwickTree(std::size_t n) { (void)n; ALGOLIB_TODO(); }
    void add(std::size_t i, std::int64_t delta) { (void)i; (void)delta; ALGOLIB_TODO(); }
    std::int64_t prefix_sum(std::size_t i) const { (void)i; ALGOLIB_TODO(); }
    std::int64_t range_sum(std::size_t l, std::size_t r) const { return prefix_sum(r) - prefix_sum(l); }

private:
    std::vector<std::int64_t> bit_;
};

class SegmentTree {
public:
    explicit SegmentTree(const std::vector<std::int64_t>& a) { (void)a; ALGOLIB_TODO(); }
    void set(std::size_t i, std::int64_t x) { (void)i; (void)x; ALGOLIB_TODO(); }
    std::optional<std::int64_t> range_min(std::size_t l, std::size_t r) const { (void)l; (void)r; ALGOLIB_TODO(); }

private:
    std::size_t n_ = 0;
    std::vector<std::int64_t> t_;
};

}  // namespace algolib
