#include <stdio.h>
#include <stdlib.h>
#include "ring_buffer.h"

void print_buffer_state(const RingBuffer* buf) {
    printf("  [State] Size: %zu / %zu | Empty: %s | Full: %s\n",
           rb_size(buf), rb_capacity(buf),
           rb_is_empty(buf) ? "Yes" : "No ",
           rb_is_full(buf)  ? "Yes" : "No ");
}

// Print contents without destroying the buffer (using a temporary array)
void print_buffer_contents(RingBuffer* buf) {
    size_t size = rb_size(buf);
    if (size == 0) {
        printf("  [Contents] <empty>\n");
        return;
    }

    int* temp = (int*)malloc(size * sizeof(int));
    if (!temp) return;

    printf("  [Contents] ");
    for (size_t i = 0; i < size; i++) {
        rb_pop(buf, &temp[i]);
        printf("%d ", temp[i]);
    }
    printf("\n");

    // Put the elements back
    for (size_t i = 0; i < size; i++) {
        rb_push(buf, temp[i]);
    }
    free(temp);
}

int main(void) {
    RingBuffer* buf = NULL;
    RB_Error err;
    int val;

    printf("=== 1. Creating a buffer with capacity 5 ===\n");
    err = rb_create(5, &buf);
    if (err != RB_OK) return 1;
    print_buffer_state(buf);

    printf("\n=== 2. Filling to the limit (edge case: FULL) ===\n");
    for (int i = 1; i <= 5; i++) {
        rb_push(buf, i * 10);
    }
    print_buffer_state(buf);
    print_buffer_contents(buf);

    printf("\n=== 3. Attempting to push into a full buffer ===\n");
    err = rb_push(buf, 999);
    printf("  Push result: %d (Expected %d - RB_ERR_FULL)\n", err, RB_ERR_FULL);
    printf("  Data was not corrupted:\n");
    print_buffer_contents(buf);

    printf("\n=== 4. Extracting half of the elements ===\n");
    for (int i = 0; i < 3; i++) {
        rb_pop(buf, &val);
        printf("  Popped: %d\n", val);
    }
    print_buffer_state(buf);

    printf("\n=== 5. Adding new elements (Position wraps around the end of the array) ===\n");
    printf("  (head and tail are currently at the end, new elements will go to the beginning)\n");
    rb_push(buf, 60);
    rb_push(buf, 70);
    rb_push(buf, 80);
    print_buffer_state(buf);
    print_buffer_contents(buf);

    printf("\n=== 6. Peeking at the first element ===\n");
    rb_peek(buf, &val);
    printf("  Peeked: %d (should be 40, buffer unchanged)\n", val);
    print_buffer_contents(buf);

    printf("\n=== 7. Emptying the buffer (edge case: EMPTY) ===\n");
    while (!rb_is_empty(buf)) {
        rb_pop(buf, &val);
    }
    print_buffer_state(buf);

    printf("\n=== 8. Attempting to pop from an empty buffer ===\n");
    err = rb_pop(buf, &val);
    printf("  Pop result: %d (Expected %d - RB_ERR_EMPTY)\n", err, RB_ERR_EMPTY);

    rb_destroy(buf);
    printf("\n=== Memory successfully freed. Exiting. ===\n");

    return 0;
}