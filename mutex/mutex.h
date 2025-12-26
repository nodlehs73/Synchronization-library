#ifndef MUTEX_H
#define MUTEX_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_BACKOFF 2048
#define MIN_BACKOFF 64

#define EXP_BACKOFF 1
#define RAND_BACKOFF 2
#define FIXED_BACKOFF 3
#define DEFAULT_BACKOFF 4
#define NO_BACKOFF 5

struct mutex {
  atomic_bool lock;
  void (*lock_function)(struct mutex *);
};

void mutex_unlock(struct mutex *mut) { atomic_store(&mut->lock, false); }

void mutex_lock(struct mutex *mut) { mut->lock_function(mut); }

void exp_backoff(struct mutex *mut) {
  int current_backoff = MIN_BACKOFF;

  while (true) {
    if (!atomic_exchange(&mut->lock, true)) {
      return;
    }

    do {
      for (volatile int i = 0; i < current_backoff; ++i)
        ;

      if (current_backoff < MAX_BACKOFF) {
        current_backoff <<= 1;
      }

    } while (atomic_load(&mut->lock));
  }
}

void rand_backoff(struct mutex *mut) {
  while (true) {
    if (!atomic_exchange(&mut->lock, true)) {
      return;
    }

    do {
      const int BACKOFF_ITER =
          arc4random_uniform(MAX_BACKOFF - MIN_BACKOFF) + MIN_BACKOFF;

      for (volatile int i = 0; i < BACKOFF_ITER; ++i)
        ;

    } while (atomic_load(&mut->lock));
  }
}

void default_backoff(struct mutex *mut) {
  while (atomic_exchange(&mut->lock, true))
    ;
}

void fixed_backoff(struct mutex *mut) {
  while (true) {
    if (!atomic_exchange(&mut->lock, true)) {
      return;
    }

    do {
      for (volatile int i = 0; i < 150; ++i)
        ;

    } while (atomic_load(&mut->lock));
  }
}

void no_backoff(struct mutex *mut) {
  while (true) {
    if (!atomic_exchange(&mut->lock, true)) {
      return;
    }

    while (atomic_load(&mut->lock))
      ;
  }
}

void mutex_init(struct mutex *mut, const int LOCK_TYPE) {
  atomic_store(&mut->lock, false);

  switch (LOCK_TYPE) {
  case EXP_BACKOFF:
    mut->lock_function = exp_backoff;
    break;
  case RAND_BACKOFF:
    mut->lock_function = rand_backoff;
    break;
  case FIXED_BACKOFF:
    mut->lock_function = fixed_backoff;
    break;
  case NO_BACKOFF:
    mut->lock_function = no_backoff;
    break;
  default:
    mut->lock_function = default_backoff;
  }
}

#endif
