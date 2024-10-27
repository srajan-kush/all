#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in the buffer

// Semaphores
sem_t empty;    // Tracks empty slots in buffer
sem_t full;     // Tracks full slots in buffer
sem_t mutex;    // Ensures mutual exclusion on buffer access

void *producer(void *arg) {
    int item;
    while (1) {
        item = rand() % 100;  // Producing a random item

        sem_wait(&empty);  // Wait if buffer is full (empty slots are 0)
        sem_wait(&mutex);  // Lock access to the buffer

        // Add item to buffer
        buffer[count] = item;
        count++;
        printf("Produced: %d, Items in buffer: %d\n", item, count);

        sem_post(&mutex);  // Unlock buffer access
        sem_post(&full);   // Signal that buffer has a new item

        // sleep(1);  // Simulate production time
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    while (1) {
        sem_wait(&full);   // Wait if buffer is empty (full slots are 0)
        sem_wait(&mutex);  // Lock access to the buffer

        // Remove item from buffer
        item = buffer[--count];
        printf("Consumed: %d, Items in buffer: %d\n", item, count);

        sem_post(&mutex);  // Unlock buffer access
        sem_post(&empty);  // Signal that buffer has an empty slot

        // sleep(1);  // Simulate consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially, buffer is empty, so we have BUFFER_SIZE empty slots
    sem_init(&full, 0, 0);             // Initially, no items in the buffer, so full is 0
    sem_init(&mutex, 0, 1);            // Mutex initialized to 1 (unlocked)

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Join threads (optional, to wait for them to finish)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Destroy semaphores (optional, if threads are expected to finish)
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
