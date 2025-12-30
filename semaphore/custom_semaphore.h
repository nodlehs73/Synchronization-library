#ifndef SEMPAHORE_H
#define SEMPAHORE_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include "futex_wrapper.h"


struct semaphore
{
    atomic_int waiters;
    atomic_int limit;
};

static void semaphore_init(struct semaphore *sem, const int limit)
{
    sem->waiters = 0;
    sem->limit = limit;
}

static void semaphore_wait(struct semaphore *sem)   // lock
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

static void semaphore_post(struct semaphore *sem)   // unlock
{
    atomic_fetch_add(&sem->limit, 1);

    if (atomic_load(&sem->waiters) > 0) 
        futex_wake(&sem->limit, 1);
}

static void print_current_limit(struct semaphore *sem)
{
    printf("current limit is: %d\n", sem->limit); 
}

#endif