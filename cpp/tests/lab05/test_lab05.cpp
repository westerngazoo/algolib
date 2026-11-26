#include <algolib/lab05/sorting.hpp>

#include <algotest.hpp>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

using namespace algolib;

namespace {
struct Item {
    int key;
    int id;
    bool operator<(const Item& o) const { return key < o.key; }  // ignora id
};

std::vector<std::vector<int>> casos(std::size_t n_rand) {
    algotest::Lcg rng(6);
    std::vector<std::vector<int>> cs = {{}, {1}, {2, 1}, {1, 2}, {3, 3, 3}, {5, -1, 3, -1, 0}};
    for (std::size_t n : {7u, 16u, 100u, (unsigned)n_rand}) {
        std::vector<int> r(n);
        for (auto& x : r) x = (int)rng.range(-1000, 1000);
        cs.push_back(r);
        std::vector<int> pocos(n);  // muchos duplicados
        for (auto& x : pocos) x = (int)rng.range(0, 3);
        cs.push_back(pocos);
        std::vector<int> asc(n), desc(n);
        for (std::size_t i = 0; i < n; ++i) asc[i] = (int)i, desc[i] = (int)(n - i);
        cs.push_back(asc);
        cs.push_back(desc);
    }
    return cs;
}

void probar(const std::function<void(std::span<int>)>& sort, std::size_t n_rand) {
    for (auto c : casos(n_rand)) {
        auto esperado = c;
        std::sort(esperado.begin(), esperado.end());
        sort(std::span{c});
        CHECK(c == esperado);
    }
}

void probar_estable(const std::function<void(std::span<Item>)>& sort) {
    algotest::Lcg rng(8);
    std::vector<Item> v(2000);
    for (int i = 0; i < 2000; ++i) v[i] = {(int)rng.range(0, 20), i};
    auto esperado = v;
    std::stable_sort(esperado.begin(), esperado.end());
    sort(std::span{v});
    for (std::size_t i = 0; i < v.size(); ++i) {
        CHECK_EQ(v[i].key, esperado[i].key);
        CHECK_EQ(v[i].id, esperado[i].id);
    }
}
}  // namespace

TEST(insertion_sort_correcto) { probar([](std::span<int> a) { insertion_sort(a); }, 1000); }
TEST(insertion_sort_estable) { probar_estable([](std::span<Item> a) { insertion_sort(a); }); }
TEST(merge_sort_correcto) { probar([](std::span<int> a) { merge_sort(a); }, 100000); }
TEST(merge_sort_estable) { probar_estable([](std::span<Item> a) { merge_sort(a); }); }
TEST(quick_sort_correcto) { probar([](std::span<int> a) { quick_sort(a); }, 100000); }
TEST(heap_sort_correcto) { probar([](std::span<int> a) { heap_sort(a); }, 100000); }

TEST(quick_sort_adversarial) {
    std::vector<int> asc(200000), iguales(200000, 7);
    for (int i = 0; i < 200000; ++i) asc[i] = i;
    auto e = asc;
    quick_sort(std::span{asc});
    CHECK(asc == e);
    quick_sort(std::span{iguales});
    CHECK(std::all_of(iguales.begin(), iguales.end(), [](int x) { return x == 7; }));
}

TEST(sort_strings) {
    std::vector<std::string> v{"pera", "manzana", "uva", "kiwi", "fresa", "banana"};
    auto e = v;
    std::sort(e.begin(), e.end());
    auto a = v, b = v, c = v;
    merge_sort(std::span{a});
    quick_sort(std::span{b});
    heap_sort(std::span{c});
    CHECK(a == e);
    CHECK(b == e);
    CHECK(c == e);
}

TEST(counting_y_radix) {
    algotest::Lcg rng(9);
    std::vector<std::uint32_t> a(100000), b(100000);
    for (auto& x : a) x = (std::uint32_t)rng.below(1000);
    for (auto& x : b) x = (std::uint32_t)rng.next() ^ ((std::uint32_t)rng.next() << 31);
    auto ea = a, eb = b;
    std::sort(ea.begin(), ea.end());
    std::sort(eb.begin(), eb.end());
    counting_sort(std::span{a}, 999);
    radix_sort(std::span{b});
    CHECK(a == ea);
    CHECK(b == eb);
    std::vector<std::uint32_t> vacio;
    counting_sort(std::span{vacio}, 0);
    radix_sort(std::span{vacio});
    CHECK(vacio.empty());
}
