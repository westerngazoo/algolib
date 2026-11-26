#include <algolib/lab13/range_trees.h>
#include <algolib/lab13/trie.h>

#include "algotest.h"

#include <stdlib.h>
#include <string.h>

static void trie_basico(void) {
    Trie *t = trie_new();
    CHECK_EQ(trie_node_count(t), 1);
    const char *ws[] = {"casa", "cama", "camion", "cas", "perro"};
    for (int i = 0; i < 5; i++) CHECK(trie_insert(t, ws[i]));
    CHECK(!trie_insert(t, "casa"));
    CHECK_EQ(trie_size(t), 5);
    CHECK(trie_contains(t, "cas"));
    CHECK(!trie_contains(t, "ca"));
    CHECK(trie_starts_with(t, "ca"));
    CHECK(!trie_starts_with(t, "gato"));
    CHECK(trie_starts_with(t, ""));
    CHECK_EQ(trie_count_with_prefix(t, "ca"), 4);
    CHECK_EQ(trie_count_with_prefix(t, "cam"), 2);
    CHECK_EQ(trie_count_with_prefix(t, ""), 5);
    CHECK_EQ(trie_count_with_prefix(t, "x"), 0);
    trie_free(t);
    trie_free(NULL);
}

static void trie_erase_poda(void) {
    Trie *t = trie_new();
    trie_insert(t, "abc");
    CHECK_EQ(trie_node_count(t), 4);           /* raíz + a + b + c */
    trie_insert(t, "abcde");
    CHECK_EQ(trie_node_count(t), 6);
    CHECK(trie_erase(t, "abcde"));
    CHECK_EQ(trie_node_count(t), 4);           /* poda d y e */
    CHECK(!trie_erase(t, "ab"));               /* prefijo, no palabra */
    CHECK(trie_contains(t, "abc"));
    CHECK(trie_erase(t, "abc"));
    CHECK_EQ(trie_node_count(t), 1);
    CHECK_EQ(trie_size(t), 0);
    trie_free(t);
}

static void trie_aleatorio(void) {
    /* Oráculo: palabras de 1..6 letras en {a,b,c,d} → índice único en base 5 (≤ 5^6). */
    enum { MAXW = 15625 };
    static bool esta[MAXW];
    size_t n = 0;
    Lcg rng = {23};
    Trie *t = trie_new();
    for (int op = 0; op < 20000; op++) {
        char w[7] = {0};
        size_t len = 1 + lcg_below(&rng, 6), idx = 0;
        for (size_t i = 0; i < len; i++) {
            w[i] = (char)('a' + lcg_below(&rng, 4));
            idx = idx * 5 + (size_t)(w[i] - 'a' + 1);
        }
        if (lcg_below(&rng, 3) != 0) {
            CHECK_EQ(trie_insert(t, w), !esta[idx]);
            if (!esta[idx]) n++;
            esta[idx] = true;
        } else {
            CHECK_EQ(trie_erase(t, w), esta[idx]);
            if (esta[idx]) n--;
            esta[idx] = false;
        }
        if (op % 200 == 0) {
            size_t plen = lcg_below(&rng, len + 1), cnt = 0;
            /* cuenta palabras del oráculo que empiezan con w[0..plen) */
            for (size_t code = 0; code < MAXW; code++) {
                if (!esta[code]) continue;
                char s[7] = {0};
                size_t c = code, k = 0;
                char rev[7];
                while (c) { rev[k++] = (char)('a' + c % 5 - 1); c /= 5; }
                for (size_t i = 0; i < k; i++) s[i] = rev[k - 1 - i];
                if (strncmp(s, w, plen) == 0) cnt++;
            }
            char p[7] = {0};
            memcpy(p, w, plen);
            CHECK_EQ(trie_count_with_prefix(t, p), cnt);
            CHECK_EQ(trie_size(t), n);
        }
    }
    trie_free(t);
}

static void fenwick_vs_ingenuo(void) {
    Lcg rng = {24};
    size_t n = 1000;
    FenwickTree *f = fenwick_new(n);
    int64_t a[1000] = {0};
    CHECK_EQ(fenwick_prefix_sum(f, 0), 0);
    for (int op = 0; op < 20000; op++) {
        if (lcg_below(&rng, 2)) {
            size_t i = lcg_below(&rng, n);
            int64_t d = lcg_range(&rng, -1000, 1000);
            fenwick_add(f, i, d);
            a[i] += d;
        } else {
            size_t l = lcg_below(&rng, n + 1), r = lcg_below(&rng, n + 1);
            if (l > r) { size_t t = l; l = r; r = t; }
            int64_t s = 0;
            for (size_t i = l; i < r; i++) s += a[i];
            CHECK_EQ(fenwick_range_sum(f, l, r), s);
        }
    }
    fenwick_free(f);
    fenwick_free(NULL);
}

static void segtree_vs_ingenuo(void) {
    Lcg rng = {25};
    size_t n = 777;                              /* no potencia de 2 a propósito */
    int64_t a[777];
    for (size_t i = 0; i < n; i++) a[i] = lcg_range(&rng, -1000000, 1000000);
    SegmentTree *st = segtree_new(a, n);
    for (int op = 0; op < 20000; op++) {
        if (lcg_below(&rng, 2)) {
            size_t i = lcg_below(&rng, n);
            int64_t x = lcg_range(&rng, -1000000, 1000000);
            segtree_set(st, i, x);
            a[i] = x;
        } else {
            size_t l = lcg_below(&rng, n + 1), r = lcg_below(&rng, n + 1);
            if (l > r) { size_t t = l; l = r; r = t; }
            int64_t got;
            bool hay = segtree_range_min(st, l, r, &got);
            CHECK_EQ(hay, l < r);
            if (hay) {
                int64_t mn = a[l];
                for (size_t i = l + 1; i < r; i++) if (a[i] < mn) mn = a[i];
                CHECK_EQ(got, mn);
            }
        }
    }
    segtree_free(st);
    SegmentTree *vacio = segtree_new(NULL, 0);
    int64_t x;
    CHECK(!segtree_range_min(vacio, 0, 0, &x));
    segtree_free(vacio);
    segtree_free(NULL);
}

int main(void) {
    return algotest_run("lab13", (AlgoTest[]){
        T(trie_basico), T(trie_erase_poda), T(trie_aleatorio), T(fenwick_vs_ingenuo), T(segtree_vs_ingenuo), {0}});
}
