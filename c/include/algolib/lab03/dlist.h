/* LAB 03 — DList: lista doblemente ligada (deque sobre nodos). Todo O(1) salvo to_array.
 * Invariante: size == 0 <=> head == tail == NULL; head->prev == NULL; tail->next == NULL;
 *             para todo nodo n con n->next != NULL: n->next->prev == n. */
#ifndef ALGOLIB_LAB03_DLIST_H
#define ALGOLIB_LAB03_DLIST_H

#include <algolib/common.h>

typedef struct DList DList;

DList *dlist_new(size_t elem_size);
void dlist_free(DList *d);
bool dlist_push_front(DList *d, const void *elem);
bool dlist_push_back(DList *d, const void *elem);
bool dlist_pop_front(DList *d, void *out);
bool dlist_pop_back(DList *d, void *out);
void *dlist_front(const DList *d);
void *dlist_back(const DList *d);
size_t dlist_size(const DList *d);
size_t dlist_to_array(const DList *d, void *out);
size_t dlist_to_array_reversed(const DList *d, void *out);

#endif
