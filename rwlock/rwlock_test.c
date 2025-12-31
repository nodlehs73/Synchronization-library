#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "rwlock.h"

int counter = 0;
struct rwlock rwmutex;

void* read_counter (void *data) {
  int thread_idx = *((int*) data);

  shared_lock (&rwmutex);
  printf ("READING %d from thread with idx %d\n", counter, thread_idx);
  
  shared_unlock (&rwmutex);
  return NULL;
}

void* modify_counter (void* data) {
  int thread_idx = *((int*) data);

  lock (&rwmutex);
  counter++;
  
  unlock (&rwmutex);
  printf ("MODIFYING COUNTER TO %d WITH THREAD_IDX %d\n", counter, thread_idx);

  return NULL;
}

pthread_t threads[10000];

int main(int argc, char* argv[]) {
  rwlock_init (&rwmutex);

  const int READER_CHANCE = atoi (argv[1]);
  const int NUMBER_THREADS = atoi (argv[2]);
  
  for (int i = 0; i < NUMBER_THREADS; ++i) {
    const int chance = arc4random_uniform (100) + 1;
    
    int* thread_idx = (int*) malloc (sizeof (int));
    *thread_idx = i;
    
    if (chance <= READER_CHANCE) {
      pthread_create (&threads[i], NULL, read_counter, (void*) thread_idx);
    } else {
      pthread_create (&threads[i], NULL, modify_counter, (void*) thread_idx);
    }
  }

  for (int i = 0; i < NUMBER_THREADS; ++i) {
    pthread_join (threads[i], NULL);
  }
}
