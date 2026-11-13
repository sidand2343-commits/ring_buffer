#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdbool.h>

// Структура кольцевого буфера
typedef struct {
    int* data;       // Сырая память для элементов
    size_t capacity; // Максимальная ёмкость
    size_t size;     // Текущее количество элементов (решает проблему full/empty)
    size_t head;     // Позиция чтения (откуда забираем)
    size_t tail;     // Позиция записи (куда кладём)
} RingBuffer;

// Коды ошибок
typedef enum {
    RB_OK = 0,
    RB_ERR_NULL_PTR,       // Передан нулевой указатель
    RB_ERR_INVALID_CAP,    // Некорректная ёмкость (0)
    RB_ERR_ALLOC,          // Ошибка выделения памяти
    RB_ERR_FULL,           // Буфер полон
    RB_ERR_EMPTY           // Буфер пуст
} RingBufferError;

// Интерфейс
RingBufferError rb_create(size_t capacity, RingBuffer** out_buffer);
void rb_destroy(RingBuffer* buffer);

RingBufferError rb_push(RingBuffer* buffer, int value);
RingBufferError rb_pop(RingBuffer* buffer, int* out_value);
RingBufferError rb_peek(const RingBuffer* buffer, int* out_value);

size_t rb_size(const RingBuffer* buffer);
size_t rb_capacity(const RingBuffer* buffer);
bool rb_is_empty(const RingBuffer* buffer);
bool rb_is_full(const RingBuffer* buffer);

#endif // RING_BUFFER_H