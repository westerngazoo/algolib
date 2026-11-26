/* LAB07 — tu implementación de <algolib/lab07/avl_map.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab07/avl_map.h>

#include <stdlib.h>
#include <string.h>

typedef struct AvlNode {
    struct AvlNode *left, *right;
    int height;
    unsigned char kv[];   /* clave (key_size bytes) seguida del valor (value_size bytes) */
} AvlNode;

struct AvlMap {
    AvlNode *root;
    size_t size;
    size_t key_size, value_size;
    algolib_cmp cmp;
};

AvlMap *avl_map_new(size_t key_size, size_t value_size, algolib_cmp key_cmp) {
    (void)key_size;
    (void)value_size;
    (void)key_cmp;
    ALGOLIB_TODO();
}

void avl_map_free(AvlMap *m) {
    (void)m;
    ALGOLIB_TODO();
}

bool avl_map_insert(AvlMap *m, const void *key, const void *value) {
    (void)m;
    (void)key;
    (void)value;
    ALGOLIB_TODO();
}

void *avl_map_find(const AvlMap *m, const void *key) {
    (void)m;
    (void)key;
    ALGOLIB_TODO();
}

bool avl_map_erase(AvlMap *m, const void *key) {
    (void)m;
    (void)key;
    ALGOLIB_TODO();
}

size_t avl_map_size(const AvlMap *m) {
    (void)m;
    ALGOLIB_TODO();
}

int avl_map_height(const AvlMap *m) {
    (void)m;
    ALGOLIB_TODO();
}

size_t avl_map_keys(const AvlMap *m, void *out) {
    (void)m;
    (void)out;
    ALGOLIB_TODO();
}

bool avl_map_min_key(const AvlMap *m, void *out) {
    (void)m;
    (void)out;
    ALGOLIB_TODO();
}

bool avl_map_max_key(const AvlMap *m, void *out) {
    (void)m;
    (void)out;
    ALGOLIB_TODO();
}
