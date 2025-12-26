#include <stdio.h>
#include "mutex.h"
#include <stdatomic.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define TO_INCREASE (1 << 20) 

atomic_int atomic_counter;
struct mutex mut;

void* increase_counter_atomic (void *data) {
  for (int i = 0; i < TO_INCREASE; ++i) {
    atomic_fetch_add (&atomic_counter, 1);
  }

  return NULL;
} 

int counter;
void* increase_counter_mutex (void* data) {
  for (int i = 0; i < TO_INCREASE; ++i) {
    mutex_lock (&mut);
    ++counter;
    mutex_unlock(&mut);
  }

  return NULL;
}

const char* STRATEGIES[] = {"EXP_BACKOFF", "RAND_BACKOFF", "FIXED_BACKOFF", "DEFAULT_BACKOFF", "NO_BACKOFF"};

int main(int argc, char* argv[]) {
  const int number_strategies = sizeof (STRATEGIES) / sizeof (char*);
  int STRATEGY = 1;
  for (int i = 0; i < number_strategies; ++i) {
    if (strcmp (argv[2], STRATEGIES[i]) == 0) {
      STRATEGY = i + 1;
    }
  }


  mutex_init (&mut, STRATEGY);
  
  pthread_t threads[16];
  const int NUM_THREADS = atoi (argv[1]);
  
  clock_t start = clock();
  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_create (&threads[i], NULL, increase_counter_mutex, NULL);
  }

  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join (threads[i], NULL);
  }
  
  double cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
  printf ("%.6f\n", cpu_time / NUM_THREADS); 
  fflush (stdout);
}
