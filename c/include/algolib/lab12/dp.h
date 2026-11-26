/* LAB 12 — Programación dinámica (documenta SRTBOT para cada problema).  Notas: notas/L12
 *  fibonacci        O(n) tiempo, O(1) memoria (n <= 93)
 *  lcs_length       O(|a|·|b|) tiempo, O(min(|a|,|b|)) memoria
 *  edit_distance    O(|a|·|b|) (Levenshtein, costos 1)
 *  knapsack_01      O(n·W) pseudo-polinomial
 *  lis_length       O(n lg n), estrictamente creciente
 *  coin_change_min  O(k·amount); false si no se puede formar */
#ifndef ALGOLIB_LAB12_DP_H
#define ALGOLIB_LAB12_DP_H

#include <algolib/common.h>

uint64_t fibonacci(unsigned n);
size_t lcs_length(const char *a, const char *b);
size_t edit_distance(const char *a, const char *b);
uint64_t knapsack_01(const size_t *weights, const uint64_t *values, size_t n, size_t capacity);
size_t lis_length(const int64_t *a, size_t n);
bool coin_change_min(const size_t *coins, size_t k, size_t amount, size_t *out);

#endif
