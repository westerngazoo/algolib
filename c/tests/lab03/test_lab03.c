#include <algolib/lab03/dlist.h>
#include <algolib/lab03/forward_list.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

static void flist_lifo(void) {
    ForwardList *l = forward_list_new(sizeof(int));
    CHECK_EQ(forward_list_size(l), 0);
    CHECK(forward_list_front(l) == NULL);
    for (int i = 0; i < 5; i++) forward_list_push_front(l, &i);
    int a[5];
    CHECK_EQ(forward_list_to_array(l, a), 5);
    int e[] = {4, 3, 2, 1, 0};
    CHECK(memcmp(a, e, sizeof e) == 0);
    int x;
    CHECK(forward_list_pop_front(l, &x));
    CHECK_EQ(x, 4);
    CHECK_EQ(*(int *)forward_list_front(l), 3);
    forward_list_free(l);
    forward_list_free(NULL);
}

static void flist_vacia(void) {
    ForwardList *l = forward_list_new(sizeof(int));
    CHECK(!forward_list_pop_front(l, NULL));
    forward_list_reverse(l);
    CHECK_EQ(forward_list_size(l), 0);
    forward_list_free(l);
}

static void flist_reverse(void) {
    ForwardList *l = forward_list_new(sizeof(int));
    for (int i = 0; i < 6; i++) forward_list_push_front(l, &i);   /* 5 4 3 2 1 0 */
    forward_list_reverse(l);
    int a[6], e[] = {0, 1, 2, 3, 4, 5};
    forward_list_to_array(l, a);
    CHECK(memcmp(a, e, sizeof e) == 0);
    forward_list_push_front(l, &(int){9});
    CHECK_EQ(forward_list_size(l), 7);
    CHECK_EQ(*(int *)forward_list_front(l), 9);
    forward_list_free(l);
}

static void flist_un_millon(void) {
    /* Un free recursivo desbordaría la pila: debe ser iterativo. */
    ForwardList *l = forward_list_new(sizeof(int));
    for (int i = 0; i < 1000000; i++) forward_list_push_front(l, &i);
    CHECK_EQ(forward_list_size(l), 1000000);
    forward_list_free(l);
}

static void dlist_basico(void) {
    DList *d = dlist_new(sizeof(int));
    dlist_push_back(d, &(int){2});
    dlist_push_front(d, &(int){1});
    dlist_push_back(d, &(int){3});
    int a[3], e[] = {1, 2, 3}, er[] = {3, 2, 1};
    CHECK_EQ(dlist_to_array(d, a), 3);
    CHECK(memcmp(a, e, sizeof e) == 0);
    CHECK_EQ(dlist_to_array_reversed(d, a), 3);
    CHECK(memcmp(a, er, sizeof er) == 0);
    CHECK_EQ(*(int *)dlist_front(d), 1);
    CHECK_EQ(*(int *)dlist_back(d), 3);
    int x;
    CHECK(dlist_pop_front(d, &x)); CHECK_EQ(x, 1);
    CHECK(dlist_pop_back(d, &x)); CHECK_EQ(x, 3);
    CHECK_EQ(*(int *)dlist_front(d), 2);
    CHECK_EQ(*(int *)dlist_back(d), 2);
    CHECK(dlist_pop_back(d, NULL));
    CHECK_EQ(dlist_size(d), 0);
    CHECK(!dlist_pop_front(d, NULL));
    CHECK(dlist_back(d) == NULL);
    dlist_push_front(d, &(int){9});        /* reutilizable tras vaciarse */
    CHECK_EQ(dlist_to_array_reversed(d, a), 1);
    CHECK_EQ(a[0], 9);
    dlist_free(d);
    dlist_free(NULL);
}

static void dlist_aleatorio_vs_deque(void) {
    /* Oráculo: un arreglo grande con el "frente" a la mitad. */
    enum { CAP = 50000 };
    int *buf = malloc(CAP * sizeof *buf);
    size_t ini = CAP / 2, fin = CAP / 2;     /* elementos en buf[ini, fin) */
    Lcg rng = {3};
    DList *d = dlist_new(sizeof(int));
    for (int op = 0; op < 20000; op++) {
        int x = (int)lcg_range(&rng, 0, 999), y;
        switch (lcg_below(&rng, 4)) {
            case 0: dlist_push_front(d, &x); buf[--ini] = x; break;
            case 1: dlist_push_back(d, &x); buf[fin++] = x; break;
            case 2:
                if (fin > ini) { CHECK(dlist_pop_front(d, &y)); CHECK_EQ(y, buf[ini++]); }
                else CHECK(!dlist_pop_front(d, &y));
                break;
            default:
                if (fin > ini) { CHECK(dlist_pop_back(d, &y)); CHECK_EQ(y, buf[--fin]); }
                else CHECK(!dlist_pop_back(d, &y));
        }
        CHECK_EQ(dlist_size(d), fin - ini);
    }
    size_t n = fin - ini;
    int *a = malloc((n + 1) * sizeof *a);
    dlist_to_array(d, a);
    CHECK(n == 0 || memcmp(a, buf + ini, n * sizeof *a) == 0);
    dlist_to_array_reversed(d, a);
    for (size_t i = 0; i < n; i++) CHECK_EQ(a[i], buf[fin - 1 - i]);
    free(a);
    free(buf);
    dlist_free(d);
}

int main(void) {
    return algotest_run("lab03", (AlgoTest[]){
        T(flist_lifo), T(flist_vacia), T(flist_reverse), T(flist_un_millon),
        T(dlist_basico), T(dlist_aleatorio_vs_deque), {0}});
}
