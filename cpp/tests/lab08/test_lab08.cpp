#include <algolib/lab08/order_stat_set.hpp>

#include <algotest.hpp>

#include <algorithm>
#include <set>
#include <vector>

using algolib::OrderStatSet;

TEST(ostat_basico) {
    OrderStatSet<int> s;
    for (int x : {50, 20, 80, 10, 30, 70, 90}) CHECK(s.insert(x));
    CHECK(!s.insert(30));
    CHECK_EQ(s.size(), 7u);
    CHECK_EQ(*s.select(0), 10);
    CHECK_EQ(*s.select(3), 50);
    CHECK_EQ(*s.select(6), 90);
    CHECK(!s.select(7).has_value());
    CHECK_EQ(s.rank(10), 0u);
    CHECK_EQ(s.rank(55), 4u);   // 10,20,30,50 < 55
    CHECK_EQ(s.rank(100), 7u);
    CHECK(s.erase(50));
    CHECK(!s.erase(50));
    CHECK_EQ(*s.select(3), 70);
    CHECK(!s.contains(50));
}

TEST(ostat_aleatorio_vs_vector_ordenado) {
    algotest::Lcg rng(13);
    OrderStatSet<int> s;
    std::set<int> o;
    for (int op = 0; op < 30000; ++op) {
        int x = (int)rng.range(0, 5000);
        if (rng.below(3) != 0) CHECK_EQ(s.insert(x), o.insert(x).second);
        else CHECK_EQ(s.erase(x), o.erase(x) == 1);
        CHECK_EQ(s.size(), o.size());
        if (op % 100 == 0) {
            std::vector<int> v(o.begin(), o.end());
            for (int q = 0; q < 20; ++q) {
                std::size_t k = rng.below(v.size() + 1);
                auto sel = s.select(k);
                CHECK_EQ(sel.has_value(), k < v.size());
                if (sel) CHECK_EQ(*sel, v[k]);
                int y = (int)rng.range(-1, 5001);
                CHECK_EQ(s.rank(y), (std::size_t)(std::lower_bound(v.begin(), v.end(), y) - v.begin()));
            }
        }
    }
}
