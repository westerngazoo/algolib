#include "algotest.h"

#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>

enum { RES_OK = 0, RES_FAIL = 1, RES_TODO = 2 };

static jmp_buf entorno;
static char mensaje[512];
static int n_ok, n_fail, n_todo;

_Noreturn void algolib_todo(const char *funcion) {
    snprintf(mensaje, sizeof mensaje, "no implementado: %s", funcion);
    longjmp(entorno, RES_TODO);
}

_Noreturn void algotest_fail(const char *file, int line, const char *msg) {
    snprintf(mensaje, sizeof mensaje, "%s:%d  %s", file, line, msg);
    longjmp(entorno, RES_FAIL);
}

int algotest_run(const char *suite, const AlgoTest *tests) {
    printf("== %s (C) ==\n", suite);
    for (const AlgoTest *t = tests; t->name != NULL; t++) {
        switch (setjmp(entorno)) {
            case RES_OK:
                t->fn();
                n_ok++;
                printf("  [ OK ] %s\n", t->name);
                break;
            case RES_TODO:
                n_todo++;
                printf("  [TODO] %s  (%s)\n", t->name, mensaje);
                break;
            default:
                n_fail++;
                printf("  [FAIL] %s\n         %s\n", t->name, mensaje);
                break;
        }
        fflush(stdout);
    }
    printf("-- %d ok, %d fallidos, %d pendientes\n", n_ok, n_fail, n_todo);
    fflush(stdout);
    /* Si algo falló o está pendiente, un caso abortado pudo dejar memoria sin liberar:
     * salimos con _exit para que LeakSanitizer no reporte esas fugas "falsas". */
    if (n_fail || n_todo) _exit(1);
    return 0;
}
