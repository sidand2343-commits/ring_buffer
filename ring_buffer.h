#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>

// Error codes
typedef enum {
    RB_OK = 0,
    RB_ERR_NULL_PTR,
    RB_ERR_FULL,
    RB_ERR_EMPTY,
    RB_ERR_ALLOC,
    RB_ERR_ZERO_CAPACITY
} RB_Error;

// Opaque structure (forward declaration)
typedef struct RingBuffer RingBuffer;

// Creation and destruction
RB_Error rb_create(size_t capacity, RingBuffer** out_buffer);
void rb_destroy(RingBuffer* buffer);

// Data operations
RB_Error rb_push(RingBuffer* buffer, int value);
RB_Error rb_pop(RingBuffer* buffer, int* out_value);
RB_Error rb_peek(const RingBuffer* buffer, int* out_value);

// Metadata
size_t rb_size(const RingBuffer* buffer);
size_t rb_capacity(const RingBuffer* buffer);
int rb_is_empty(const RingBuffer* buffer);
int rb_is_full(const RingBuffer* buffer);

#endif // RING_BUFFER_H