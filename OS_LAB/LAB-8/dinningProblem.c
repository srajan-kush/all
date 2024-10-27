#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t s_mutex[N];

void *philosopher(void *arg)
{
    int id = *((int *)arg);

    printf("Philosopher %d is thinking......\n", id);

    sem_wait(&s_mutex[id]);
    sem_wait(&s_mutex[(id + 1) % N]);

    printf("Philosopher %d takes fork %d and %d\n", id, id, (id + 1) % N);
    printf("Philosopher %d is Eating\n", id);

    sleep(1);

    printf("Philosopher %d putting fork %d and %d down\n", id, id, (id + 1) % N);

    sem_post(&s_mutex[id]);
    sem_post(&s_mutex[(id + 1) % N]);

    return NULL;
}

int main()
{
    pthread_t philosophers[N]; // Create threads for N philosophers
    int philosopher_ids[N] = {0, 1, 2, 3, 4};

    // Initialize semaphores
    for(int i = 0; i < N; i++){
        sem_init(&s_mutex[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Join all threads
    for (int i = 0; i < N; i++)
    {
        pthread_join(philosophers[i], NULL); // Corrected this line
    }

    // Destroy semaphores
    for (int i = 0; i < N; i++)
    {
        sem_destroy(&s_mutex[i]);
    }

    return 0;
}
