#include <algolib/lab12/dp.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

static void fibonacci_valores(void) {
    CHECK(fibonacci(0) == 0);
    CHECK(fibonacci(1) == 1);
    CHECK(fibonacci(10) == 55);
    CHECK(fibonacci(90) == 2880067194370816120ULL);
    CHECK(fibonacci(93) == 12200160415121876738ULL);
}

static void lcs_clasicos(void) {
    CHECK_EQ(lcs_length("ABCBDAB", "BDCABA"), 4);    /* CLRS 15.4 */
    CHECK_EQ(lcs_length("", "abc"), 0);
    CHECK_EQ(lcs_length("abc", "abc"), 3);
    CHECK_EQ(lcs_length("abc", "def"), 0);
}

static void edit_distance_clasicos(void) {
    CHECK_EQ(edit_distance("kitten", "sitting"), 3);
    CHECK_EQ(edit_distance("", "abc"), 3);
    CHECK_EQ(edit_distance("intention", "execution"), 5);
    CHECK_EQ(edit_distance("same", "same"), 0);
}

/* Oráculos recursivos exponenciales sobre prefijos a[0..i) y b[0..j). */
static size_t lcs_rec(const char *a, size_t i, const char *b, size_t j) {
    if (i == 0 || j == 0) return 0;
    if (a[i - 1] == b[j - 1]) return 1 + lcs_rec(a, i - 1, b, j - 1);
    size_t x = lcs_rec(a, i - 1, b, j), y = lcs_rec(a, i, b, j - 1);
    return x > y ? x : y;
}
static size_t ed_rec(const char *a, size_t i, const char *b, size_t j) {
    if (i == 0) return j;
    if (j == 0) return i;
    size_t x = ed_rec(a, i - 1, b, j) + 1, y = ed_rec(a, i, b, j - 1) + 1;
    size_t z = ed_rec(a, i - 1, b, j - 1) + (a[i - 1] != b[j - 1]);
    size_t m = x < y ? x : y;
    return m < z ? m : z;
}

static void lcs_y_edit_aleatorio(void) {
    Lcg rng = {20};
    for (int t = 0; t < 40; t++) {
        char a[9] = {0}, b[9] = {0};
        size_t na = lcg_below(&rng, 8), nb = lcg_below(&rng, 8);
        for (size_t i = 0; i < na; i++) a[i] = (char)('a' + lcg_below(&rng, 3));
        for (size_t i = 0; i < nb; i++) b[i] = (char)('a' + lcg_below(&rng, 3));
        CHECK_EQ(lcs_length(a, b), lcs_rec(a, na, b, nb));
        CHECK_EQ(edit_distance(a, b), ed_rec(a, na, b, nb));
    }
}

static void knapsack_vs_subconjuntos(void) {
    size_t w0[] = {1, 3, 4, 5};
    uint64_t v0[] = {1, 4, 5, 7};
    CHECK_EQ(knapsack_01(w0, v0, 4, 7), 9);
    CHECK_EQ(knapsack_01(NULL, NULL, 0, 10), 0);
    Lcg rng = {21};
    for (int t = 0; t < 40; t++) {
        size_t n = lcg_below(&rng, 13), cap = lcg_below(&rng, 50), w[12];
        uint64_t v[12], best = 0;
        for (size_t i = 0; i < n; i++) { w[i] = 1 + lcg_below(&rng, 20); v[i] = lcg_below(&rng, 100); }
        for (uint32_t mask = 0; mask < (1u << n); mask++) {
            size_t ws = 0;
            uint64_t vs = 0;
            for (size_t i = 0; i < n; i++)
                if (mask >> i & 1) { ws += w[i]; vs += v[i]; }
            if (ws <= cap && vs > best) best = vs;
        }
        CHECK_EQ(knapsack_01(w, v, n, cap), best);
    }
}

static void lis_vs_cuadratico(void) {
    int64_t a0[] = {10, 9, 2, 5, 3, 7, 101, 18}, a1[] = {7, 7, 7};
    CHECK_EQ(lis_length(a0, 8), 4);
    CHECK_EQ(lis_length(NULL, 0), 0);
    CHECK_EQ(lis_length(a1, 3), 1);                 /* estrictamente creciente */
    Lcg rng = {22};
    int64_t a[300];
    size_t L[300];
    for (int t = 0; t < 30; t++) {
        size_t n = lcg_below(&rng, 300), best = 0;
        for (size_t i = 0; i < n; i++) a[i] = lcg_range(&rng, -50, 50);
        for (size_t i = 0; i < n; i++) {
            L[i] = 1;
            for (size_t j = 0; j < i; j++)
                if (a[j] < a[i] && L[j] + 1 > L[i]) L[i] = L[j] + 1;
            if (L[i] > best) best = L[i];
        }
        CHECK_EQ(lis_length(a, n), best);
    }
    size_t n = 1000000;
    int64_t *g = malloc(n * sizeof *g);
    for (size_t i = 0; i < n; i++) g[i] = (int64_t)i;
    CHECK_EQ(lis_length(g, n), n);                  /* O(n²) no terminaría a tiempo */
    free(g);
}

static void coin_change(void) {
    size_t out = 0;
    size_t c1[] = {1, 5, 6, 9}, c2[] = {1, 2, 5}, c3[] = {2}, c4[] = {186, 419, 83, 408};
    CHECK(coin_change_min(c1, 4, 11, &out)); CHECK_EQ(out, 2);    /* 5 + 6 (greedy daría 3) */
    CHECK(coin_change_min(c2, 3, 0, &out)); CHECK_EQ(out, 0);
    CHECK(!coin_change_min(c3, 1, 3, &out));
    CHECK(coin_change_min(c4, 4, 6249, &out)); CHECK_EQ(out, 20);
}

int main(void) {
    return algotest_run("lab12", (AlgoTest[]){
        T(fibonacci_valores), T(lcs_clasicos), T(edit_distance_clasicos), T(lcs_y_edit_aleatorio),
        T(knapsack_vs_subconjuntos), T(lis_vs_cuadratico), T(coin_change), {0}});
}
