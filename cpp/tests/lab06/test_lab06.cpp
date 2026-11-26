#include <algolib/lab06/binary_heap.hpp>

#include <algotest.hpp>

#include <algorithm>
#include <functional>
#include <queue>
#include <string>
#include <vector>

using algolib::BinaryHeap;

TEST(heap_min_basico) {
    BinaryHeap<int> h;
    CHECK(h.empty());
    for (int x : {5, 1, 8, 3, 9, 2}) h.push(x);
    CHECK_EQ(h.size(), 6u);
    CHECK_EQ(h.top(), 1);
    h.pop();
    CHECK_EQ(h.top(), 2);
    CHECK(h.drain_sorted() == (std::vector<int>{2, 3, 5, 8, 9}));
    CHECK(h.empty());
    CHECK_THROWS(h.pop(), std::out_of_range);
    CHECK_THROWS(h.top(), std::out_of_range);
}

TEST(heap_comparador_max) {
    BinaryHeap<std::string, std::greater<std::string>> h;
    for (auto s : {"b", "d", "a", "c"}) h.push(s);
    CHECK_EQ(h.top(), "d");
    CHECK(h.drain_sorted() == (std::vector<std::string>{"d", "c", "b", "a"}));
}

TEST(heap_heapify_desde_vector) {
    algotest::Lcg rng(10);
    std::vector<int> v(50000);
    for (auto& x : v) x = (int)rng.range(-100000, 100000);
    BinaryHeap<int> h(v);
    CHECK_EQ(h.size(), v.size());
    std::sort(v.begin(), v.end());
    CHECK(h.drain_sorted() == v);
}

TEST(heap_aleatorio_vs_priority_queue) {
    algotest::Lcg rng(11);
    BinaryHeap<int> h;
    std::priority_queue<int, std::vector<int>, std::greater<int>> o;
    for (int op = 0; op < 50000; ++op) {
        if (o.empty() || rng.below(3) != 0) {
            int x = (int)rng.range(0, 500);
            h.push(x); o.push(x);
        } else {
            CHECK_EQ(h.top(), o.top());
            h.pop(); o.pop();
        }
        CHECK_EQ(h.size(), o.size());
    }
}

namespace {
long comparaciones = 0;
struct MenorContando {
    bool operator()(int a, int b) const { ++comparaciones; return a < b; }
};
}  // namespace

TEST(heap_heapify_es_lineal) {
    // Construir desde un vector debe costar O(n): a lo más 2n comparaciones (L06 §4).
    // Hacer n push sobre la entrada descendente costaría ~n lg n ≈ 16n.
    const int n = 1 << 16;
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = n - i;
    comparaciones = 0;
    BinaryHeap<int, MenorContando> h(v);
    CHECK(comparaciones <= 2L * n);
    CHECK_EQ(h.top(), 1);
}
