#include <stdio.h>
#include "ring_buffer.h"

// Вспомогательная функция для красивого вывода логического содержимого буфера
void print_buffer(const RingBuffer* buf) {
    printf("  Logical contents: [ ");
    for (size_t i = 0; i < buf->size; i++) {
        size_t idx = (buf->head + i) % buf->capacity;
        printf("%d ", buf->data[idx]);
    }
    printf("]\n");
}

int main(void) {
    RingBuffer* buf = NULL;
    int val = 0;

    printf("--- Test 1: Creation and Edge Cases (Empty) ---\n");
    if (rb_create(5, &buf) != RB_OK) return 1;
    printf("Created buffer with capacity %zu\n", rb_capacity(buf));

    // Попытка извлечь из пустого
    if (rb_pop(buf, &val) == RB_ERR_EMPTY) {
        printf("  [OK] Pop from empty buffer correctly returned ERR_EMPTY\n");
    }
    // Попытка посмотреть в пустом
    if (rb_peek(buf, &val) == RB_ERR_EMPTY) {
        printf("  [OK] Peek into empty buffer correctly returned ERR_EMPTY\n");
    }

    printf("\n--- Test 2: Filling and Edge Cases (Full) ---\n");
    for (int i = 1; i <= 5; i++) rb_push(buf, i * 10);
    printf("Filled buffer with 5 elements.\n");
    print_buffer(buf);

    // Попытка добавить в полный
    if (rb_push(buf, 999) == RB_ERR_FULL) {
        printf("  [OK] Push to full buffer correctly returned ERR_FULL (data not corrupted)\n");
    }
    print_buffer(buf);

    printf("\n--- Test 3: Wrap-around (Transition through the end) ---\n");
    // Извлекаем 2 элемента (голова сдвигается)
    rb_pop(buf, &val); printf("Popped: %d\n", val);
    rb_pop(buf, &val); printf("Popped: %d\n", val);

    // Добавляем ещё 2 (хвост упрется в конец массива и перейдет в начало)
    rb_push(buf, 60);
    rb_push(buf, 70);
    printf("After popping 2 and pushing 2 more (wrap-around occurred):\n");
    print_buffer(buf); // Ожидается: 30 40 50 60 70

    printf("\n--- Test 4: Single Element and Repeated Deletion ---\n");
    // Опустошаем буфер почти полностью
    while (rb_size(buf) > 1) {
        rb_pop(buf, &val);
    }
    printf("Left 1 element. Peek shows: ");
    rb_peek(buf, &val);
    printf("%d\n", val);

    rb_pop(buf, &val);
    printf("Popped last element: %d. Is empty? %s\n", val, rb_is_empty(buf) ? "Yes" : "No");

    // Повторное удаление из уже пустого
    if (rb_pop(buf, &val) == RB_ERR_EMPTY) {
        printf("  [OK] Repeated deletion from empty buffer handled correctly\n");
    }

    rb_destroy(buf);
    printf("\nBuffer destroyed. Memory freed.\n");

    return 0;
}