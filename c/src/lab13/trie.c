/* LAB13 — tu implementación de <algolib/lab13/trie.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab13/trie.h>

#include <stdlib.h>
#include <string.h>

typedef struct TrieNode {
    struct TrieNode *next[26];
    size_t prefix_count;   /* palabras que pasan por este nodo */
    bool end;              /* ¿termina una palabra aquí? */
} TrieNode;

struct Trie {
    TrieNode *root;
    size_t words;
    size_t nodes;
};

Trie *trie_new(void) {
    ALGOLIB_TODO();
}

void trie_free(Trie *t) {
    (void)t;
    ALGOLIB_TODO();
}

bool trie_insert(Trie *t, const char *w) {
    (void)t;
    (void)w;
    ALGOLIB_TODO();
}

bool trie_contains(const Trie *t, const char *w) {
    (void)t;
    (void)w;
    ALGOLIB_TODO();
}

bool trie_starts_with(const Trie *t, const char *p) {
    (void)t;
    (void)p;
    ALGOLIB_TODO();
}

size_t trie_count_with_prefix(const Trie *t, const char *p) {
    (void)t;
    (void)p;
    ALGOLIB_TODO();
}

bool trie_erase(Trie *t, const char *w) {
    (void)t;
    (void)w;
    ALGOLIB_TODO();
}

size_t trie_size(const Trie *t) {
    (void)t;
    ALGOLIB_TODO();
}

size_t trie_node_count(const Trie *t) {
    (void)t;
    ALGOLIB_TODO();
}
