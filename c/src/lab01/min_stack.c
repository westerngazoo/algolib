/* LAB01 — tu implementación de <algolib/lab01/min_stack.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab01/min_stack.h>
#include <algolib/lab01/array_stack.h>

#include <stdlib.h>
#include <string.h>

/* Sugerencia: compón dos ArrayStack (elementos y mínimos). Enuncia el invariante. */
struct MinStack {
    ArrayStack *items;
    ArrayStack *mins;
    algolib_cmp cmp;
};

MinStack *min_stack_new(size_t elem_size, algolib_cmp cmp) {
    (void)elem_size;
    (void)cmp;
    ALGOLIB_TODO();
}

void min_stack_free(MinStack *s) {
    (void)s;
    ALGOLIB_TODO();
}

bool min_stack_push(MinStack *s, const void *elem) {
    (void)s;
    (void)elem;
    ALGOLIB_TODO();
}

bool min_stack_pop(MinStack *s, void *out) {
    (void)s;
    (void)out;
    ALGOLIB_TODO();
}

void *min_stack_top(const MinStack *s) {
    (void)s;
    ALGOLIB_TODO();
}

void *min_stack_min(const MinStack *s) {
    (void)s;
    ALGOLIB_TODO();
}

size_t min_stack_size(const MinStack *s) {
    (void)s;
    ALGOLIB_TODO();
}
