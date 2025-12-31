#ifndef CONDITIONAL_VARIABLES
#define CONDITIONAL_VARIABLES

#include <pthread.h>
#include <semaphore.h>

struct conditional_variable
{
    int waiters_count;
    sem_t internal_semaphore;
    pthread_mutex_t internal_mutex;
};

void conditional_variable_init(struct conditional_variable *cond)
{
    cond->waiters_count = 0;
    pthread_mutex_init(&cond->internal_mutex, NULL);
    sem_init(&cond->internal_semaphore, 0, 0);
}

void conditional_variable_wait(struct conditional_variable *cond, pthread_mutex_t *mtx)
{
    pthread_mutex_lock(&cond->internal_mutex);
    cond->waiters_count++;
    pthread_mutex_unlock(&cond->internal_mutex);

    pthread_mutex_unlock(mtx);

    sem_wait(&cond->internal_semaphore);

    pthread_mutex_lock(mtx);
}

void conditional_variable_signal(struct conditional_variable *cond)
{
    pthread_mutex_lock(&cond->internal_mutex);

    if (cond->waiters_count > 0)
    {
        cond->waiters_count--;
        sem_post(&cond->internal_semaphore);
    }

    pthread_mutex_unlock(&cond->internal_mutex);
}

void conditional_variable_broadcast(struct conditional_variable *cond)
{
    pthread_mutex_lock(&cond->internal_mutex);

    while (cond->waiters_count > 0)
    {
        sem_post(&cond->internal_semaphore);
        cond->waiters_count--;
    }

    pthread_mutex_unlock(&cond->internal_mutex);
}

void conditional_variable_destroy(struct conditional_variable *cond)
{
    pthread_mutex_destroy(&cond->internal_mutex);
    sem_destroy(&cond->internal_semaphore);
}

#endif