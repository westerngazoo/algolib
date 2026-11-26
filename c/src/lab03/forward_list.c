/* LAB03 — tu implementación de <algolib/lab03/forward_list.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab03/forward_list.h>

#include <stdlib.h>
#include <string.h>

/* Nodo con "flexible array member": el valor vive justo después del puntero. */
typedef struct FLNode {
    struct FLNode *next;
    unsigned char value[];
} FLNode;

struct ForwardList {
    FLNode *head;
    size_t elem_size;
    size_t size;
};

ForwardList *forward_list_new(size_t elem_size) {
    (void)elem_size;
    ALGOLIB_TODO();
}

void forward_list_free(ForwardList *l) {
    (void)l;
    ALGOLIB_TODO();
}

bool forward_list_push_front(ForwardList *l, const void *elem) {
    (void)l;
    (void)elem;
    ALGOLIB_TODO();
}

bool forward_list_pop_front(ForwardList *l, void *out) {
    (void)l;
    (void)out;
    ALGOLIB_TODO();
}

void *forward_list_front(const ForwardList *l) {
    (void)l;
    ALGOLIB_TODO();
}

size_t forward_list_size(const ForwardList *l) {
    (void)l;
    ALGOLIB_TODO();
}

void forward_list_reverse(ForwardList *l) {
    (void)l;
    ALGOLIB_TODO();
}

size_t forward_list_to_array(const ForwardList *l, void *out) {
    (void)l;
    (void)out;
    ALGOLIB_TODO();
}
