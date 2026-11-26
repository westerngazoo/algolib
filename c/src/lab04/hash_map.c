/* LAB04 — tu implementación de <algolib/lab04/hash_map.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab04/hash_map.h>

#include <stdlib.h>
#include <string.h>

/* Sugerencia (encadenamiento): cada entrada guarda clave y valor contiguos en kv[]. */
typedef struct HMEntry {
    struct HMEntry *next;
    unsigned char kv[];
} HMEntry;

struct HashMap {
    HMEntry **buckets;
    size_t nbuckets;
    size_t size;
    size_t key_size, value_size;
    algolib_hash hash;
    algolib_eq eq;
};

HashMap *hash_map_new(size_t key_size, size_t value_size, algolib_hash hash, algolib_eq eq) {
    (void)key_size;
    (void)value_size;
    (void)hash;
    (void)eq;
    ALGOLIB_TODO();
}

void hash_map_free(HashMap *m) {
    (void)m;
    ALGOLIB_TODO();
}

bool hash_map_insert(HashMap *m, const void *key, const void *value) {
    (void)m;
    (void)key;
    (void)value;
    ALGOLIB_TODO();
}

void *hash_map_find(const HashMap *m, const void *key) {
    (void)m;
    (void)key;
    ALGOLIB_TODO();
}

bool hash_map_erase(HashMap *m, const void *key) {
    (void)m;
    (void)key;
    ALGOLIB_TODO();
}

size_t hash_map_size(const HashMap *m) {
    (void)m;
    ALGOLIB_TODO();
}

double hash_map_load_factor(const HashMap *m) {
    (void)m;
    ALGOLIB_TODO();
}
