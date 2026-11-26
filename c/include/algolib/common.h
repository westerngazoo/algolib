/* algolib — utilidades comunes para la versión en C. */
#ifndef ALGOLIB_COMMON_H
#define ALGOLIB_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Las funciones pendientes llaman ALGOLIB_TODO(). El runner de tests lo reporta
 * como [TODO] (no como [FAIL]). Equivalente a ALGOLIB_TODO() en C++ y todo!() en Rust.
 * _Noreturn: el compilador sabe que no regresa, así que no exige un `return`. */
_Noreturn void algolib_todo(const char *funcion);
#define ALGOLIB_TODO() algolib_todo(__func__)

/* Comparador al estilo de qsort: negativo si a < b, 0 si a == b, positivo si a > b.
 * Es la forma de C de escribir un "genérico con orden" (en C++: una plantilla con
 * operator<; en Rust: T: Ord). */
typedef int (*algolib_cmp)(const void *a, const void *b);

#endif
