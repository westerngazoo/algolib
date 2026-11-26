// LAB 04 — HashMap<K, V, Hash>: tabla hash.
//
// Estrategia a tu elección (documenta cuál y por qué):
//   (a) encadenamiento separado: vector de listas/vectores de pares;
//   (b) direccionamiento abierto con sondeo lineal + borrado con lápidas o
//       backward-shift deletion.
// Requisitos:
//   - factor de carga acotado por una constante (redimensiona al superarla);
//   - insert/find/erase en O(1) ESPERADO bajo hashing uniforme simple;
//   - correcto aun con un hash pésimo (todas las claves colisionan) — ver test.
#pragma once

#include <algolib/common.hpp>

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace algolib {

template <class K, class V, class Hash = std::hash<K>>
class HashMap {
public:
    HashMap() = default;

    // Inserta o reemplaza. Devuelve true si la clave era nueva.
    bool insert(const K& key, const V& value) { (void)key; (void)value; ALGOLIB_TODO(); }

    // Puntero al valor o nullptr si no existe.
    V* find(const K& key) { (void)key; ALGOLIB_TODO(); }
    const V* find(const K& key) const { (void)key; ALGOLIB_TODO(); }

    bool contains(const K& key) const { return find(key) != nullptr; }

    // Devuelve true si la clave existía.
    bool erase(const K& key) { (void)key; ALGOLIB_TODO(); }

    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    double load_factor() const { ALGOLIB_TODO(); }

private:
    // Representación libre. Ejemplo (encadenamiento):
    // std::vector<std::vector<std::pair<K, V>>> buckets_;
    std::size_t size_ = 0;
    Hash hash_{};
};

}  // namespace algolib
