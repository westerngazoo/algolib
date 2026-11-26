/* LAB 08 — OrderStatSet: conjunto ordenado con estadísticos de orden.  Notas: notas/L08
 * Árbol balanceado AUMENTADO con el tamaño de cada subárbol.
 *   select(k): k-ésimo menor (0-indexado) → false si k >= size
 *   rank(x):   número de elementos estrictamente menores que x */
#ifndef ALGOLIB_LAB08_ORDER_STAT_SET_H
#define ALGOLIB_LAB08_ORDER_STAT_SET_H

#include <algolib/common.h>

typedef struct OrderStatSet OrderStatSet;

OrderStatSet *order_stat_set_new(size_t key_size, algolib_cmp cmp);
void order_stat_set_free(OrderStatSet *s);
bool order_stat_set_insert(OrderStatSet *s, const void *key);
bool order_stat_set_erase(OrderStatSet *s, const void *key);
bool order_stat_set_contains(const OrderStatSet *s, const void *key);
size_t order_stat_set_size(const OrderStatSet *s);
bool order_stat_set_select(const OrderStatSet *s, size_t k, void *out);
size_t order_stat_set_rank(const OrderStatSet *s, const void *key);

#endif
