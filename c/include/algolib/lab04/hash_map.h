/* LAB 04 — HashMap: tabla hash genérica (claves y valores de tamaño fijo).
 * Encadenamiento o direccionamiento abierto (documenta cuál). Factor de carga <= 1;
 * insert/find/erase O(1) esperado; correcta aun si todas las claves colisionan.
 * En C la función hash y la igualdad se pasan como punteros a función. */
#ifndef ALGOLIB_LAB04_HASH_MAP_H
#define ALGOLIB_LAB04_HASH_MAP_H

#include <algolib/common.h>

typedef size_t (*algolib_hash)(const void *key);
typedef bool (*algolib_eq)(const void *a, const void *b);

typedef struct HashMap HashMap;

HashMap *hash_map_new(size_t key_size, size_t value_size, algolib_hash hash, algolib_eq eq);
void hash_map_free(HashMap *m);
bool hash_map_insert(HashMap *m, const void *key, const void *value);
void *hash_map_find(const HashMap *m, const void *key);
bool hash_map_erase(HashMap *m, const void *key);
size_t hash_map_size(const HashMap *m);
double hash_map_load_factor(const HashMap *m);

/* hash_map_insert: inserta o reemplaza el valor; true si la clave era nueva.
 * hash_map_find:   puntero al valor guardado, o NULL. */

/* Hash y comparación listos para claves int (ya implementados). */
static inline size_t algolib_hash_int(const void *key) {
    uint64_t x = (uint64_t)(int64_t)(*(const int *)key);   /* mezcla de splitmix64 */
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return (size_t)(x ^ (x >> 31));
}
static inline bool algolib_eq_int(const void *a, const void *b) {
    return *(const int *)a == *(const int *)b;
}

#endif
