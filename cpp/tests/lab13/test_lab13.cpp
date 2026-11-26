#include <algolib/lab13/range_trees.hpp>
#include <algolib/lab13/trie.hpp>

#include <algotest.hpp>

#include <algorithm>
#include <set>
#include <string>
#include <vector>

using namespace algolib;

TEST(trie_basico) {
    Trie t;
    CHECK_EQ(t.node_count(), 1u);
    for (auto w : {"casa", "cama", "camion", "cas", "perro"}) CHECK(t.insert(w));
    CHECK(!t.insert("casa"));
    CHECK_EQ(t.size(), 5u);
    CHECK(t.contains("cas"));
    CHECK(!t.contains("ca"));
    CHECK(t.starts_with("ca"));
    CHECK(!t.starts_with("gato"));
    CHECK(t.starts_with(""));
    CHECK_EQ(t.count_with_prefix("ca"), 4u);
    CHECK_EQ(t.count_with_prefix("cam"), 2u);
    CHECK_EQ(t.count_with_prefix(""), 5u);
    CHECK_EQ(t.count_with_prefix("x"), 0u);
}

TEST(trie_erase_poda) {
    Trie t;
    t.insert("abc");
    std::size_t con_abc = t.node_count();  // raíz + a + b + c = 4
    CHECK_EQ(con_abc, 4u);
    t.insert("abcde");
    CHECK_EQ(t.node_count(), 6u);
    CHECK(t.erase("abcde"));
    CHECK_EQ(t.node_count(), 4u);  // poda d, e
    CHECK(!t.erase("ab"));          // prefijo, no palabra
    CHECK(t.contains("abc"));
    CHECK(t.erase("abc"));
    CHECK_EQ(t.node_count(), 1u);
    CHECK_EQ(t.size(), 0u);
}

TEST(trie_aleatorio_vs_set) {
    algotest::Lcg rng(23);
    Trie t;
    std::set<std::string> o;
    for (int op = 0; op < 20000; ++op) {
        std::string w;
        for (auto n = 1 + rng.below(6); n--;) w += char('a' + rng.below(4));
        if (rng.below(3) != 0) CHECK_EQ(t.insert(w), o.insert(w).second);
        else CHECK_EQ(t.erase(w), o.erase(w) == 1);
        if (op % 200 == 0) {
            std::string p = w.substr(0, rng.below(w.size() + 1));
            std::size_t cnt = 0;
            for (auto& s : o) cnt += s.compare(0, p.size(), p) == 0;
            CHECK_EQ(t.count_with_prefix(p), cnt);
            CHECK_EQ(t.size(), o.size());
        }
    }
}

TEST(fenwick_vs_ingenuo) {
    algotest::Lcg rng(24);
    const std::size_t n = 1000;
    FenwickTree f(n);
    std::vector<std::int64_t> a(n, 0);
    CHECK_EQ(f.prefix_sum(0), 0);
    for (int op = 0; op < 20000; ++op) {
        if (rng.below(2)) {
            auto i = rng.below(n);
            auto d = rng.range(-1000, 1000);
            f.add(i, d);
            a[i] += d;
        } else {
            auto l = rng.below(n + 1), r = rng.below(n + 1);
            if (l > r) std::swap(l, r);
            std::int64_t s = 0;
            for (auto i = l; i < r; ++i) s += a[i];
            CHECK_EQ(f.range_sum(l, r), s);
        }
    }
}

TEST(segtree_vs_ingenuo) {
    algotest::Lcg rng(25);
    const std::size_t n = 777;  // no potencia de 2 a propósito
    std::vector<std::int64_t> a(n);
    for (auto& x : a) x = rng.range(-1'000'000, 1'000'000);
    SegmentTree st(a);
    for (int op = 0; op < 20000; ++op) {
        if (rng.below(2)) {
            auto i = rng.below(n);
            auto x = rng.range(-1'000'000, 1'000'000);
            st.set(i, x);
            a[i] = x;
        } else {
            auto l = rng.below(n + 1), r = rng.below(n + 1);
            if (l > r) std::swap(l, r);
            auto got = st.range_min(l, r);
            if (l == r) CHECK(!got.has_value());
            else CHECK_EQ(*got, *std::min_element(a.begin() + (long)l, a.begin() + (long)r));
        }
    }
    CHECK(!SegmentTree({}).range_min(0, 0).has_value());
}
