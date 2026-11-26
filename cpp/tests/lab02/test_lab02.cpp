#include <algolib/lab02/vector.hpp>

#include <algotest.hpp>

#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

using algolib::Vector;

namespace {
struct Tracked {
    static inline int alive = 0;
    int v;
    explicit Tracked(int x) : v(x) { ++alive; }
    Tracked(const Tracked& o) : v(o.v) { ++alive; }
    Tracked(Tracked&& o) noexcept : v(o.v) { ++alive; }
    Tracked& operator=(const Tracked&) = default;
    Tracked& operator=(Tracked&&) noexcept = default;
    ~Tracked() { --alive; }
};

template <class T>
std::vector<T> to_std(const Vector<T>& v) { return std::vector<T>(v.begin(), v.end()); }
}  // namespace

static_assert(std::contiguous_iterator<Vector<int>::iterator>);

TEST(vector_push_index) {
    Vector<int> v;
    for (int i = 0; i < 1000; ++i) v.push_back(i * i);
    CHECK_EQ(v.size(), 1000u);
    for (int i = 0; i < 1000; ++i) CHECK_EQ(v[i], i * i);
    CHECK_EQ(v.front(), 0);
    CHECK_EQ(v.back(), 999 * 999);
    CHECK_THROWS(v.at(1000), std::out_of_range);
}

TEST(vector_initializer_list_e_iteradores) {
    Vector<int> v{5, 3, 8, 1};
    std::sort(v.begin(), v.end());  // exige iteradores de acceso aleatorio
    CHECK(to_std(v) == (std::vector<int>{1, 3, 5, 8}));
    CHECK_EQ(std::accumulate(v.begin(), v.end(), 0), 17);
}

TEST(vector_insert_erase) {
    Vector<std::string> v{"b", "d"};
    v.insert(0, "a");
    v.insert(2, "c");
    v.insert(4, "e");
    CHECK(to_std(v) == (std::vector<std::string>{"a", "b", "c", "d", "e"}));
    v.erase(0);
    v.erase(1);
    v.erase(2);
    CHECK(to_std(v) == (std::vector<std::string>{"b", "d"}));
    CHECK_THROWS(v.insert(3, "x"), std::out_of_range);
    CHECK_THROWS(v.erase(2), std::out_of_range);
}

TEST(vector_insert_alias) {
    Vector<std::string> v{std::string(40, 'a')};
    v.shrink_to_fit();
    v.insert(0, v[0]);  // referencia a un elemento propio + realojamiento
    CHECK_EQ(v.size(), 2u);
    CHECK(v[0] == v[1]);
}

TEST(vector_reserve_shrink) {
    Vector<int> v;
    v.reserve(100);
    CHECK(v.capacity() >= 100);
    CHECK_EQ(v.size(), 0u);
    for (int i = 0; i < 10; ++i) v.push_back(i);
    std::size_t cap = v.capacity();
    v.reserve(5);  // no debe encoger
    CHECK_EQ(v.capacity(), cap);
    v.shrink_to_fit();
    CHECK_EQ(v.capacity(), 10u);
    CHECK_EQ(v[9], 9);
}

TEST(vector_sin_fugas) {
    Tracked::alive = 0;
    {
        Vector<Tracked> a;
        for (int i = 0; i < 100; ++i) a.emplace_back(i);
        a.insert(50, Tracked(-1));
        a.erase(0);
        CHECK_EQ(Tracked::alive, 100);
        Vector<Tracked> b = a;
        CHECK_EQ(Tracked::alive, 200);
        b.clear();
        CHECK_EQ(Tracked::alive, 100);
        Vector<Tracked> c = std::move(a);
        a = c;
        a.shrink_to_fit();
        a.pop_back();
        CHECK_EQ(Tracked::alive, 199);
    }
    CHECK_EQ(Tracked::alive, 0);
}

TEST(vector_aleatorio_vs_std) {
    algotest::Lcg rng(2);
    Vector<int> v;
    std::vector<int> o;
    for (int op = 0; op < 5000; ++op) {
        auto r = rng.below(10);
        if (r < 4 || o.empty()) {
            int x = (int)rng.range(-99, 99);
            v.push_back(x); o.push_back(x);
        } else if (r < 6) {
            auto i = rng.below(o.size() + 1);
            int x = (int)rng.range(-99, 99);
            v.insert(i, x); o.insert(o.begin() + (long)i, x);
        } else if (r < 8) {
            auto i = rng.below(o.size());
            v.erase(i); o.erase(o.begin() + (long)i);
        } else {
            v.pop_back(); o.pop_back();
        }
        CHECK_EQ(v.size(), o.size());
    }
    CHECK(to_std(v) == o);
}
