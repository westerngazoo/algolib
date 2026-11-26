#include <algolib/lab03/dlist.hpp>
#include <algolib/lab03/forward_list.hpp>

#include <algotest.hpp>

#include <deque>
#include <string>
#include <vector>

using algolib::DList;
using algolib::ForwardList;

TEST(flist_lifo) {
    ForwardList<int> l;
    CHECK(l.empty());
    for (int i = 0; i < 5; ++i) l.push_front(i);
    CHECK_EQ(l.size(), 5u);
    CHECK(l.to_vector() == (std::vector<int>{4, 3, 2, 1, 0}));
    CHECK_EQ(l.front(), 4);
    l.pop_front();
    CHECK_EQ(l.front(), 3);
    CHECK_THROWS((ForwardList<int>{}.pop_front()), std::out_of_range);
}

TEST(flist_reverse) {
    ForwardList<std::string> l;
    l.reverse();  // lista vacía
    CHECK(l.empty());
    for (auto s : {"c", "b", "a"}) l.push_front(s);
    l.reverse();
    CHECK(l.to_vector() == (std::vector<std::string>{"c", "b", "a"}));
    l.push_front("d");
    CHECK_EQ(l.size(), 4u);
    CHECK_EQ(l.front(), "d");
}

TEST(flist_destructor_iterativo_un_millon) {
    // Con el destructor recursivo por defecto esto desborda la pila.
    ForwardList<int> l;
    for (int i = 0; i < 1'000'000; ++i) l.push_front(i);
    CHECK_EQ(l.size(), 1'000'000u);
}

TEST(flist_move) {
    ForwardList<int> a;
    a.push_front(1);
    ForwardList<int> b = std::move(a);
    CHECK_EQ(b.front(), 1);
}

TEST(dlist_deque_basico) {
    DList<int> d;
    d.push_back(2);
    d.push_front(1);
    d.push_back(3);
    CHECK(d.to_vector() == (std::vector<int>{1, 2, 3}));
    CHECK(d.to_vector_reversed() == (std::vector<int>{3, 2, 1}));
    CHECK_EQ(d.front(), 1);
    CHECK_EQ(d.back(), 3);
    d.pop_front();
    d.pop_back();
    CHECK_EQ(d.front(), 2);
    CHECK_EQ(d.back(), 2);
    d.pop_back();
    CHECK(d.empty());
    CHECK_THROWS(d.pop_front(), std::out_of_range);
    CHECK_THROWS(d.back(), std::out_of_range);
    d.push_front(9);  // reutilizable tras vaciarse
    CHECK(d.to_vector_reversed() == (std::vector<int>{9}));
}

TEST(dlist_aleatorio_vs_deque) {
    algotest::Lcg rng(3);
    DList<int> d;
    std::deque<int> o;
    for (int op = 0; op < 20000; ++op) {
        int x = (int)rng.range(0, 999);
        switch (rng.below(4)) {
            case 0: d.push_front(x); o.push_front(x); break;
            case 1: d.push_back(x); o.push_back(x); break;
            case 2: if (!o.empty()) { CHECK_EQ(d.front(), o.front()); d.pop_front(); o.pop_front(); } break;
            case 3: if (!o.empty()) { CHECK_EQ(d.back(), o.back()); d.pop_back(); o.pop_back(); } break;
        }
        CHECK_EQ(d.size(), o.size());
    }
    CHECK(d.to_vector() == std::vector<int>(o.begin(), o.end()));
    CHECK(d.to_vector_reversed() == std::vector<int>(o.rbegin(), o.rend()));
}
