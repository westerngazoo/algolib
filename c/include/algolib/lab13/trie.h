/* LAB 13 — Trie sobre el alfabeto 'a'..'z'. Todas las operaciones O(L).  Notas: notas/L13
 * count_with_prefix O(L): mantén un contador por nodo. erase poda nodos sin uso. */
#ifndef ALGOLIB_LAB13_TRIE_H
#define ALGOLIB_LAB13_TRIE_H

#include <algolib/common.h>

typedef struct Trie Trie;

Trie *trie_new(void);
void trie_free(Trie *t);
bool trie_insert(Trie *t, const char *w);
bool trie_contains(const Trie *t, const char *w);
bool trie_starts_with(const Trie *t, const char *p);
size_t trie_count_with_prefix(const Trie *t, const char *p);
bool trie_erase(Trie *t, const char *w);
size_t trie_size(const Trie *t);
size_t trie_node_count(const Trie *t);

/* trie_node_count: número de nodos incluida la raíz (1 en un trie vacío). */
#endif
