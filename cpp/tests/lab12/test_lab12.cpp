#include <algolib/lab12/dp.hpp>

#include <algotest.hpp>

#include <algorithm>
#include <functional>

using namespace algolib;

TEST(fibonacci_valores) {
    CHECK_EQ(fibonacci(0), 0u);
    CHECK_EQ(fibonacci(1), 1u);
    CHECK_EQ(fibonacci(10), 55u);
    CHECK_EQ(fibonacci(90), 2880067194370816120ULL);
    CHECK_EQ(fibonacci(93), 12200160415121876738ULL);
}

TEST(lcs_clasicos) {
    CHECK_EQ(lcs_length("ABCBDAB", "BDCABA"), 4u);  // CLRS 15.4
    CHECK_EQ(lcs_length("", "abc"), 0u);
    CHECK_EQ(lcs_length("abc", "abc"), 3u);
    CHECK_EQ(lcs_length("abc", "def"), 0u);
}

TEST(edit_distance_clasicos) {
    CHECK_EQ(edit_distance("kitten", "sitting"), 3u);
    CHECK_EQ(edit_distance("", "abc"), 3u);
    CHECK_EQ(edit_distance("intention", "execution"), 5u);
    CHECK_EQ(edit_distance("same", "same"), 0u);
}

TEST(lcs_y_edit_aleatorio_vs_recursion) {
    // Oráculos recursivos exponenciales sobre cadenas cortas.
    std::function<std::size_t(const std::string&, const std::string&)> lcs = [&](const std::string& a, const std::string& b) -> std::size_t {
        if (a.empty() || b.empty()) return 0;
        if (a.back() == b.back()) return 1 + lcs(a.substr(0, a.size() - 1), b.substr(0, b.size() - 1));
        return std::max(lcs(a.substr(0, a.size() - 1), b), lcs(a, b.substr(0, b.size() - 1)));
    };
    std::function<std::size_t(const std::string&, const std::string&)> ed = [&](const std::string& a, const std::string& b) -> std::size_t {
        if (a.empty()) return b.size();
        if (b.empty()) return a.size();
        auto a2 = a.substr(0, a.size() - 1), b2 = b.substr(0, b.size() - 1);
        return std::min({ed(a2, b) + 1, ed(a, b2) + 1, ed(a2, b2) + (a.back() != b.back())});
    };
    algotest::Lcg rng(20);
    for (int t = 0; t < 40; ++t) {
        std::string a, b;
        for (auto n = rng.below(8); n--;) a += char('a' + rng.below(3));
        for (auto n = rng.below(8); n--;) b += char('a' + rng.below(3));
        CHECK_EQ(lcs_length(a, b), lcs(a, b));
        CHECK_EQ(edit_distance(a, b), ed(a, b));
    }
}

TEST(knapsack_vs_subconjuntos) {
    CHECK_EQ(knapsack_01({1, 3, 4, 5}, {1, 4, 5, 7}, 7), 9u);
    CHECK_EQ(knapsack_01({}, {}, 10), 0u);
    algotest::Lcg rng(21);
    for (int t = 0; t < 40; ++t) {
        std::size_t n = rng.below(13), W = rng.below(50);
        std::vector<std::size_t> w(n);
        std::vector<std::uint64_t> v(n);
        for (std::size_t i = 0; i < n; ++i) { w[i] = 1 + rng.below(20); v[i] = rng.below(100); }
        std::uint64_t best = 0;
        for (std::uint32_t mask = 0; mask < (1u << n); ++mask) {
            std::size_t ws = 0; std::uint64_t vs = 0;
            for (std::size_t i = 0; i < n; ++i) if (mask >> i & 1) { ws += w[i]; vs += v[i]; }
            if (ws <= W) best = std::max(best, vs);
        }
        CHECK_EQ(knapsack_01(w, v, W), best);
    }
}

TEST(lis_vs_cuadratico) {
    CHECK_EQ(lis_length({10, 9, 2, 5, 3, 7, 101, 18}), 4u);
    CHECK_EQ(lis_length({}), 0u);
    CHECK_EQ(lis_length({7, 7, 7}), 1u);  // estrictamente creciente
    algotest::Lcg rng(22);
    for (int t = 0; t < 30; ++t) {
        std::vector<std::int64_t> a(rng.below(300));
        for (auto& x : a) x = rng.range(-50, 50);
        std::vector<std::size_t> L(a.size(), 1);
        std::size_t best = 0;
        for (std::size_t i = 0; i < a.size(); ++i) {
            for (std::size_t j = 0; j < i; ++j) if (a[j] < a[i]) L[i] = std::max(L[i], L[j] + 1);
            best = std::max(best, L[i]);
        }
        CHECK_EQ(lis_length(a), best);
    }
    std::vector<std::int64_t> grande(1'000'000);
    for (std::size_t i = 0; i < grande.size(); ++i) grande[i] = (std::int64_t)i;
    CHECK_EQ(lis_length(grande), grande.size());  // O(n²) no terminaría a tiempo
}

TEST(coin_change) {
    CHECK_EQ(*coin_change_min({1, 5, 6, 9}, 11), 2u);   // 5 + 6 (greedy daría 9+1+1)
    CHECK_EQ(*coin_change_min({1, 2, 5}, 0), 0u);
    CHECK(!coin_change_min({2}, 3).has_value());
    CHECK_EQ(*coin_change_min({186, 419, 83, 408}, 6249), 20u);
}
