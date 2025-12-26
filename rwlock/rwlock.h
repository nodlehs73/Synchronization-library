#ifndef RWLOCK_H
#define RWLOCK_H

#include <stdatomic.h>

struct rwlock {
  atomic_int serving;
  atomic_int ticket;
  atomic_int readers;
};

void rwlock_init (struct rwlock* lock) {
  atomic_store (&lock->serving, 0);
  atomic_store (&lock->ticket, 0);
  atomic_store (&lock->readers, 0);
}


void lock (struct rwlock* lock) {
  const int ticket_order = atomic_fetch_add (&lock->ticket, 1);
  
  while (ticket_order != atomic_load (&lock->serving) || atomic_load (&lock->readers) != 0)
    ;
}

void shared_lock (struct rwlock* lock) {
  const int ticket_order = atomic_fetch_add (&lock->ticket, 1);

  while (ticket_order != atomic_load (&lock->serving)) 
    ;

  atomic_fetch_add (&lock->readers, 1);
  atomic_fetch_add (&lock->serving, 1);
}

void unlock (struct rwlock* lock) {
  atomic_fetch_add (&lock->serving, 1);
}

void shared_unlock (struct rwlock* lock) {
  atomic_fetch_add (&lock->readers, -1); 
}
#endif
