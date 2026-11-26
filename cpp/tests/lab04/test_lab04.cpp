#include <algolib/lab04/hash_map.hpp>
#include <algolib/lab04/ring_queue.hpp>

#include <algotest.hpp>

#include <deque>
#include <map>
#include <string>

using algolib::HashMap;
using algolib::RingQueue;

TEST(ring_fifo) {
    RingQueue<int> q;
    for (int i = 0; i < 100; ++i) q.push(i);
    for (int i = 0; i < 100; ++i) {
        CHECK_EQ(q.front(), i);
        q.pop();
    }
    CHECK(q.empty());
    CHECK_THROWS(q.pop(), std::out_of_range);
}

TEST(ring_envuelve_y_crece) {
    // Fuerza head_ != 0 antes de crecer: el realojamiento debe "desenrollar".
    RingQueue<std::string> q;
    int n = 0;
    do { q.push(std::to_string(n++)); } while (q.size() < 3 || q.size() != q.capacity());
    std::size_t cap = q.capacity();  // lleno
    q.pop(); q.pop();
    q.push(std::to_string(n++));
    q.push(std::to_string(n++));
    CHECK_EQ(q.capacity(), cap);     // cupo sin crecer: dio la vuelta
    q.push(std::to_string(n++));     // ahora sí crece
    CHECK(q.capacity() > cap);
    for (std::size_t i = 0; i < q.size(); ++i) CHECK_EQ(q[i], std::to_string(i + 2));
    CHECK_EQ(q.back(), std::to_string(n - 1));
}

TEST(ring_aleatorio_vs_deque) {
    algotest::Lcg rng(4);
    RingQueue<int> q;
    std::deque<int> o;
    for (int op = 0; op < 50000; ++op) {
        if (o.empty() || rng.below(5) < 3) {
            int x = (int)rng.range(0, 1 << 20);
            q.push(x); o.push_back(x);
        } else {
            CHECK_EQ(q.front(), o.front());
            q.pop(); o.pop_front();
        }
        CHECK_EQ(q.size(), o.size());
        if (!o.empty()) CHECK_EQ(q.back(), o.back());
    }
}

namespace {
struct Tracked {
    static inline int alive = 0;
    int v;
    Tracked(int x) : v(x) { ++alive; }
    Tracked(const Tracked& o) : v(o.v) { ++alive; }
    Tracked(Tracked&& o) noexcept : v(o.v) { ++alive; }
    ~Tracked() { --alive; }
};
}  // namespace

TEST(ring_sin_fugas) {
    Tracked::alive = 0;
    {
        RingQueue<Tracked> q;
        for (int i = 0; i < 37; ++i) q.push(Tracked(i));
        for (int i = 0; i < 20; ++i) q.pop();
        for (int i = 0; i < 50; ++i) q.push(Tracked(i));  // da la vuelta y crece
        CHECK_EQ(Tracked::alive, 67);
    }
    CHECK_EQ(Tracked::alive, 0);
}

TEST(hash_muchos_borrados) {
    // 200 000 claves distintas pasan por la tabla, pero nunca hay más de 64 vivas.
    // Con lápidas que nunca se limpian, la tabla se llena de lápidas y las búsquedas
    // fallidas no terminan (o el factor de carga real se dispara).
    HashMap<int, int> m;
    for (int i = 0; i < 200000; ++i) {
        CHECK(m.insert(i, i));
        if (i >= 64) CHECK(m.erase(i - 64));
        CHECK(!m.contains(-1 - i));  // búsqueda fallida: debe terminar
    }
    CHECK_EQ(m.size(), 64u);
    for (int i = 200000 - 64; i < 200000; ++i) CHECK_EQ(*m.find(i), i);
}

TEST(hash_basico) {
    HashMap<std::string, int> m;
    CHECK(m.insert("uno", 1));
    CHECK(m.insert("dos", 2));
    CHECK(!m.insert("uno", 11));  // reemplaza
    CHECK_EQ(m.size(), 2u);
    CHECK_EQ(*m.find("uno"), 11);
    CHECK(m.find("tres") == nullptr);
    CHECK(m.erase("uno"));
    CHECK(!m.erase("uno"));
    CHECK(!m.contains("uno"));
    CHECK_EQ(m.size(), 1u);
}

TEST(hash_factor_de_carga_acotado) {
    HashMap<int, int> m;
    for (int i = 0; i < 100000; ++i) m.insert(i, i);
    CHECK(m.load_factor() <= 1.0);
    CHECK(m.load_factor() > 0.0);
    for (int i = 0; i < 100000; ++i) CHECK_EQ(*m.find(i), i);
}

namespace {
struct ConstantHash {
    std::size_t operator()(int) const { return 42; }
};
}  // namespace

TEST(hash_colisiones_totales) {
    HashMap<int, int, ConstantHash> m;
    for (int i = 0; i < 500; ++i) m.insert(i, -i);
    for (int i = 0; i < 500; i += 2) CHECK(m.erase(i));
    for (int i = 0; i < 500; ++i) {
        if (i % 2 == 0) CHECK(!m.contains(i));
        else CHECK_EQ(*m.find(i), -i);
    }
    CHECK_EQ(m.size(), 250u);
}

TEST(hash_aleatorio_vs_map) {
    algotest::Lcg rng(5);
    HashMap<int, int> m;
    std::map<int, int> o;
    for (int op = 0; op < 50000; ++op) {
        int k = (int)rng.range(0, 2000);
        switch (rng.below(3)) {
            case 0: {
                int v = (int)rng.range(0, 1000);
                CHECK_EQ(m.insert(k, v), o.find(k) == o.end());
                o[k] = v;
                break;
            }
            case 1:
                CHECK_EQ(m.erase(k), o.erase(k) == 1);
                break;
            default: {
                auto it = o.find(k);
                const int* p = m.find(k);
                CHECK_EQ(p != nullptr, it != o.end());
                if (p) CHECK_EQ(*p, it->second);
            }
        }
        CHECK_EQ(m.size(), o.size());
    }
}
