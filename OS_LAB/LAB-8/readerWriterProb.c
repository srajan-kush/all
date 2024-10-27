#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t rw_mutex;    
sem_t mutex;     
int read_count = 0;

// Shared resource
int shared_data = 1;

void *reader(void *arg)
{
        int reader_id = *(int *)arg;

        // Entry Section (Reader priority)
        sem_wait(&mutex); // Lock mutex to update read_count
        read_count++;
        if (read_count == 1)
        {
            sem_wait(&rw_mutex); // If this is the first reader, lock rw_mutex
        }
        sem_post(&mutex); // Unlock mutex

        // Critical Section (Reading the resource)
        printf("Reader %d is reading the shared data: %d\n", reader_id, shared_data);

        // Exit Section
        sem_wait(&mutex); // Lock mutex to update read_count
        read_count--;
        if (read_count == 0)
        {
            sem_post(&rw_mutex); // If this is the last reader, unlock rw_mutex
        }
        sem_post(&mutex); // Unlock mutex
}

void *writer(void *arg)
{
        int writer_id = *(int *)arg;

        // Entry Section
        sem_wait(&rw_mutex); // Lock rw_mutex to get exclusive access to the shared resource

        // Critical Section (Writing to the resource)
        shared_data++; // Modify the shared resource
        printf("Writer %d is writing to the shared data. New value: %d\n", writer_id, shared_data);

        // Exit Section
        sem_post(&rw_mutex); // Unlock rw_mutex
}

int main()
{
    pthread_t rtid[5], wtid[5]; // Create threads for 5 readers and 5 writers
    int reader_ids[5] = {1, 2, 3, 4, 5};
    int writer_ids[5] = {1, 2, 3, 4, 5};

    // Initialize semaphores
    sem_init(&rw_mutex, 0, 1); // Binary semaphore for writer access (1 allows access, 0 blocks)
    sem_init(&mutex, 0, 1);    // Binary semaphore for mutual exclusion in updating reader count

    // Create reader threads
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&wtid[i], NULL, writer, &writer_ids[i]);
        pthread_create(&rtid[i], NULL, reader, &reader_ids[i]);
    }


    // Join all threads
    for (int i = 0; i < 5; i++)
    {
        pthread_join(rtid[i], NULL);
        pthread_join(wtid[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    return 0;
}