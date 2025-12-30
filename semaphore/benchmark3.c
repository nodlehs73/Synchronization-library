#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "custom_semaphore.h"
#include "no_futex_custom_semaphore.h"

#define NUM_THREADS 10000
#define Limit 10

struct semaphore my_sem;
struct no_futex_semaphore my_no_futex_sem;
sem_t c_sem;

void *test_custom_no_futex(void *v)
{
    no_futex_semaphore_wait(&my_no_futex_sem);
    __asm__ volatile("" : : : "memory");
    no_futex_semaphore_post(&my_no_futex_sem);

    return NULL;
}

void *test_custom(void *v)
{
    semaphore_wait(&my_sem);
    __asm__ volatile("" : : : "memory");
    semaphore_post(&my_sem);

    return NULL;
}

void *test_c_sem(void *v)
{
    sem_wait(&c_sem);
    __asm__ volatile("" : : : "memory");
    sem_post(&c_sem);

    return NULL;
}

double run_benchmark(void *(*func)(void *))
{
    pthread_t threads[NUM_THREADS];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, func, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main()
{
    no_futex_semaphore_init(&my_no_futex_sem, Limit);
    semaphore_init(&my_sem, Limit);
    sem_init(&c_sem, 0, Limit);

    printf("Starting benchmark: %d threads, %d limit for the semaphores.\n\n", NUM_THREADS, Limit);

    double time_no_futex_custom = run_benchmark(test_custom_no_futex);
    printf("Custom Semaphore without Futex:     %.5f seconds\n", time_no_futex_custom);

    double time_custom = run_benchmark(test_custom);
    printf("Custom Semaphore:                   %.5f seconds\n", time_custom);

    double time_c = run_benchmark(test_c_sem);
    printf("Standard C Semaphore:               %.5f seconds", time_c);

    printf("\n\nResult: Custom is %.2fx as fast as C\n", time_c / time_custom);

    sem_destroy(&c_sem);
    return 0;
}