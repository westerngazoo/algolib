#include <algolib/lab01/array_stack.hpp>
#include <algolib/lab01/min_stack.hpp>

#include <algotest.hpp>

#include <algorithm>
#include <string>
#include <vector>

using algolib::ArrayStack;
using algolib::MinStack;

namespace {
// Cuenta objetos vivos: detecta fugas y dobles destrucciones.
struct Tracked {
    static inline int alive = 0;
    int v;
    explicit Tracked(int x) : v(x) { ++alive; }
    Tracked(const Tracked& o) : v(o.v) { ++alive; }
    Tracked(Tracked&& o) noexcept : v(o.v) { ++alive; }
    Tracked& operator=(const Tracked&) = default;
    ~Tracked() { --alive; }
};

// Tipo sin constructor por defecto: prohíbe implementar con new T[n].
struct NoDefault {
    int v;
    explicit NoDefault(int x) : v(x) {}
};
}  // namespace

// ---------------------------------------------------------------- ArrayStack

TEST(stack_lifo) {
    ArrayStack<int> s;
    CHECK(s.empty());
    for (int i = 0; i < 100; ++i) s.push(i);
    CHECK_EQ(s.size(), 100u);
    for (int i = 99; i >= 0; --i) {
        CHECK_EQ(s.top(), i);
        s.pop();
    }
    CHECK(s.empty());
}

TEST(stack_vacia_lanza) {
    ArrayStack<int> s;
    CHECK_THROWS(s.pop(), std::out_of_range);
    CHECK_THROWS(s.top(), std::out_of_range);
    const ArrayStack<int>& cs = s;
    CHECK_THROWS(cs.top(), std::out_of_range);
}

TEST(stack_crecimiento_geometrico) {
    ArrayStack<int> s;
    std::size_t prev = s.capacity(), cambios = 0;
    for (int i = 0; i < 1024; ++i) {
        s.push(i);
        CHECK(s.size() <= s.capacity());
        if (s.capacity() != prev) {
            CHECK(prev == 0 ? s.capacity() == 1 : s.capacity() == 2 * prev);
            ++cambios;
            prev = s.capacity();
        }
    }
    CHECK_EQ(cambios, 11u);  // 1, 2, 4, ..., 1024
}

TEST(stack_sin_constructor_por_defecto) {
    ArrayStack<NoDefault> s;
    for (int i = 0; i < 10; ++i) s.emplace(i);
    CHECK_EQ(s.top().v, 9);
}

TEST(stack_alias_push_top) {
    ArrayStack<std::string> s;
    s.push(std::string(40, 'x'));  // cadena larga: fuerza memoria dinámica
    for (int i = 0; i < 20; ++i) s.push(s.top());
    CHECK_EQ(s.size(), 21u);
    CHECK_EQ(s.top(), std::string(40, 'x'));
}

TEST(stack_copia_movimiento_sin_fugas) {
    Tracked::alive = 0;
    {
        ArrayStack<Tracked> a;
        for (int i = 0; i < 50; ++i) a.emplace(i);
        ArrayStack<Tracked> b = a;
        CHECK_EQ(Tracked::alive, 100);
        ArrayStack<Tracked> c = std::move(a);
        CHECK(a.empty());
        CHECK_EQ(c.size(), 50u);
        b.pop();
        CHECK_EQ(b.top().v, 48);
        a = c;
        CHECK_EQ(Tracked::alive, 149);
        c.clear();
        CHECK(c.empty());
        CHECK_EQ(Tracked::alive, 99);
    }
    CHECK_EQ(Tracked::alive, 0);
}

TEST(stack_aleatorio_vs_vector) {
    algotest::Lcg rng(1);
    ArrayStack<long> s;
    std::vector<long> oracle;
    for (int op = 0; op < 20000; ++op) {
        if (oracle.empty() || rng.below(3) != 0) {
            long x = rng.range(-1000, 1000);
            s.push(x);
            oracle.push_back(x);
        } else {
            CHECK_EQ(s.top(), oracle.back());
            s.pop();
            oracle.pop_back();
        }
        CHECK_EQ(s.size(), oracle.size());
    }
}

// ---------------------------------------------------------------- MinStack

TEST(minstack_basico) {
    MinStack<int> m;
    CHECK(m.empty());
    m.push(5); m.push(3); m.push(7); m.push(3); m.push(1);
    CHECK_EQ(m.get_min(), 1);
    m.pop();
    CHECK_EQ(m.get_min(), 3);
    m.pop();  // quita un 3 duplicado: el mínimo sigue siendo 3
    CHECK_EQ(m.get_min(), 3);
    m.pop();
    CHECK_EQ(m.get_min(), 3);
    m.pop();
    CHECK_EQ(m.get_min(), 5);
    CHECK_EQ(m.top(), 5);
    m.pop();
    CHECK(m.empty());
    CHECK_THROWS(m.get_min(), std::out_of_range);
}

TEST(minstack_aleatorio) {
    algotest::Lcg rng(7);
    MinStack<int> m;
    std::vector<int> oracle;
    for (int op = 0; op < 20000; ++op) {
        if (oracle.empty() || rng.below(2) == 0) {
            int x = static_cast<int>(rng.range(-50, 50));
            m.push(x);
            oracle.push_back(x);
        } else {
            m.pop();
            oracle.pop_back();
        }
        CHECK_EQ(m.size(), oracle.size());
        if (!oracle.empty())
            CHECK_EQ(m.get_min(), *std::min_element(oracle.begin(), oracle.end()));
    }
}
