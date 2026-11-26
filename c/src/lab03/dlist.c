/* LAB03 — tu implementación de <algolib/lab03/dlist.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab03/dlist.h>

#include <stdlib.h>
#include <string.h>

typedef struct DNode {
    struct DNode *prev, *next;
    unsigned char value[];
} DNode;

struct DList {
    DNode *head, *tail;
    size_t elem_size;
    size_t size;
};

DList *dlist_new(size_t elem_size) {
    (void)elem_size;
    ALGOLIB_TODO();
}

void dlist_free(DList *d) {
    (void)d;
    ALGOLIB_TODO();
}

bool dlist_push_front(DList *d, const void *elem) {
    (void)d;
    (void)elem;
    ALGOLIB_TODO();
}

bool dlist_push_back(DList *d, const void *elem) {
    (void)d;
    (void)elem;
    ALGOLIB_TODO();
}

bool dlist_pop_front(DList *d, void *out) {
    (void)d;
    (void)out;
    ALGOLIB_TODO();
}

bool dlist_pop_back(DList *d, void *out) {
    (void)d;
    (void)out;
    ALGOLIB_TODO();
}

void *dlist_front(const DList *d) {
    (void)d;
    ALGOLIB_TODO();
}

void *dlist_back(const DList *d) {
    (void)d;
    ALGOLIB_TODO();
}

size_t dlist_size(const DList *d) {
    (void)d;
    ALGOLIB_TODO();
}

size_t dlist_to_array(const DList *d, void *out) {
    (void)d;
    (void)out;
    ALGOLIB_TODO();
}

size_t dlist_to_array_reversed(const DList *d, void *out) {
    (void)d;
    (void)out;
    ALGOLIB_TODO();
}
