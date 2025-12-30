#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "custom_semaphore.h"
#include <time.h>

void sleep_01_sec() {
    struct timespec ts;
    ts.tv_sec = 0;                         // 0 seconds
    ts.tv_nsec = 10 * 10 * 1000;         // 10 million nanoseconds (0.01s)
    nanosleep(&ts, NULL);
}


// sem_t sem;
struct semaphore sem;

void *tfun(void *v)
{
    // sem_wait(&sem);
    
    // int *tid = (int *)v;
    printf("%s reached the barrier \n", v);
    print_current_limit(&sem);
    
    semaphore_wait(&sem);

    sleep_01_sec();

    printf(" ->%s passed\n", v);

    // sem_post(&sem);
    semaphore_post(&sem);
    print_current_limit(&sem);

    return NULL;
}

int limit = 10;
#define N 100000

int main(int argc, char *argv[])
{
    pthread_t thr[N];

    // sem_init(&sem, 0, limit);
    semaphore_init(&sem, limit);
    print_current_limit(&sem);

    for(int i = 0; i < N; i++)
    {
        char *s = malloc(12 * sizeof(char));
        sprintf(s, "%d", i);

        pthread_create(&thr[i], NULL, tfun, s);
    }

    for(int i = 0; i < N; i++) {
        pthread_join(thr[i], NULL);
    }

    print_current_limit(&sem);

    // sem_destroy(&sem);
    return 0;
}