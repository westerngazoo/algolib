#include <algolib/lab07/avl_map.hpp>

#include <algotest.hpp>

#include <cmath>
#include <map>
#include <string>
#include <vector>

using algolib::AvlMap;

namespace {
bool cumple_cota_avl(int h, std::size_t n) {
    return h < 1.4405 * std::log2(double(n) + 2) - 0.3277 + 1e-9;
}
}  // namespace

TEST(avl_basico) {
    AvlMap<std::string, int> m;
    CHECK(m.empty());
    CHECK_EQ(m.height(), 0);
    CHECK(m.insert("b", 2));
    CHECK(m.insert("a", 1));
    CHECK(m.insert("c", 3));
    CHECK(!m.insert("a", 10));
    CHECK_EQ(*m.find("a"), 10);
    CHECK(m.find("z") == nullptr);
    CHECK(m.keys() == (std::vector<std::string>{"a", "b", "c"}));
    CHECK_EQ(*m.min_key(), "a");
    CHECK_EQ(*m.max_key(), "c");
    CHECK(m.erase("b"));
    CHECK(!m.erase("b"));
    CHECK_EQ(m.size(), 2u);
}

TEST(avl_secuencial_es_perfecto) {
    // Insertar 1..7 en orden produce el árbol perfecto de altura 3.
    AvlMap<int, int> m;
    for (int i = 1; i <= 7; ++i) m.insert(i, i);
    CHECK_EQ(m.height(), 3);
    CHECK(!AvlMap<int, int>{}.min_key().has_value());
}

TEST(avl_cota_de_altura_insercion_ordenada) {
    AvlMap<int, int> m;
    for (int i = 0; i < 100000; ++i) {
        m.insert(i, i);
        if ((i & (i + 1)) == 0) CHECK(cumple_cota_avl(m.height(), m.size()));
    }
    CHECK(cumple_cota_avl(m.height(), m.size()));
    for (int i = 0; i < 100000; i += 3) m.erase(i);
    CHECK(cumple_cota_avl(m.height(), m.size()));
}

TEST(avl_aleatorio_vs_map) {
    algotest::Lcg rng(12);
    AvlMap<int, int> m;
    std::map<int, int> o;
    for (int op = 0; op < 50000; ++op) {
        int k = (int)rng.range(0, 3000);
        switch (rng.below(3)) {
            case 0: { int v = (int)rng.range(0, 99); CHECK_EQ(m.insert(k, v), o.find(k) == o.end()); o[k] = v; break; }
            case 1: CHECK_EQ(m.erase(k), o.erase(k) == 1); break;
            default: {
                auto it = o.find(k);
                const int* p = m.find(k);
                CHECK_EQ(p != nullptr, it != o.end());
                if (p) CHECK_EQ(*p, it->second);
            }
        }
        CHECK_EQ(m.size(), o.size());
        if (op % 5000 == 0) {
            std::vector<int> ks;
            for (auto& [k2, v2] : o) ks.push_back(k2);
            CHECK(m.keys() == ks);
            CHECK(cumple_cota_avl(m.height(), m.size()));
        }
    }
}

TEST(avl_borrar_rebalancea) {
    // Borrar casi todo un lado: sin rotaciones al borrar, quedaría la columna derecha
    // del árbol original (altura ~16 con ~500 claves) y violaría la cota.
    AvlMap<int, int> m;
    for (int i = 0; i < 65536; ++i) m.insert(i, i);
    for (int i = 0; i < 65000; ++i) CHECK(m.erase(i));
    CHECK_EQ(m.size(), 536u);
    CHECK(cumple_cota_avl(m.height(), m.size()));
    std::vector<int> esperado;
    for (int i = 65000; i < 65536; ++i) esperado.push_back(i);
    CHECK(m.keys() == esperado);
}
