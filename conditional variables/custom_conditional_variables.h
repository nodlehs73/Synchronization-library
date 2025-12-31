#ifndef CONDITIONAL_VARIABLES
#define CONDITIONAL_VARIABLES

// #include <semaphore.h>
#include "../mutex/mutex.h"
#include "../semaphore/custom_semaphore.h"

struct conditional_variable
{
    int waiters_count;
    struct semaphore internal_semaphore;
    struct mutex internal_mutex;   //// asta
};

void conditional_variable_init(struct conditional_variable *cond)
{
    cond->waiters_count = 0;
    mutex_init(&cond->internal_mutex, RAND_BACKOFF);    /// asta
    semaphore_init(&cond->internal_semaphore, 0);
}

void conditional_variable_wait(struct conditional_variable *cond, struct mutex *mtx)
{
    mutex_lock(&cond->internal_mutex);  /// asta
    cond->waiters_count++;
    mutex_unlock(&cond->internal_mutex); /// asta

    mutex_unlock(mtx);  // <-- asta nu, asta e parametru

    semaphore_wait(&cond->internal_semaphore);

    mutex_lock(mtx);   // <-- asta nu, asta e parametru
}

void conditional_variable_signal(struct conditional_variable *cond)
{
    mutex_lock(&cond->internal_mutex);  // asta

    if (cond->waiters_count > 0)
    {
        cond->waiters_count--;
        semaphore_post(&cond->internal_semaphore);
    }

    mutex_unlock(&cond->internal_mutex);    // asta
}

void conditional_variable_broadcast(struct conditional_variable *cond)
{
    mutex_lock(&cond->internal_mutex);  // asta

    while (cond->waiters_count > 0)
    {
        semaphore_post(&cond->internal_semaphore);
        cond->waiters_count--;
    }

    mutex_unlock(&cond->internal_mutex);  // asta
}

void conditional_variable_destroy(struct conditional_variable *cond)
{
    //mutex_destroy(&cond->internal_mutex);  // asta
    // sem_destroy(&cond->internal_semaphore);
}

#endif
