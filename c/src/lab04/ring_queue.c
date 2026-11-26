/* LAB04 — tu implementación de <algolib/lab04/ring_queue.h>.
 * Reemplaza cada ALGOLIB_TODO() por tu código. Este archivo es TUYO:
 * `make actualizar` nunca lo sobrescribe. */
#include <algolib/lab04/ring_queue.h>

#include <stdlib.h>
#include <string.h>

struct RingQueue {
    unsigned char *data;
    size_t elem_size;
    size_t head;   /* índice del frente */
    size_t size;
    size_t cap;
};

RingQueue *ring_queue_new(size_t elem_size) {
    (void)elem_size;
    ALGOLIB_TODO();
}

void ring_queue_free(RingQueue *q) {
    (void)q;
    ALGOLIB_TODO();
}

bool ring_queue_push(RingQueue *q, const void *elem) {
    (void)q;
    (void)elem;
    ALGOLIB_TODO();
}

bool ring_queue_pop(RingQueue *q, void *out) {
    (void)q;
    (void)out;
    ALGOLIB_TODO();
}

void *ring_queue_front(const RingQueue *q) {
    (void)q;
    ALGOLIB_TODO();
}

void *ring_queue_back(const RingQueue *q) {
    (void)q;
    ALGOLIB_TODO();
}

void *ring_queue_get(const RingQueue *q, size_t i) {
    (void)q;
    (void)i;
    ALGOLIB_TODO();
}

size_t ring_queue_size(const RingQueue *q) {
    (void)q;
    ALGOLIB_TODO();
}

size_t ring_queue_capacity(const RingQueue *q) {
    (void)q;
    ALGOLIB_TODO();
}
