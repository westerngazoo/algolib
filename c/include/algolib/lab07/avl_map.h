/* LAB 07 — AvlMap: diccionario ordenado sobre árbol AVL.  Notas: notas/L07
 * Invariantes: BST por key_cmp; |h(izq) - h(der)| <= 1 con h(vacío) = 0, h(hoja) = 1.
 * insert/find/erase O(lg n) peor caso. Cota: h < 1.4405·lg(n+2) − 0.3277. */
#ifndef ALGOLIB_LAB07_AVL_MAP_H
#define ALGOLIB_LAB07_AVL_MAP_H

#include <algolib/common.h>

typedef struct AvlMap AvlMap;

AvlMap *avl_map_new(size_t key_size, size_t value_size, algolib_cmp key_cmp);
void avl_map_free(AvlMap *m);
bool avl_map_insert(AvlMap *m, const void *key, const void *value);
void *avl_map_find(const AvlMap *m, const void *key);
bool avl_map_erase(AvlMap *m, const void *key);
size_t avl_map_size(const AvlMap *m);
int avl_map_height(const AvlMap *m);
size_t avl_map_keys(const AvlMap *m, void *out);
bool avl_map_min_key(const AvlMap *m, void *out);
bool avl_map_max_key(const AvlMap *m, void *out);

/* avl_map_insert: inserta o reemplaza; true si la clave era nueva.
 * avl_map_keys:   copia las claves en orden (in-order) en out; devuelve cuántas. */
#endif
