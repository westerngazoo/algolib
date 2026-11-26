/* LAB 04 — RingQueue: cola FIFO sobre buffer circular que crece.  Notas: notas/L04
 * El elemento i (0 = frente) vive en la posición (head + i) mod capacidad.
 * push O(1) amortizado; pop/front/back/get O(1). Al crecer, "desenrolla" el buffer. */
#ifndef ALGOLIB_LAB04_RING_QUEUE_H
#define ALGOLIB_LAB04_RING_QUEUE_H

#include <algolib/common.h>

typedef struct RingQueue RingQueue;

RingQueue *ring_queue_new(size_t elem_size);
void ring_queue_free(RingQueue *q);
bool ring_queue_push(RingQueue *q, const void *elem);
bool ring_queue_pop(RingQueue *q, void *out);
void *ring_queue_front(const RingQueue *q);
void *ring_queue_back(const RingQueue *q);
void *ring_queue_get(const RingQueue *q, size_t i);
size_t ring_queue_size(const RingQueue *q);
size_t ring_queue_capacity(const RingQueue *q);

#endif
