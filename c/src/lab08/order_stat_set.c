/* LAB08 — tu implementación de <algolib/lab08/order_stat_set.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab08/order_stat_set.h>

#include <stdlib.h>
#include <string.h>

typedef struct OSNode {
    struct OSNode *left, *right;
    int height;          /* o prioridad, si haces un treap */
    size_t subtree;      /* AUMENTACIÓN: tamaño del subárbol */
    unsigned char key[];
} OSNode;

struct OrderStatSet {
    OSNode *root;
    size_t key_size;
    algolib_cmp cmp;
};

OrderStatSet *order_stat_set_new(size_t key_size, algolib_cmp cmp) {
    (void)key_size;
    (void)cmp;
    ALGOLIB_TODO();
}

void order_stat_set_free(OrderStatSet *s) {
    (void)s;
    ALGOLIB_TODO();
}

bool order_stat_set_insert(OrderStatSet *s, const void *key) {
    (void)s;
    (void)key;
    ALGOLIB_TODO();
}

bool order_stat_set_erase(OrderStatSet *s, const void *key) {
    (void)s;
    (void)key;
    ALGOLIB_TODO();
}

bool order_stat_set_contains(const OrderStatSet *s, const void *key) {
    (void)s;
    (void)key;
    ALGOLIB_TODO();
}

size_t order_stat_set_size(const OrderStatSet *s) {
    (void)s;
    ALGOLIB_TODO();
}

bool order_stat_set_select(const OrderStatSet *s, size_t k, void *out) {
    (void)s;
    (void)k;
    (void)out;
    ALGOLIB_TODO();
}

size_t order_stat_set_rank(const OrderStatSet *s, const void *key) {
    (void)s;
    (void)key;
    ALGOLIB_TODO();
}
