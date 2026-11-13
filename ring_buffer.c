#include "ring_buffer.h"
#include <stdlib.h>

RingBufferError rb_create(size_t capacity, RingBuffer** out_buffer) {
   if (!out_buffer) return RB_ERR_NULL_PTR;
   if (capacity == 0) return RB_ERR_INVALID_CAP;

   RingBuffer* buf = (RingBuffer*)malloc(sizeof(RingBuffer));
   if (!buf) return RB_ERR_ALLOC;

   buf->data = (int*)malloc(capacity * sizeof(int));
   if (!buf->data) {
      free(buf);
      return RB_ERR_ALLOC;
   }

   buf->capacity = capacity;
   buf->size = 0;
   buf->head = 0;
   buf->tail = 0;

   *out_buffer = buf;
   return RB_OK;
}

void rb_destroy(RingBuffer* buffer) {
   if (buffer) {
      free(buffer->data);
      free(buffer);
   }
}

RingBufferError rb_push(RingBuffer* buffer, int value) {
   if (!buffer) return RB_ERR_NULL_PTR;
   if (buffer->size == buffer->capacity) return RB_ERR_FULL;

   buffer->data[buffer->tail] = value;
   // Переход через конец массива с помощью остатка от деления
   buffer->tail = (buffer->tail + 1) % buffer->capacity;
   buffer->size++;

   return RB_OK;
}

RingBufferError rb_pop(RingBuffer* buffer, int* out_value) {
   if (!buffer || !out_value) return RB_ERR_NULL_PTR;
   if (buffer->size == 0) return RB_ERR_EMPTY;

   *out_value = buffer->data[buffer->head];
   buffer->head = (buffer->head + 1) % buffer->capacity;
   buffer->size--;

   return RB_OK;
}

RingBufferError rb_peek(const RingBuffer* buffer, int* out_value) {
   if (!buffer || !out_value) return RB_ERR_NULL_PTR;
   if (buffer->size == 0) return RB_ERR_EMPTY;

   *out_value = buffer->data[buffer->head];
   return RB_OK;
}

size_t rb_size(const RingBuffer* buffer) {
   return buffer ? buffer->size : 0;
}

size_t rb_capacity(const RingBuffer* buffer) {
   return buffer ? buffer->capacity : 0;
}

bool rb_is_empty(const RingBuffer* buffer) {
   return buffer ? (buffer->size == 0) : true;
}

bool rb_is_full(const RingBuffer* buffer) {
   return buffer ? (buffer->size == buffer->capacity) : false;
}