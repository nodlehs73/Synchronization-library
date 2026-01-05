#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "custom_conditional_variables.h"
#include "../mutex/mutex.h"

#define NUM_THREADS 9000

struct mutex mtx;
pthread_mutex_t posix_mtx;

pthread_cond_t c_cond;
struct conditional_variable custom_cond;
int ready;


void *test_custom(void *arg)
{
    mutex_lock(&mtx);

    while(!ready)
    {
        conditional_variable_wait(&custom_cond, &mtx);
    }
    
    mutex_unlock(&mtx);

    return NULL;
}

double run_custom_benchmark(void *(*func)(void *))
{
    pthread_t threads[NUM_THREADS];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    // broadcast
    ready = 0;

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, func, NULL);

    usleep(1000); // to start waiting
    
    ready = 1;
    conditional_variable_broadcast(&custom_cond);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);


    // signal 
    ready = 0;

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, func, NULL);

    usleep(1000); // to start waiting
    
    ready = 1;
    for (int i = 0; i < NUM_THREADS; i++)
        conditional_variable_signal(&custom_cond);
    
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void *test_c(void *arg)
{
    pthread_mutex_lock(&posix_mtx);

    while(!ready)
    {
        pthread_cond_wait(&c_cond, &posix_mtx);
    }
    
    pthread_mutex_unlock(&posix_mtx);

    return NULL;
}

double run_benchmark(void *(*func)(void *))
{
    pthread_t threads[NUM_THREADS];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    // broadcast
    ready = 0;

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, func, NULL);

    usleep(1000); // to start waiting
    
    ready = 1;
    pthread_cond_broadcast(&c_cond);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);


    // signal 
    ready = 0;

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, func, NULL);

    usleep(1000); // to start waiting
    
    ready = 1;
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_cond_signal(&c_cond);
    
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}



int main()
{
    conditional_variable_init(&custom_cond);
    pthread_cond_init(&c_cond, NULL);
    mutex_init(&mtx, RAND_BACKOFF);
    pthread_mutex_init (&posix_mtx, NULL);
    printf("Starting benchmark: %d threads.\n\n", NUM_THREADS);

    double time_custom = run_custom_benchmark(test_custom); ///     change here
    printf("Custom Conditional Variable:                   %.5f seconds\n", time_custom);

    double time_c = run_benchmark(test_c);
    printf("Standard C Conditional Variable:               %.5f seconds\n", time_c);

    printf("\nResult: Custom is \033[0;34m%.2fx\033[0m as fast as C\n", time_c / time_custom);

    pthread_cond_destroy(&c_cond);
    conditional_variable_destroy(&custom_cond);
    return 0;
}
