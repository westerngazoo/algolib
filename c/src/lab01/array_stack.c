/* LAB01 — tu implementación de <algolib/lab01/array_stack.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab01/array_stack.h>

#include <stdlib.h>
#include <string.h>

/* Representación sugerida (puedes cambiarla: es tuya).
 * Invariante: size <= cap; los primeros size*elem_size bytes de data son elementos válidos;
 *             data == NULL <=> cap == 0. */
struct ArrayStack {
    unsigned char *data;
    size_t elem_size;
    size_t size;
    size_t cap;
};

ArrayStack *array_stack_new(size_t elem_size) {
    (void)elem_size;
    ALGOLIB_TODO();
}

void array_stack_free(ArrayStack *s) {
    (void)s;
    ALGOLIB_TODO();
}

bool array_stack_push(ArrayStack *s, const void *elem) {
    (void)s;
    (void)elem;
    ALGOLIB_TODO();
}

bool array_stack_pop(ArrayStack *s, void *out) {
    (void)s;
    (void)out;
    ALGOLIB_TODO();
}

void *array_stack_top(const ArrayStack *s) {
    (void)s;
    ALGOLIB_TODO();
}

size_t array_stack_size(const ArrayStack *s) {
    (void)s;
    ALGOLIB_TODO();
}

size_t array_stack_capacity(const ArrayStack *s) {
    (void)s;
    ALGOLIB_TODO();
}

bool array_stack_empty(const ArrayStack *s) {
    (void)s;
    ALGOLIB_TODO();
}

void array_stack_clear(ArrayStack *s) {
    (void)s;
    ALGOLIB_TODO();
}
