#include "ring_buffer.h"
#include <stdlib.h>

/*
 * =====================================================================
 * WRITTEN JUSTIFICATIONS (as required by the assignment)
 * =====================================================================
 *
 * 1. Justification for choosing the method to distinguish empty and full states:
 * The structure uses an additional field 'count' (number of elements).
 * The alternative standard approach is to always leave one cell free.
 * Why the counter was chosen: it allows using the entire allocated array
 * capacity (N elements, not N-1). Checks for emptiness and fullness become
 * trivial (O(1)) and do not depend on the relative positions of head and tail.
 * Although the counter requires updating on every operation, in a single-threaded
 * context this provides better readability and predictability without losing
 * the usable buffer capacity.
 *
 * 2. Restricting capacity to powers of two (modulo vs bitmask):
 * Restricting buffer capacity strictly to powers of two is an engineering trade-off.
 * "For": replacing the modulo operation (%) with bitwise AND (& (capacity - 1))
 * works noticeably faster, as the processor performs division longer than bitwise
 * operations.
 * "Against": strict binding to powers of two (2, 4, 8, 16...) is extremely
 * inconvenient in practice. If the user needs a buffer for 100 elements, they
 * will have to allocate memory for 128, leading to unjustified memory waste.
 * In modern systems, where compilers know how to optimize division, and the
 * bottleneck is more often cache misses, universality (support for any capacity)
 * is more important than micro-optimization. Therefore, the code uses the
 * classic modulo operation.
 * =====================================================================
 */

struct RingBuffer {
    int* data;       // Memory block
    size_t capacity; // Capacity
    size_t head;     // Read position
    size_t tail;     // Write position
    size_t count;    // Number of elements (to distinguish empty/full)
};

RB_Error rb_create(size_t capacity, RingBuffer** out_buffer) {
    if (!out_buffer) return RB_ERR_NULL_PTR;
    if (capacity == 0) return RB_ERR_ZERO_CAPACITY;

    RingBuffer* buf = (RingBuffer*)malloc(sizeof(RingBuffer));
    if (!buf) return RB_ERR_ALLOC;

    buf->data = (int*)malloc(capacity * sizeof(int));
    if (!buf->data) {
        free(buf);
        return RB_ERR_ALLOC;
    }

    buf->capacity = capacity;
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;

    *out_buffer = buf;
    return RB_OK;
}

void rb_destroy(RingBuffer* buffer) {
    if (buffer) {
        free(buffer->data);
        free(buffer);
    }
}

RB_Error rb_push(RingBuffer* buffer, int value) {
    if (!buffer) return RB_ERR_NULL_PTR;
    if (buffer->count == buffer->capacity) return RB_ERR_FULL;

    buffer->data[buffer->tail] = value;
    buffer->tail = (buffer->tail + 1) % buffer->capacity; // Wrap around the end
    buffer->count++;

    return RB_OK;
}

RB_Error rb_pop(RingBuffer* buffer, int* out_value) {
    if (!buffer || !out_value) return RB_ERR_NULL_PTR;
    if (buffer->count == 0) return RB_ERR_EMPTY;

    *out_value = buffer->data[buffer->head];
    buffer->head = (buffer->head + 1) % buffer->capacity; // Wrap around the end
    buffer->count--;

    return RB_OK;
}

RB_Error rb_peek(const RingBuffer* buffer, int* out_value) {
    if (!buffer || !out_value) return RB_ERR_NULL_PTR;
    if (buffer->count == 0) return RB_ERR_EMPTY;

    *out_value = buffer->data[buffer->head];
    // Read position is NOT changed
    return RB_OK;
}

size_t rb_size(const RingBuffer* buffer) {
    return buffer ? buffer->count : 0;
}

size_t rb_capacity(const RingBuffer* buffer) {
    return buffer ? buffer->capacity : 0;
}

int rb_is_empty(const RingBuffer* buffer) {
    return buffer ? (buffer->count == 0) : 1;
}

int rb_is_full(const RingBuffer* buffer) {
    return buffer ? (buffer->count == buffer->capacity) : 0;
}