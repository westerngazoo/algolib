/* LAB 03 — ForwardList: lista simplemente ligada.  Notas: notas/L03
 * push_front/pop_front/front O(1); reverse O(n) tiempo y O(1) memoria extra.
 * forward_list_free debe ser ITERATIVO (el test usa 10^6 nodos). */
#ifndef ALGOLIB_LAB03_FORWARD_LIST_H
#define ALGOLIB_LAB03_FORWARD_LIST_H

#include <algolib/common.h>

typedef struct ForwardList ForwardList;

ForwardList *forward_list_new(size_t elem_size);
void forward_list_free(ForwardList *l);
bool forward_list_push_front(ForwardList *l, const void *elem);
bool forward_list_pop_front(ForwardList *l, void *out);
void *forward_list_front(const ForwardList *l);
size_t forward_list_size(const ForwardList *l);
void forward_list_reverse(ForwardList *l);
size_t forward_list_to_array(const ForwardList *l, void *out);

/* forward_list_to_array: copia los elementos de frente a final en `out` (que debe
 * tener espacio para size() elementos) y devuelve cuántos copió. */
#endif
