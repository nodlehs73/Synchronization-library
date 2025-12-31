#ifndef NO_FUTEX_SEMPAHORE_H
#define NO_FUTEX_SEMPAHORE_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include "futex_wrapper.h"


struct no_futex_semaphore
{
    atomic_int waiters;
    atomic_int limit;
};

static void no_futex_semaphore_init(struct no_futex_semaphore *sem, const int limit)
{
    sem->waiters = 0;
    sem->limit = limit;
}

static void no_futex_semaphore_wait(struct no_futex_semaphore *sem)   // lock
{
    int current;
    
    while(true)
    {
        current = atomic_load(&sem->limit);

        if(current > 0 && atomic_compare_exchange_strong(&sem->limit, &current, current - 1))
        {
            return;
        }
        else 
        {
            atomic_fetch_add(&sem->waiters, 1);

            futex_wait(&sem->limit, 0);

            atomic_fetch_add(&sem->waiters, -1);
        }
    }
}

static void no_futex_semaphore_post(struct no_futex_semaphore *sem)   // unlock
{
    atomic_fetch_add(&sem->limit, 1);

    if (atomic_load(&sem->waiters) > 0) 
        futex_wake(&sem->limit, 1);
}


#endif
