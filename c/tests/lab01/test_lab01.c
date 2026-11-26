#include <algolib/lab01/array_stack.h>
#include <algolib/lab01/min_stack.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------- ArrayStack */

static void stack_lifo(void) {
    ArrayStack *s = array_stack_new(sizeof(int));
    CHECK(s != NULL);
    CHECK(array_stack_empty(s));
    for (int i = 0; i < 100; i++) CHECK(array_stack_push(s, &i));
    CHECK_EQ(array_stack_size(s), 100);
    for (int i = 99; i >= 0; i--) {
        CHECK_EQ(*(int *)array_stack_top(s), i);
        int x = -1;
        CHECK(array_stack_pop(s, &x));
        CHECK_EQ(x, i);
    }
    CHECK(array_stack_empty(s));
    array_stack_free(s);
}

static void stack_vacia(void) {
    ArrayStack *s = array_stack_new(sizeof(int));
    int x = 7;
    CHECK(!array_stack_pop(s, &x));
    CHECK_EQ(x, 7);                     /* no se escribe nada si está vacía */
    CHECK(array_stack_top(s) == NULL);
    CHECK(!array_stack_pop(s, NULL));
    array_stack_push(s, &x);
    CHECK(array_stack_pop(s, NULL));    /* out == NULL: solo descarta */
    CHECK(array_stack_empty(s));
    array_stack_free(s);
    array_stack_free(NULL);             /* free(NULL) debe ser válido, como en la libc */
}

static void stack_crecimiento_geometrico(void) {
    ArrayStack *s = array_stack_new(sizeof(int));
    size_t prev = array_stack_capacity(s), cambios = 0;
    CHECK_EQ(prev, 0);
    for (int i = 0; i < 1024; i++) {
        array_stack_push(s, &i);
        CHECK(array_stack_size(s) <= array_stack_capacity(s));
        if (array_stack_capacity(s) != prev) {
            CHECK(prev == 0 ? array_stack_capacity(s) == 1 : array_stack_capacity(s) == 2 * prev);
            cambios++;
            prev = array_stack_capacity(s);
        }
    }
    CHECK_EQ(cambios, 11);  /* 1, 2, 4, ..., 1024 */
    array_stack_free(s);
}

typedef struct {
    char nombre[40];
    double x;
    long id;
} Registro;

static void stack_elementos_grandes(void) {
    ArrayStack *s = array_stack_new(sizeof(Registro));
    for (long i = 0; i < 50; i++) {
        Registro r;
        memset(&r, 0, sizeof r);
        snprintf(r.nombre, sizeof r.nombre, "registro-%ld", i);
        r.x = (double)i / 2;
        r.id = i;
        array_stack_push(s, &r);
    }
    for (long i = 49; i >= 0; i--) {
        Registro r;
        CHECK(array_stack_pop(s, &r));
        char esperado[40];
        snprintf(esperado, sizeof esperado, "registro-%ld", i);
        CHECK(strcmp(r.nombre, esperado) == 0);
        CHECK(r.x == (double)i / 2);
        CHECK_EQ(r.id, i);
    }
    array_stack_free(s);
}

static void stack_alias_push_top(void) {
    /* El elemento a apilar está DENTRO del bloque de la pila. Si al crecer liberas el
     * bloque viejo antes de copiar el elemento, lees memoria liberada (ASan lo detecta). */
    ArrayStack *s = array_stack_new(sizeof(Registro));
    Registro r;
    memset(&r, 0, sizeof r);
    strcpy(r.nombre, "original");
    r.id = 42;
    array_stack_push(s, &r);
    for (int i = 0; i < 20; i++) CHECK(array_stack_push(s, array_stack_top(s)));
    CHECK_EQ(array_stack_size(s), 21);
    for (int i = 0; i < 21; i++) {
        Registro t;
        array_stack_pop(s, &t);
        CHECK(strcmp(t.nombre, "original") == 0);
        CHECK_EQ(t.id, 42);
    }
    array_stack_free(s);
}

static void stack_clear(void) {
    ArrayStack *s = array_stack_new(sizeof(int));
    for (int i = 0; i < 10; i++) array_stack_push(s, &i);
    array_stack_clear(s);
    CHECK(array_stack_empty(s));
    CHECK(array_stack_top(s) == NULL);
    int x = 5;
    array_stack_push(s, &x);           /* sigue siendo usable */
    CHECK_EQ(*(int *)array_stack_top(s), 5);
    array_stack_free(s);
}

static void stack_aleatorio_vs_arreglo(void) {
    Lcg rng = {1};
    ArrayStack *s = array_stack_new(sizeof(long));
    long *oraculo = malloc(20000 * sizeof *oraculo);
    size_t n = 0;
    for (int op = 0; op < 20000; op++) {
        if (n == 0 || lcg_below(&rng, 3) != 0) {
            long x = (long)lcg_range(&rng, -1000, 1000);
            array_stack_push(s, &x);
            oraculo[n++] = x;
        } else {
            CHECK_EQ(*(long *)array_stack_top(s), oraculo[n - 1]);
            long x;
            array_stack_pop(s, &x);
            CHECK_EQ(x, oraculo[--n]);
        }
        CHECK_EQ(array_stack_size(s), n);
    }
    free(oraculo);
    array_stack_free(s);
}

/* ---------------------------------------------------------------- MinStack */

static void minstack_basico(void) {
    MinStack *m = min_stack_new(sizeof(int), cmp_int);
    CHECK(min_stack_min(m) == NULL);
    int xs[] = {5, 3, 7, 3, 1};
    for (int i = 0; i < 5; i++) min_stack_push(m, &xs[i]);
    CHECK_EQ(*(int *)min_stack_min(m), 1);
    min_stack_pop(m, NULL);
    CHECK_EQ(*(int *)min_stack_min(m), 3);
    min_stack_pop(m, NULL);              /* quita un 3 duplicado: el mínimo sigue siendo 3 */
    CHECK_EQ(*(int *)min_stack_min(m), 3);
    min_stack_pop(m, NULL);
    CHECK_EQ(*(int *)min_stack_min(m), 3);
    min_stack_pop(m, NULL);
    CHECK_EQ(*(int *)min_stack_min(m), 5);
    CHECK_EQ(*(int *)min_stack_top(m), 5);
    int x;
    CHECK(min_stack_pop(m, &x));
    CHECK_EQ(x, 5);
    CHECK_EQ(min_stack_size(m), 0);
    CHECK(min_stack_min(m) == NULL);
    CHECK(!min_stack_pop(m, NULL));
    min_stack_free(m);
}

static void minstack_aleatorio(void) {
    Lcg rng = {7};
    MinStack *m = min_stack_new(sizeof(int), cmp_int);
    int *oraculo = malloc(20000 * sizeof *oraculo);
    size_t n = 0;
    for (int op = 0; op < 20000; op++) {
        if (n == 0 || lcg_below(&rng, 2) == 0) {
            int x = (int)lcg_range(&rng, -50, 50);
            min_stack_push(m, &x);
            oraculo[n++] = x;
        } else {
            min_stack_pop(m, NULL);
            n--;
        }
        CHECK_EQ(min_stack_size(m), n);
        if (n > 0) {
            int mn = oraculo[0];
            for (size_t i = 1; i < n; i++)
                if (oraculo[i] < mn) mn = oraculo[i];
            CHECK_EQ(*(int *)min_stack_min(m), mn);
        }
    }
    free(oraculo);
    min_stack_free(m);
}

int main(void) {
    return algotest_run("lab01", (AlgoTest[]){
        T(stack_lifo), T(stack_vacia), T(stack_crecimiento_geometrico),
        T(stack_elementos_grandes), T(stack_alias_push_top), T(stack_clear),
        T(stack_aleatorio_vs_arreglo), T(minstack_basico), T(minstack_aleatorio), {0}});
}
