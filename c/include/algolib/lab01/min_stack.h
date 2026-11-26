/* LAB 01 — MinStack: pila con min() en O(1) peor caso (Problem Set 1, ej. 8).
 * Compón tu ArrayStack. El comparador decide qué es "menor". */
#ifndef ALGOLIB_LAB01_MIN_STACK_H
#define ALGOLIB_LAB01_MIN_STACK_H

#include <algolib/common.h>

typedef struct MinStack MinStack;

MinStack *min_stack_new(size_t elem_size, algolib_cmp cmp);
void min_stack_free(MinStack *s);
bool min_stack_push(MinStack *s, const void *elem);
bool min_stack_pop(MinStack *s, void *out);
void *min_stack_top(const MinStack *s);
void *min_stack_min(const MinStack *s);
size_t min_stack_size(const MinStack *s);

#endif
