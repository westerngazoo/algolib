#include <algolib/lab08/order_stat_set.h>

#include "algotest.h"

static void ostat_basico(void) {
    OrderStatSet *s = order_stat_set_new(sizeof(int), cmp_int);
    int xs[] = {50, 20, 80, 10, 30, 70, 90};
    for (int i = 0; i < 7; i++) CHECK(order_stat_set_insert(s, &xs[i]));
    CHECK(!order_stat_set_insert(s, &(int){30}));
    CHECK_EQ(order_stat_set_size(s), 7);
    int x;
    CHECK(order_stat_set_select(s, 0, &x)); CHECK_EQ(x, 10);
    CHECK(order_stat_set_select(s, 3, &x)); CHECK_EQ(x, 50);
    CHECK(order_stat_set_select(s, 6, &x)); CHECK_EQ(x, 90);
    CHECK(!order_stat_set_select(s, 7, &x));
    CHECK_EQ(order_stat_set_rank(s, &(int){10}), 0);
    CHECK_EQ(order_stat_set_rank(s, &(int){55}), 4);     /* 10,20,30,50 < 55 */
    CHECK_EQ(order_stat_set_rank(s, &(int){100}), 7);
    CHECK(order_stat_set_erase(s, &(int){50}));
    CHECK(!order_stat_set_erase(s, &(int){50}));
    CHECK(order_stat_set_select(s, 3, &x)); CHECK_EQ(x, 70);
    CHECK(!order_stat_set_contains(s, &(int){50}));
    order_stat_set_free(s);
    order_stat_set_free(NULL);
}

static void ostat_aleatorio(void) {
    /* Oráculo: arreglo de presencia sobre [0, 5000]; select y rank por recorrido lineal. */
    static bool esta[5001];
    size_t n = 0;
    Lcg rng = {13};
    OrderStatSet *s = order_stat_set_new(sizeof(int), cmp_int);
    for (int op = 0; op < 30000; op++) {
        int x = (int)lcg_range(&rng, 0, 5000);
        if (lcg_below(&rng, 3) != 0) {
            CHECK_EQ(order_stat_set_insert(s, &x), !esta[x]);
            if (!esta[x]) n++;
            esta[x] = true;
        } else {
            CHECK_EQ(order_stat_set_erase(s, &x), esta[x]);
            if (esta[x]) n--;
            esta[x] = false;
        }
        CHECK_EQ(order_stat_set_size(s), n);
        if (op % 100 == 0) {
            for (int q = 0; q < 20; q++) {
                size_t k = lcg_below(&rng, n + 1);
                int got, want = -1;
                size_t c = 0;
                for (int v = 0; v <= 5000 && want < 0; v++)
                    if (esta[v] && c++ == k) want = v;
                CHECK_EQ(order_stat_set_select(s, k, &got), k < n);
                if (k < n) CHECK_EQ(got, want);
                int y = (int)lcg_range(&rng, -1, 5001);
                size_t r = 0;
                for (int v = 0; v <= 5000 && v < y; v++) r += esta[v];
                CHECK_EQ(order_stat_set_rank(s, &y), r);
            }
        }
    }
    order_stat_set_free(s);
}

int main(void) {
    return algotest_run("lab08", (AlgoTest[]){T(ostat_basico), T(ostat_aleatorio), {0}});
}
