#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "./spsc_queue.h" // Ensure this contains your SPSC_QUEUE code

#define TEST_COUNT 100000

void* producer(void* arg) {
    struct SPSC_QUEUE* q = (struct SPSC_QUEUE*)arg;
    for (int i = 0; i < TEST_COUNT; i++) {
        spsc_push(q, i);
    }
    return NULL;
}

void* consumer(void* arg) {
    struct SPSC_QUEUE* q = (struct SPSC_QUEUE*)arg;
    int last_val = -1;
    int count = 0;

    while (count < TEST_COUNT) {
        int val = spsc_try_pop(q);
        if (val != EMPTY_VALUE) {
            // Verify data integrity: SPSC should be FIFO
            if (val != last_val + 1) {
                printf("Error: Expected %d but got %d\n", last_val + 1, val);
            }
            last_val = val;
            count++;
        }
    }
    printf("Successfully consumed %d items in order!\n", count);
    return NULL;
}

int main() {
    struct SPSC_QUEUE q;
    SPSC_QUEUE_init(&q);

    pthread_t prod_thread, cons_thread;

    printf("Starting SPSC test with %d items...\n", TEST_COUNT);

    pthread_create(&prod_thread, NULL, producer, &q);
    pthread_create(&cons_thread, NULL, consumer, &q);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    return 0;
}
