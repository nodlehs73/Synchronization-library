#ifndef NO_FUTEX_SEMPAHORE_H
#define NO_FUTEX_SEMPAHORE_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>


struct no_futex_semaphore
{
    atomic_int limit;
};

static void no_futex_semaphore_init(struct no_futex_semaphore *sem, const int limit)
{
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
    }
}

static void no_futex_semaphore_post(struct no_futex_semaphore *sem)   // unlock
{
    atomic_fetch_add(&sem->limit, 1);
}


#endif