/* LAB 01 — ArrayStack: pila genérica sobre arreglo dinámico.  Notas: notas/L01
 *
 * Genérico al estilo de C: la pila guarda elementos de `elem_size` bytes y los copia
 * con memcpy. Tú defines `struct ArrayStack` en c/src/lab01/array_stack.c.
 *
 * Complejidad: push O(1) amortizado (duplicar la capacidad: 0 → 1 → 2 → 4 …);
 *              pop / top O(1).
 * Errores: C no tiene excepciones ni Option. Convención del curso:
 *   - funciones que pueden fallar devuelven bool (false = vacía / sin memoria);
 *   - funciones que devuelven un elemento devuelven un puntero, NULL si no hay.
 * Cuidado: array_stack_push(s, array_stack_top(s)) pasa un puntero a TU PROPIO bloque. */
#ifndef ALGOLIB_LAB01_ARRAY_STACK_H
#define ALGOLIB_LAB01_ARRAY_STACK_H

#include <algolib/common.h>

typedef struct ArrayStack ArrayStack;

ArrayStack *array_stack_new(size_t elem_size);
void array_stack_free(ArrayStack *s);
bool array_stack_push(ArrayStack *s, const void *elem);
bool array_stack_pop(ArrayStack *s, void *out);
void *array_stack_top(const ArrayStack *s);
size_t array_stack_size(const ArrayStack *s);
size_t array_stack_capacity(const ArrayStack *s);
bool array_stack_empty(const ArrayStack *s);
void array_stack_clear(ArrayStack *s);

/* Detalles:
 *   array_stack_new   NULL si no hay memoria. Capacidad inicial 0.
 *   array_stack_free  acepta NULL.
 *   array_stack_push  copia elem_size bytes desde elem; false si no hay memoria.
 *   array_stack_pop   false si está vacía; si out != NULL copia ahí el tope.
 *   array_stack_top   NULL si está vacía; el puntero vale hasta el siguiente push. */
#endif
