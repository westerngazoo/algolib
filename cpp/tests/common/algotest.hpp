// algotest — micro-framework de pruebas (sin dependencias externas).
//
//   TEST(nombre) { CHECK(cond); CHECK_EQ(a, b); CHECK_THROWS(expr, Tipo); }
//
// Cada lab compila un ejecutable labNN_tests; el main lo provee algotest_main.cpp.
#pragma once

#include <algolib/common.hpp>

#include <cstdint>
#include <cstdio>
#include <exception>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace algotest {

struct Failure {
    std::string msg;
};

struct Case {
    const char* name;
    void (*fn)();
};

inline std::vector<Case>& registry() {
    static std::vector<Case> r;
    return r;
}

struct Registrar {
    Registrar(const char* n, void (*f)()) { registry().push_back({n, f}); }
};

// Imprime un valor si tiene operator<<; si no, "<?>".
template <class T, class = void>
struct printable : std::false_type {};
template <class T>
struct printable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>>
    : std::true_type {};

template <class T>
std::string show(const T& v) {
    if constexpr (printable<T>::value) {
        std::ostringstream os;
        os << v;
        return os.str();
    } else {
        return "<?>";
    }
}

[[noreturn]] inline void fail(const char* file, int line, const std::string& what) {
    throw Failure{std::string(file) + ":" + std::to_string(line) + "  " + what};
}

// Generador congruencial lineal determinista (mismo que en Rust: tests/common/mod.rs).
// Así los tests aleatorios son reproducibles y comparables entre lenguajes.
struct Lcg {
    std::uint64_t s;
    explicit Lcg(std::uint64_t seed) : s(seed) {}
    std::uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
    // Uniforme en [0, n).
    std::uint64_t below(std::uint64_t n) { return next() % n; }
    // Uniforme en [lo, hi].
    std::int64_t range(std::int64_t lo, std::int64_t hi) {
        return lo + static_cast<std::int64_t>(below(static_cast<std::uint64_t>(hi - lo + 1)));
    }
};

inline int run(const char* suite) {
    int ok = 0, failed = 0, todo = 0;
    std::printf("== %s (C++) ==\n", suite);
    for (auto& c : registry()) {
        try {
            c.fn();
            ++ok;
            std::printf("  [ OK ] %s\n", c.name);
        } catch (const algolib::NotImplemented& e) {
            ++todo;
            std::printf("  [TODO] %s  (%s)\n", c.name, e.what());
        } catch (const Failure& f) {
            ++failed;
            std::printf("  [FAIL] %s\n         %s\n", c.name, f.msg.c_str());
        } catch (const std::exception& e) {
            ++failed;
            std::printf("  [FAIL] %s\n         excepción inesperada: %s\n", c.name, e.what());
        }
    }
    std::printf("-- %d ok, %d fallidos, %d pendientes\n", ok, failed, todo);
    return (failed || todo) ? 1 : 0;
}

}  // namespace algotest

#define ALGOTEST_CAT2(a, b) a##b
#define ALGOTEST_CAT(a, b) ALGOTEST_CAT2(a, b)

#define TEST(name)                                                              \
    static void name();                                                         \
    static ::algotest::Registrar ALGOTEST_CAT(reg_, name)(#name, &name);       \
    static void name()

// Variádico para admitir comas de plantillas: CHECK(f<int, int>()).
#define CHECK(...)                                                              \
    do {                                                                        \
        if (!(__VA_ARGS__))                                                     \
            ::algotest::fail(__FILE__, __LINE__, "CHECK(" #__VA_ARGS__ ")");    \
    } while (0)

// Copia (no referencia) los dos lados: si `a` es `*f()` con f() devolviendo un
// temporal (p. ej. un std::optional), una referencia quedaría colgando al terminar
// la expresión; la extensión de vida de los temporales no atraviesa operator*.
#define CHECK_EQ(a, b)                                                          \
    do {                                                                        \
        const auto va_ = (a);                                                   \
        const auto vb_ = (b);                                                   \
        if (!(va_ == vb_))                                                      \
            ::algotest::fail(__FILE__, __LINE__,                                \
                             "CHECK_EQ(" #a ", " #b ")  →  " +                  \
                                 ::algotest::show(va_) + " != " +               \
                                 ::algotest::show(vb_));                        \
    } while (0)

#define CHECK_THROWS(expr, Exc)                                                 \
    do {                                                                        \
        bool thrown_ = false;                                                   \
        try {                                                                   \
            (void)(expr);                                                       \
        } catch (const Exc&) {                                                  \
            thrown_ = true;                                                     \
        }                                                                       \
        if (!thrown_)                                                           \
            ::algotest::fail(__FILE__, __LINE__,                                \
                             "CHECK_THROWS(" #expr ", " #Exc ") no lanzó");      \
    } while (0)
