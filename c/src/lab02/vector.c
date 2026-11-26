/* LAB02 — tu implementación de <algolib/lab02/vector.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab02/vector.h>

#include <stdlib.h>
#include <string.h>

struct Vector {
    unsigned char *data;
    size_t elem_size;
    size_t size;
    size_t cap;
};

Vector *vector_new(size_t elem_size) {
    (void)elem_size;
    ALGOLIB_TODO();
}

void vector_free(Vector *v) {
    (void)v;
    ALGOLIB_TODO();
}

size_t vector_size(const Vector *v) {
    (void)v;
    ALGOLIB_TODO();
}

size_t vector_capacity(const Vector *v) {
    (void)v;
    ALGOLIB_TODO();
}

void *vector_get(const Vector *v, size_t i) {
    (void)v;
    (void)i;
    ALGOLIB_TODO();
}

void *vector_data(const Vector *v) {
    (void)v;
    ALGOLIB_TODO();
}

bool vector_push_back(Vector *v, const void *elem) {
    (void)v;
    (void)elem;
    ALGOLIB_TODO();
}

bool vector_pop_back(Vector *v, void *out) {
    (void)v;
    (void)out;
    ALGOLIB_TODO();
}

bool vector_insert(Vector *v, size_t i, const void *elem) {
    (void)v;
    (void)i;
    (void)elem;
    ALGOLIB_TODO();
}

bool vector_erase(Vector *v, size_t i, void *out) {
    (void)v;
    (void)i;
    (void)out;
    ALGOLIB_TODO();
}

bool vector_reserve(Vector *v, size_t n) {
    (void)v;
    (void)n;
    ALGOLIB_TODO();
}

bool vector_shrink_to_fit(Vector *v) {
    (void)v;
    ALGOLIB_TODO();
}

void vector_clear(Vector *v) {
    (void)v;
    ALGOLIB_TODO();
}
