/* algotest (C) — micro-framework de pruebas, mismo formato que en C++ y Rust:
 *   [ OK ] / [FAIL] / [TODO]
 *
 * Cada archivo de tests define sus funciones `static void nombre(void)` y termina con
 *   int main(void) { return algotest_run("labNN", (AlgoTest[]){ T(caso1), T(caso2), {0} }); }
 *
 * CHECK / CHECK_EQ y ALGOLIB_TODO() abortan el caso con longjmp (lo más parecido a
 * una excepción que tiene C). Si todos los casos pasan, el programa termina normalmente
 * y LeakSanitizer revisa que no haya fugas de memoria. */
#ifndef ALGOTEST_H
#define ALGOTEST_H

#include <algolib/common.h>

#include <stdint.h>
#include <stdio.h>

typedef struct {
    const char *name;
    void (*fn)(void);
} AlgoTest;

#define T(f) {#f, f}

int algotest_run(const char *suite, const AlgoTest *tests);
_Noreturn void algotest_fail(const char *file, int line, const char *msg);

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) algotest_fail(__FILE__, __LINE__, "CHECK(" #cond ")");  \
    } while (0)

/* Compara como enteros con signo de 64 bits (sirve para int, size_t, int64_t, bool). */
#define CHECK_EQ(a, b)                                                       \
    do {                                                                     \
        long long va_ = (long long)(a), vb_ = (long long)(b);                \
        if (va_ != vb_) {                                                    \
            char m_[300];                                                    \
            snprintf(m_, sizeof m_, "CHECK_EQ(%s, %s)  →  %lld != %lld", #a, #b, va_, vb_); \
            algotest_fail(__FILE__, __LINE__, m_);                           \
        }                                                                    \
    } while (0)

/* Generador congruencial lineal: idéntico al de C++ (algotest::Lcg) y Rust (common::Lcg). */
typedef struct {
    uint64_t s;
} Lcg;
static inline uint64_t lcg_next(Lcg *r) {
    r->s = r->s * 6364136223846793005ULL + 1442695040888963407ULL;
    return r->s >> 33;
}
static inline uint64_t lcg_below(Lcg *r, uint64_t n) { return lcg_next(r) % n; }
static inline int64_t lcg_range(Lcg *r, int64_t lo, int64_t hi) {
    return lo + (int64_t)lcg_below(r, (uint64_t)(hi - lo + 1));
}

/* Comparadores de uso común en los tests. */
static inline int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}
static inline int cmp_int_desc(const void *a, const void *b) { return cmp_int(b, a); }
static inline int cmp_i64(const void *a, const void *b) {
    int64_t x = *(const int64_t *)a, y = *(const int64_t *)b;
    return (x > y) - (x < y);
}
static inline int cmp_u32(const void *a, const void *b) {
    uint32_t x = *(const uint32_t *)a, y = *(const uint32_t *)b;
    return (x > y) - (x < y);
}

#endif
