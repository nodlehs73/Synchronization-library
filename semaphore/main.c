

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

int limit = 3;
sem_t sem;


void *tfun(void *v)
{
    // int *tid = (int *)v;
    printf("%s reached the barrier \n", v);

    sem_wait(&sem);

    printf(" =>%s passed the barrier \n", v);

    sem_post(&sem);

    return NULL;
}


#define N 5

int main(int argc, char *argv[])
{
    pthread_t thr[N];

    sem_init(&sem, 0, limit);

    pthread_create(&thr[0], NULL, tfun, "1");
    pthread_create(&thr[1], NULL, tfun, "2");
    pthread_create(&thr[2], NULL, tfun, "3");
    pthread_create(&thr[3], NULL, tfun, "4");
    pthread_create(&thr[4], NULL, tfun, "5");


    for(int i = 0; i < N; i++) {
        pthread_join(thr[i], NULL);
    }

    sem_destroy(&sem);
    return 0;
}